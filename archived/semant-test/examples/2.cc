#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;


static bool return_exist = false; // if return exists
Symbol return_type; // return's type
int curr_stmt_num = 0; // all kinds of sentence blocks
int loop_num = 0; // the num of for and while's loop sentence blocks
int callDecl_num = 0 ; // the num of call  declare
//tables:
SymbolTable<Symbol,Symbol> *varname_type_table  = new SymbolTable<Symbol,Symbol>(); //variable name and type table
SymbolTable<Symbol,Symbol> *paraname_type_table = new SymbolTable<Symbol,Symbol>(); //parameter name and type table
SymbolTable<Symbol,Symbol> *funcname_type_table = new SymbolTable<Symbol,Symbol>(); //function name and type table
SymbolTable<Symbol,Symbol> *localvarname_type_table = new SymbolTable<Symbol,Symbol>(); //loacal variable name and type table
SymbolTable<Symbol,Variables> *funcname_paras_table = new SymbolTable<Symbol, Variables>();//function name and parameter table


///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////


static ostream& semant_error() {
    semant_errors++;
    return error_stream;
}

static ostream& semant_error(tree_node *t) {
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream& internal_error(int lineno) {
    error_stream << "FATAL:" << lineno << ": ";
    return error_stream;
}

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
//all error
//Error[101]:function print can't be redefined!!!\n
//Error[102]:function was already defined!!!\n
//Error[103]:var cannot be the type of void. void can only be used as return's type!!!\n
//Error[104]:var was already defined!!!\n
//Error[105]:function main is not defined!!!\n
//Error[106]:function main should not have any parameters!!!\n
//Error[107]:function main should have return with the type of void!!!\n
//Error[108]:the return type:  is incorrect!!!\n
//Error[109]:function's parameter has an invalid type void!!!\n
//Error[110]:function's parameter has a duplicate name:!!!\n
//Error[111]:function should have an overall return statement!!!\n
//Error[112]:condition must be boolean, not!!!\n
//Error[113]:return is the type of , but should be  !!!\n
//Error[114]:continue must be used in a loop sentence!!!\n
//Error[115]:break must be used in a loop sentence!!!\n
//Error[116]:function print must have at least one parameter of type string!!!\n
//Error[117]:function print's first parameter must be of type string.\n
//Error[118]:function has not been defined!!!\n
//Error[119]:function is called with wrong number of arguments!!!\n
//Error[120]:for function , the  parameter should be ,but provided a !!!\n
//Error[121]:left value has not been defined!!!\n
//Error[122]:right value should be the type of , not !!!\n
//Error[123]:You can't add   and   !!!\n
//Error[124]:You can't minus   and   !!!\n
//Error[125]:You can't multi   and   !!!\n
//Error[126]:You can't divide   and   !!!\n
//Error[127]:You can't mod   and   !!!\n
//Error[128]:the  can't do a operation of neg!!!\n
//Error[129]:you can't do a comparation of lt between and !!!\n
//Error[130]:you can't do a comparation of le between and !!!\n
//Error[131]:you can't do a comparation of equ between and !!!\n
//Error[132]:you can't do a comparation of neq between and !!!\n
//Error[133]:you can't do a comparation of ge between and !!!\n
//Error[134]:you can't do a comparation of gt between and !!!\n
//Error[135]:you can't use and(&&) to do operation between and !!!\n
//Error[136]:you can't use or(||) to do operation between and !!!\n
//Error[137]:you can't use xor(^) to do operation between and !!!\n
//Error[138]:you can't use not(!) to do operation upon !!!\n
//Error[139]:you can't use bitand(&) to do operation between and !!!\n
//Error[140]:you can't use bitor(|) to do operation between and !!!\n
//Error[141]:you can't use bitnot(~) to do operation upon !!!\n
//Error[142]:var has not been defined!!!\n
static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print;

bool isValidCallName(Symbol type) {
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type) {
    return type != Void;
}

//
// Initializing the predefined symbols.
//

static void initialize_constants(void) {
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // Main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/

static bool sameType(Symbol name1, Symbol name2) {
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}

static void install_calls(Decls decls) {
    funcname_type_table->enterscope();
    funcname_paras_table->enterscope();
    funcname_type_table->addid(print,new Symbol(Void)); // print into table
    for(int i = decls->first();decls->more(i);i=decls->next(i)){
        curr_decl = decls->nth(i);
        if(curr_decl->isCallDecl() == true){
           if(curr_decl->getName() == print){
               semant_error(curr_decl)<<"Error[101]:function print can't be redefined!!!\n";
               continue;
           }
           //if it isn't in tables, push
           if(funcname_type_table->lookup(curr_decl->getName())==NULL){
               funcname_type_table->addid(curr_decl->getName(),new Symbol(curr_decl->getType()));
               funcname_paras_table->addid(curr_decl->getName(),new Variables(curr_decl->getVariables()));
           }
           //if it is in tables
           else{
               semant_error(curr_decl)<<"Error[102]:function "<<curr_decl->getName()
               <<" was already defined!!!\n";
           }
        }
    }
}

static void install_globalVars(Decls decls) {
    varname_type_table->enterscope();
    for(int i = decls->first();decls->more(i);i = decls->next(i)){
        curr_decl = decls->nth(i);
        if(curr_decl->isCallDecl()==false){
            if (curr_decl->getType()==Void){
                semant_error(curr_decl)<<"Error[103]:var "<<curr_decl->getName() 
                <<" cannot be the type of void. void can only be used as return's type!!!\n";
                continue;
            }
            if(varname_type_table->lookup(curr_decl->getName()) == NULL){
                varname_type_table->addid(curr_decl->getName(),new Symbol(curr_decl->getType()));
            }
            else{
                semant_error(curr_decl)<<"Error[104]:var "<<curr_decl->getName()
                <<" was already defined!!!\n";
            }
        }
    }
     
}

static void check_calls(Decls decls) {
    for(int i = decls->first();decls->more(i);i = decls->next(i)){
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl()){
            curr_decl->check();
        }
    }
}

static void check_main() {
    if(funcname_type_table->lookup(Main) == NULL){
        semant_error()<<"Error[105]:function main is not defined!!!\n";
    }
}

void VariableDecl_class::check() {
    if(getType() == Void){
        semant_error(this)<<"Error[103]:var "<< getName()
        <<" cannot be the type of void. void can only be used as return's type!!!\n";
    }
}

void CallDecl_class::check() {
    callDecl_num++;
    if(getName()==Main){
        if(getVariables()->len()>0){
            semant_error(this)<<"Error[106]:function main should not have any parameters!!!\n";
        }
        if(getType()!=Void){
            semant_error(this)<<"Error[107]:function main should have return with the type of void!!!\n";
        }
    }
    if(getType() != Int && getType() != Float && getType() != String && getType() != Bool && getType() != Void){
        semant_error(this)<<"Error[108]:the return type: "<<getType()  
        <<" is incorrect!!!\n";
    }
    Variables para1 = getVariables();
    paraname_type_table->enterscope();
    for(int i = para1->first();para1->more(i);i = para1->next(i)){
        if(para1->nth(i)->getType() == Void){
            semant_error(this)<<"Error[109]:function "<<getName()
            <<"'s parameter has an invalid type void!!!\n";
            continue;
        }
        if(paraname_type_table->probe(para1->nth(i)->getName())==NULL){
            paraname_type_table->addid(para1->nth(i)->getName(),new Symbol(para1->nth(i)->getType()));
        }
        else{
            semant_error(this)<<"Error[110]:function "<<getName()
            <<"'s parameter has a identical name:"
            <<para1->nth(i)->getName()<<"!!!\n";
        }
    }
    return_exist = false;
    return_type = getType();
    body->check(Int);
    if(!return_exist){
        semant_error(this)<<"Error[111]:function "<<getName()
        <<" should have an total return statement!!!\n";
    }
    callDecl_num --;
}

void StmtBlock_class::check(Symbol type) {
     curr_stmt_num ++;
     VariableDecls varbs = getVariableDecls();
     localvarname_type_table->enterscope();
     for(int i = varbs->first();varbs->more(i);i = varbs->next(i)){
        varbs->nth(i)->check();
        if(varbs->nth(i)->getType() == Void){
            continue;
        }
        if(localvarname_type_table->probe(varbs->nth(i)->getName())==NULL){
            localvarname_type_table->addid(varbs->nth(i)->getName(),new Symbol(varbs->nth(i)->getType()));
        }
        else{
            semant_error(varbs->nth(i))<<"Error[104]:var "<<varbs->nth(i)->getName()
            <<" was already defined!!!\n";
        }
    }
    Stmts stmts = getStmts();
    for(int i = stmts->first();stmts->more(i);i=stmts->next(i)){
        stmts->nth(i)->check(Int);
    }
    curr_stmt_num--;
    localvarname_type_table->exitscope();
}

void IfStmt_class::check(Symbol type) {
    condition->check(Int);
    if(condition->getType()!=Bool){
        semant_error(this)<<"Error[112]:condition must be boolean, not "
        <<condition->getType()<<"!!!\n";
    }
    thenexpr->check(Int);
    elseexpr->check(Int);
}

void WhileStmt_class::check(Symbol type) {
    loop_num++;
    condition->check(Int);
    if(condition->getType()!=Bool){
        semant_error(this)<<"Error[112]:condition must be boolean, not "
        <<condition->getType()<<"!!!\n";
    }
    body->check(Int);
    loop_num--;
}

void ForStmt_class::check(Symbol type) {
    loop_num++;
    initexpr->check(Int);
    condition->check(Int);
    if(condition->is_empty_Expr() == false){
        if(condition->getType()!=Bool){
            semant_error(this)<<"Error[112]:condition must be boolean, not "
            <<condition->getType()<<"!!!\n";
        }
    }
    loopact->check(Int);
    body->check(Int);
    loop_num--;
}

void ReturnStmt_class::check(Symbol type) {
    if (curr_stmt_num == callDecl_num){
        return_exist = true;
    }
    getValue()->check(Int);
    if(return_type != getValue()->getType()){
        semant_error(this)<<"Error[113]:current return is the type of "
        <<getValue()->getType()<<", but it should be "
        <<return_type<<"!!!\n";
    }
}

void ContinueStmt_class::check(Symbol type) {
    if(loop_num == 0){
        semant_error(this)<<"Error[114]:continue must be used in a loop sentence!!!\n";
    }
}

void BreakStmt_class::check(Symbol type) {
   if(loop_num == 0){
       semant_error(this)<<"Error[115]:break must be used in a loop sentence!!!\n";
   }
}

Symbol Call_class::checkType(){
    if (getName() == print){
        Actuals real_variable = getActuals();
        if (real_variable->len() == 0)
        {
            semant_error(this) <<"Error[116]:function print must have at least one parameter with the type of string!!!\n"; //function can't have no para
            setType(Void); 
            return type;
        }
        real_variable->nth(0)->check(Int);
        if (real_variable->nth(0)->getType() != String){
            semant_error(this) <<"Error[117]:function print's first parameter should be the type of string.\n"; //print's first para must be type of string
        }
        else{
            for (int i = real_variable->first(); real_variable->more(i); i = real_variable->next(i)){
                real_variable->nth(i)->check(Int);
            }       
        }
        setType(Void); 
        return type;
    }
    if (funcname_type_table->lookup(getName()) == NULL){
        semant_error(this) << "Error[118]:function "<<getName()
        <<" has not been defined!!!\n";
        setType(Void);
        return type;
    }
    else{
        if (funcname_paras_table->lookup(getName()) != NULL){
            Variables v_in_table = *funcname_paras_table->lookup(getName());
            int acts_in_table_len = v_in_table->len();
            if (getActuals()->len() != acts_in_table_len){
                semant_error(this) << "Error[119]:function "<<getName() 
                <<" is called with wrong number of arguments!!!\n";  
            }
            else{ 
                Actuals acts = getActuals();
                for (int i = acts->first(); acts->more(i); i = acts->next(i)){
                    acts->nth(i)->check(Int);
                    if (acts->nth(i)->getType() != v_in_table->nth(i)->getType()){
                        semant_error(this) << "Error[120]:for function " << getName() << ", the " << i + 1
                                           << " parameter should be " << v_in_table->nth(i)->getType()
                                           << ",but you provided a wrong type of " << acts->nth(i)->getType() 
                                           << "!!!\n"; // wrong para type
                        break;
                    }
                }
            }
        }
        Symbol ty = *funcname_type_table->lookup(getName());
        setType(ty);
        return type;
    }
}

Symbol Actual_class::checkType(){
    expr->check(Int);
    setType(expr->getType()); 
    return type;
}

Symbol Assign_class::checkType(){
    value->check(Int);
    Symbol s;
    if (localvarname_type_table->lookup(lvalue) == NULL && paraname_type_table->probe(lvalue) == NULL && varname_type_table->lookup(lvalue) == NULL){
        semant_error(this) << "Error[121]:left value "
        <<lvalue<<" has not been defined!!!\n";
        setType(Void);
        return type;
    }
    //first local var ,then para table ,last var out
    else if (localvarname_type_table->lookup(lvalue) != NULL) {
        s = *localvarname_type_table->lookup(lvalue);
        if (s != value->getType())
            semant_error(this) << "Error[122]:right value should be the type of " << s
                               << " , not " << value->getType() << "!!!\n";
    }
    else if (paraname_type_table->probe(lvalue) != NULL){ //para in table can be repetitive
        s = *paraname_type_table->probe(lvalue);
        if (s != value->getType())
            semant_error(this) << "Error[122]:right value should be the type of " << s
                               << " , not " << value->getType() << "!!!\n";
    }
    else if (varname_type_table->lookup(lvalue) != NULL){
        s = *varname_type_table->lookup(lvalue);
        if (s != value->getType())
            semant_error(this) << "Error[122]:right value should be the type of " << s
                               << " , not " << value->getType() << "!!!\n";
    }
    setType(s);
    return type;
}

Symbol Add_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float)){
        setType(Float);
        return type;
    }
    else if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[123]:You can't add "
        <<e1->getType()<<" and "
        <<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Minus_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float)){
        setType(Float);
        return type;
    }
    else if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[124]:You can't minus "
        <<e1->getType()<<" and "
        <<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Multi_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float)){
        setType(Float);
        return type;
    }
    else if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[125]:You can't multi "
        <<e1->getType()<<" and "
        <<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Divide_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float)){
        setType(Float);
        return type;
    }
    else if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[126]:You can't divide "
        <<e1->getType()<<" and "
        <<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Mod_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[127]:You can't mod "
        <<e1->getType()<<" and "
        <<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Neg_class::checkType(){
    e1->check(Int);
    if (e1->getType() == Int || e1->getType() == Float){
        setType(e1->getType());
        return type;
    }
    else{
        semant_error(this) << "Error[128]:the "<< e1->getType()
                           << " can't do a operation of neg!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Lt_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[129]:you can't do a comparation of lt between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Le_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[130]:you can't do a comparation of le between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Equ_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int) ||
        (e1->getType() == Bool && e2->getType() == Bool)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[131]:you can't do a comparation of equ between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Neq_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int) ||
        (e1->getType() == Bool && e2->getType() == Bool)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[132]:you can't do a comparation of neq between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Ge_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[133]:you can't do a comparation of ge between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Gt_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Int && e2->getType() == Float) ||
        (e1->getType() == Float && e2->getType() == Int) ||
        (e1->getType() == Float && e2->getType() == Float) ||
        (e1->getType() == Int && e2->getType() == Int)){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[134]:you can't do a comparation of gt between "
        <<e1->getType()<<" and "<<e2->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol And_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if (e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[135]:you can't use and(&&) to do operation between "
        << e1->getType()<< " and " << e2->getType() << "!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Or_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if (e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[136]:you can't use or(||) to do operation between "
        << e1->getType()<< " and " << e2->getType() << "!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Xor_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if ((e1->getType() == Bool && e2->getType() == Bool)||
        (e1->getType() == Int && e2->getType() == Int)){
        setType(e1->getType());
        return type;
    }
    else{
        semant_error(this) << "Error[137]:you can't use xor(^) to do operation between "
        << e1->getType()<< " and " << e2->getType() << "!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Not_class::checkType(){
    e1->check(Int);
    if (e1->getType() == Bool){
        setType(Bool);
        return type;
    }
    else{
        semant_error(this) << "Error[138]:you can't use not(!) to do operation upon "
        <<e1->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Bitand_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[139]:you can't use bitand(&) to do operation between "
        << e1->getType()<< " and " << e2->getType() << "!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Bitor_class::checkType(){
    e1->check(Int);
    e2->check(Int);
    if (e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[140]:you can't use bitor(|) to do operation between "
        <<e1->getType()<< " and " << e2->getType() << "!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Bitnot_class::checkType(){
    e1->check(Int);
    if (e1->getType() == Int){
        setType(Int);
        return type;
    }
    else{
        semant_error(this) << "Error[141]:you can't use bitnot(~) to do operation upon "
        <<e1->getType()<<"!!!\n";
        setType(Void);
        return type;
    }
}

Symbol Const_int_class::checkType(){
    setType(Int);
    return type;
}

Symbol Const_string_class::checkType(){
    setType(String);
    return type;
}

Symbol Const_float_class::checkType(){
    setType(Float);
    return type;
}

Symbol Const_bool_class::checkType(){
    setType(Bool);
    return type;
}

Symbol Object_class::checkType(){
    Symbol Obt;
    if (localvarname_type_table->lookup(var) == NULL && paraname_type_table->probe(var) == NULL && varname_type_table->lookup(var) == NULL){
        semant_error(this) << "Error[142]:var "<<var
        <<" has not been defined!!!\n";
        setType(Void);
        return Void;
    }
    else if (localvarname_type_table->lookup(var) != NULL){
        Obt = *localvarname_type_table->lookup(var);
    }
    else if (paraname_type_table->probe(var) != NULL){
        Obt = *paraname_type_table->probe(var);
    }
    else if (varname_type_table->lookup(var) != NULL){
        Obt = *varname_type_table->lookup(var);
    }
    setType(Obt);
    return type;
}

Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}

void Program_class::semant() {
    initialize_constants();
    install_calls(decls);
    check_main();
    install_globalVars(decls);
    check_calls(decls);
    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



