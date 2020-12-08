#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

std::set<Symbol> func;      
std::set<Decl> functable;   
std::set<Symbol> formpa;    
static int circ=0;
// std::set<Symbol> globals;   

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

struct P
{
    Symbol name,type;
    P(Symbol n, Symbol t){
        name = n;
        type = t;
    }
};


typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment *objectEnv = new SymbolTable<Symbol, Symbol>();
std::map<Symbol, std::vector<P>> formvarTab;

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

static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;

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
    // main function
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
    // formvarTab->enterscope();
    for(int i = decls->first(); decls->more(i); i = decls->next(i)){
        if (decls->nth(i)->isCallDecl()){
            Symbol name = decls->nth(i)->getName();
            Symbol t = decls->nth(i)->getType();
            if (func.find(name) == func.end()){
                func.insert(decls->nth(i)->getName());
            }
            else 
                semant_error(decls->nth(i))<< "Function "<< name<<" was previously defined." << endl;
            functable.insert(decls->nth(i));

            if(decls->nth(i)->getName()==Main)
                continue;


            std::vector<P> parms;
            P *q = new P(name ,decls->nth(i)->getreturnType());
            parms.push_back(*q);
            formpa.clear();
            Variables vars = decls->nth(i)->getvars();
            for(int i = vars->first(); vars->more(i); i = vars->next(i)){
                if (sameType(vars->nth(i)->getType(), Void)){
                    semant_error(vars->nth(i))<<"Function " << name << " 's parameter has an invalid type Void." << endl;
                }
                if (formpa.find(vars->nth(i)->getName()) == formpa.end()){
                    formpa.insert(vars->nth(i)->getName());
                }
                else
                {
                    semant_error(vars->nth(i))<< "Function "<<name<<"'s parameter has a duplicate name " << vars->nth(i)->getName() <<"." << endl;
                }
                P *p = new P(vars->nth(i)->getName(), vars->nth(i)->getType());
                parms.push_back(*p);
            }
        formvarTab.insert(std::pair<Symbol, std::vector<P>>(decls->nth(i)->getName(), parms));
            
            
        }
    }
}

static void install_globalVars(Decls decls) {
    objectEnv->enterscope();
    for(int i = decls->first(); decls->more(i); i = decls->next(i)){
        if (decls->nth(i)->isCallDecl() == false){
            if(decls->nth(i)->getType() == Void){
                semant_error(decls)<<"var "<< decls->nth(i)->getName() <<" cannot be of type Void." << endl;
            }
            else{
            Symbol name = decls->nth(i)->getName();
            Symbol type = decls->nth(i)->getType();
            if((objectEnv->probe(name) == NULL)){
                objectEnv->addid(name, new Symbol(type));
            }
            else
            {
                semant_error(decls->nth(i))<<"var "<< name <<" was previously defined." << endl;


            }


            }
        }
    }
    // objectEnv->exitscope();

}

static void check_main() {
    objectEnv->enterscope();
    if (func.find(Main) == func.end()){
        semant_error() << "Main function is not defined." << endl;
    }
    else{
        std::set< Decl >::iterator it;
        Decl m_func;
        for (it=functable.begin();it!=functable.end();it++){
            if ((*it)->getName() == Main){
                m_func = *it;
                functable.erase(it);
                break;
            }
        }


        if (m_func->getType() != Void){
            semant_error(m_func) << "Main function should have return type Void."<<endl;

        }
        if(m_func->ifvar()){
            semant_error(m_func) << "Main function should not have any parameters."<<endl;
        }
        m_func->check();
    }
    objectEnv->exitscope();
}

static void check_calls(Decls decls) {

    std::set< Decl >::iterator it;
    for (it=functable.begin();it!=functable.end();it++){
        (*it)->check();
    }
}

void VariableDecl_class::check() {

    Symbol type = getType();
    if(type == Void){
        semant_error(this) << "var "<< getName() <<" cannot be of type Void." << endl;
    }
    else if(objectEnv->probe(getName()) != NULL){
        semant_error(this) << "var "<< getName() <<" was previously defined."<<endl;
    }
    else{
        objectEnv->addid(getName(), new Symbol(getType()));
    }
}
bool NoReturn = 0;
void CallDecl_class::check() {
    Symbol type = getreturnType();
    if(type != Int && type != Bool && type != String && type != Float && type != Void)
    {
        semant_error(this) << "Return type is wrong." << endl;
    }


    objectEnv->enterscope();
    Variables varDecls = getVariables();
    for(int i = varDecls->first(); varDecls->more(i); i = varDecls->next(i)){
        Symbol t = varDecls->nth(i)->getType();
        Symbol n = varDecls->nth(i)->getName();
        if(objectEnv->probe(n) == NULL){
            objectEnv->addid(n, new Symbol(t));
        }
    }

    NoReturn = 0;
    getBody()->check(type);
    if (NoReturn){
        semant_error(this) << "Function "<<getName()<<" must have an overall return statement." << endl;
    }
    objectEnv->exitscope();
}


void StmtBlock_class::check(Symbol type) {

    objectEnv->enterscope();
    bool existreturn = 0;
    VariableDecls varDecls = getVariableDecls();
    for(int i = varDecls->first(); varDecls->more(i); i = varDecls->next(i)){
        varDecls->nth(i)->check();
    }


    Stmts stmts = getStmts();
    for(int i = stmts->first(); stmts->more(i); i = stmts->next(i)){
        if (stmts->nth(i)->iscirc())
            circ += 1;
        stmts->nth(i)->check(type);
        if (stmts->nth(i)->iscirc())
            circ -= 1;
        if(!existreturn)
            existreturn = stmts->nth(i)->isRETURN();
    }

    if (objectEnv->getlayer() == 3 &&!existreturn) 
        NoReturn = 1;
        
    objectEnv->exitscope();

}

void IfStmt_class::check(Symbol type) {
    getCondition()->checkType();
    if(!sameType(getCondition()->getType(), Bool)){
        semant_error(this)<< "Condition must be a Bool, got " << getCondition()->getType() << endl;
    }

    getThen()->check(type);
    getElse()->check(type);
}

void WhileStmt_class::check(Symbol type) {
    getCondition()->checkType();
    if(!sameType(getCondition()->getType(), Bool)){
        semant_error(this)<< "Condition must be a Bool, got " << getCondition()->getType() << endl;
    }
    getBody()->check(type);
}

void ForStmt_class::check(Symbol type) {
    getInit()->checkType();
    getCondition()->checkType();
    getLoop()->checkType();

    if(!sameType(getCondition()->getType(), Bool) && !(getCondition()->is_empty_Expr())){
        semant_error(this)<< "Condition must be a Bool, got " << getCondition()->getType() << endl;
    }
    getBody()->check(type);
}

void ReturnStmt_class::check(Symbol type) {

    if (!getValue()->is_empty_Expr()){
        Symbol t = getValue()->checkType();
        if(!sameType(t, type)){
            semant_error(this)<< "Returns " << t << " , but need " << type  << '.'<<endl;
        }
    }
    else
    if(!sameType(Void, type)){
        semant_error(this)<< "Returns Void , but need " << type  << '.'<<endl;
    }
}

void ContinueStmt_class::check(Symbol type) {
    if(circ <= 0)
        semant_error(this)<< "continue must be used in a loop sentence." << endl;
}

void BreakStmt_class::check(Symbol type) {
    if(circ <= 0)
        semant_error(this)<< "break must be used in a loop sentence." << endl;
}

Symbol Call_class::checkType(){
    std::map<Symbol, std::vector<P>>::iterator l_it;
    l_it = formvarTab.find(getName());
    if(l_it == formvarTab.end()){
        semant_error(this)<<"Function " << getName() << " has not been defined." << endl;
    }
    else{
        P return_value = *(formvarTab[getName()].begin());
        setType(return_value.type);
        std::vector<P> vec = formvarTab[getName()];
        std::vector<P>::iterator v_lt;
        int sum = 0;
        int i = getActuals()->first();
        bool flag = 0;
        for(v_lt=vec.begin() + 1;v_lt!=vec.end();v_lt++){
            if(!getActuals()->more(i)){
                flag = 1;
            } 
            else{
                sum ++;
                Symbol t = getActuals()->nth(i)->checkType();
                // cout << t << ' ' << *(v_lt);
                // cout << "Sum" << ' '<<sum << ' ' << i << endl;
                if (!sameType(t, (*v_lt).type)){
                    semant_error(this)<< "Function " << getName() << ", the " << sum << " parameter should be " << \
                    (*v_lt).type << " but provided a " << t << '.'<<endl;
                }
                i = getActuals()->next(i);
                // cout << "i is ::"  << i << endl;
            }
        }
        if(getActuals()->more(i)) flag = 1;
        if(flag)
            semant_error(this)<< "Function " << getName() << " called with wrong number of arguments."<<endl;
    }
    return type;
}

Symbol Actual_class::checkType(){
    setType(expr->checkType());
    return type;
}

Symbol Assign_class::checkType(){
    Object left = new Object_class(lvalue);
    left -> checkType();
    value->checkType();
    if (sameType(left->getType(), value->getType()))
        setType(value->getType());
    else{
        semant_error(this)<< "Right value must have type " << left->getType() << " , got " << value->getType()<< "."<<endl;
        setType(left->getType());
    }
    return type;
}

Symbol Add_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Float){
        setType(Float);
        return type;
    }
    semant_error(this)<< "Cannot add a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Minus_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Float){
        setType(Float);
        return type;
    }
    semant_error(this)<< "Cannot minus a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Multi_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Float){
        setType(Float);
        return type;
    }
    semant_error(this)<< "Cannot multi a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Divide_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Int){
        setType(Float);
        return type;
    }
    if(e2->getType() == Float && e1->getType() == Float){
        setType(Float);
        return type;
    }
    semant_error(this)<< "Cannot div a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Mod_class::checkType(){
//只能Int和Int
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    semant_error(this)<< "Cannot mod a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Neg_class::checkType(){
//Int和Float
    e1->checkType();
    if(e1->getType() == Int){
        setType(Int);
        return type;
    }
    if(e1->getType() == Float){
        setType(Float);
        return type;
    }
    semant_error(this)<< "A " << e1->getType() << " doesn't have a negative."<<endl;
    setType(Void);
    return type;
}

Symbol Lt_class::checkType(){
//Int和Float和Bool
//Int和Float
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Le_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Equ_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Neq_class::checkType(){
    e1->checkType();
    e2->checkType();

    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Ge_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Gt_class::checkType(){
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Float){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Float && e2->getType() == Int){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot compare a " << e1->getType() << " and a " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol And_class::checkType(){
//Bool和Bool
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot use && between " << e1->getType() << " and " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Or_class::checkType(){
//Bool和Bool
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot use || between " << e1->getType() << " and " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Xor_class::checkType(){
//Bool和Bool，Int和Int
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Bool && e2->getType() == Bool){
        setType(Bool);
        return type;
    }
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    semant_error(this)<< "Cannot use ^ between " << e1->getType() << " and " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Not_class::checkType(){
//Bool
    e1->checkType();
    if(e1->getType() == Bool){
        setType(Bool);
        return type;
    }
    semant_error(this)<< "Cannot use ! upon " << e1->getType()<<"."<<endl;
    setType(Void);
    return type;
}

Symbol Bitand_class::checkType(){
//Int和Int
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    semant_error(this)<< "Cannot use & between " << e1->getType() << " and " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Bitor_class::checkType(){
//Int和Int
    e1->checkType();
    e2->checkType();
    if(e1->getType() == Int && e2->getType() == Int){
        setType(Int);
        return type;
    }
    semant_error(this)<< "Cannot use | between " << e1->getType() << " and " << e2->getType()<< "."<<endl;
    setType(Void);
    return type;
}

Symbol Bitnot_class::checkType(){
//Int
    e1->checkType();
    if(e1->getType() == Int){
        setType(Int);
        return type;
    }
    semant_error(this)<< "Cannot use unary op ~ upon " << e1->getType() << "."<<endl;
    setType(Void);
    return type;
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
    if(objectEnv->probe(var) != NULL){
        Symbol t = *(objectEnv->probe(var));
        setType(t);
        return type;
    }
    else if (objectEnv->lookup(var) != NULL){
        Symbol t = *(objectEnv->lookup(var));
        setType(t);
        return type;
    }
    else {
        semant_error(this)<< "object "<<var<<" has not been defined." << endl;
        setType(Void);
        return type;
    }
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



