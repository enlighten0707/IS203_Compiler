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

static bool return_flag = false;
static std::stack<bool> return_stack;
static std::stack<bool> loop_stack;

typedef std::map<Symbol, CallDecl_class*> CallTable; 
CallTable callTable;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;

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
    print,
    Anytypes
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
    // Main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");

    Anytypes    = idtable.add_string("Anytypes");
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
    for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl()){
            Symbol cname = curr_decl->getName();
            if (cname == print) 
                semant_error(curr_decl) << "Function printf cannot be redefination.\n";
            if (callTable.find(curr_decl->getName()) == callTable.end()) {
                if (isValidCallName(cname)) {
                    callTable[curr_decl->getName()] = dynamic_cast<CallDecl_class*>(curr_decl);
                }
                else {
                    semant_error(curr_decl) << "Function " << curr_decl->getName() << " cannot have a name as " << cname << ".\n";
                }
            }
            else {
                semant_error(curr_decl) << "Function " << curr_decl->getName() << " was previously defined.\n";
            }
        }
    }
}

static void install_globalVars(Decls decls) {
    objectEnv.enterscope();
    for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl() != true) {
            curr_decl->check();
        }
    }
}

static void check_calls(Decls decls) {
    for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl()) {
            curr_decl->check();
        }
    }
}

static void check_main() {
    if (callTable.find(Main) != callTable.end()) {
        curr_decl = callTable[Main];
        if (dynamic_cast<CallDecl_class*>(curr_decl)->getVariables()->len() > 0) {
            semant_error(curr_decl) << "Main function should not have any parameters.\n";
        }
        if (curr_decl->getType() != Void) {
            semant_error(curr_decl) << "Main function should have return type Void.\n";
        }
    }
    else {
        semant_error() << "Main function is not defined.\n";
    }
}

void VariableDecl_class::check() {
    if (objectEnv.probe(this->getName()) == NULL) {
        if (isValidTypeName(this->getType()) == true) {
            objectEnv.addid(this->getName(), new Symbol(this->getType()));
        }
        else {
            semant_error(this->variable) << "var " << this->getName() << " cannot be of type Void. Void can just be used as return type.\n";
        }
    }
    else {
        semant_error(this->variable) << "var" << this->getName() << " was previously defined.\n";
    }
}

void CallDecl_class::check() {
    objectEnv.enterscope();
    return_flag = false;
    if (this->paras->len()>6) {
        semant_error(curr_decl) << "Function " << curr_decl->getName() << " has more than 6 parameters.\n";
    }
    for (int i = this->paras->first(); this->paras->more(i); i = this->paras->next(i)) {
        if (isValidTypeName(this->paras->nth(i)->getType()) == true) {
            if (objectEnv.probe(this->paras->nth(i)->getName()) == NULL) {
                objectEnv.addid(this->paras->nth(i)->getName(), new Symbol(this->paras->nth(i)->getType()));
            }
            else {
                semant_error(curr_decl) << "Function " << curr_decl->getName() << " \'s parameter has a duplicate name " << this->paras->nth(i)->getName() << ".\n";
            }
        }
        else {
            semant_error(curr_decl) << "Function " << curr_decl->getName() << " \'s parameter has an invalid type " << this->paras->nth(i)->getType() << ".\n";
        }
    }
    this->body->check(this->getType());
    objectEnv.exitscope();
    if (return_flag != true) {
        semant_error(curr_decl) << "Function " << curr_decl->getName() << " must have an overall return statement.\n";
    }
}

void StmtBlock_class::check(Symbol type) {
    objectEnv.enterscope();
    return_stack.push(true);
    for (int i = this->vars->first(); this->vars->more(i); i = this->vars->next(i)) {
        this->vars->nth(i)->check();
    }
    for (int i = this->stmts->first(); this->stmts->more(i); i = this->stmts->next(i)) {
        this->stmts->nth(i)->check(type);
    }
    return_stack.pop();
    objectEnv.exitscope();
}

void IfStmt_class::check(Symbol type) {
    return_stack.push(true);
    Symbol ttype = this->getCondition()->checkType();
    if ((ttype != Bool) && (ttype != Anytypes)) {
        semant_error(this->getCondition()) << "Condition must be a Bool, got " << ttype << "\n";
    }
    this->getThen()->check(type);
    this->getElse()->check(type);
    return_stack.pop();
}

void WhileStmt_class::check(Symbol type) {
    return_stack.push(true);
    loop_stack.push(true);
    Symbol ttype = this->getCondition()->checkType();
    if ((ttype != Bool) && (ttype != Anytypes)) {
        semant_error(this->getCondition()) << "Condition must be a Bool, got " << ttype << "\n";
    }
    this->getBody()->check(type);
    return_stack.pop();
    loop_stack.pop();
}

void ForStmt_class::check(Symbol type) {
    return_stack.push(true);
    loop_stack.push(true);
    this->getInit()->checkType();
    this->getLoop()->checkType();
    Symbol ttype = this->getCondition()->checkType();
    if (ttype != Bool) {
        if ((this->getCondition()->is_empty_Expr() != true ) && (ttype != Anytypes)) {
            semant_error(this->getCondition()) << "Condition must be a Bool, got " << ttype << "\n";
        }
    }
    this->getBody()->check(type);
    return_stack.pop();
    loop_stack.pop();
}

void ReturnStmt_class::check(Symbol type) {
    if (return_stack.size() == 1) {
        return_flag = true;
    }
    Symbol ttype = this->getValue()->checkType();
    if ((ttype != type) && (ttype != Anytypes)) {
        semant_error(this->getValue()) << "Returns " << ttype << " , but need " << type << "\n";
    }
}

void ContinueStmt_class::check(Symbol type) {
    if (loop_stack.empty() != false) {
        semant_error(this) << "continue must be used in a loop sentence.\n";
    }
}

void BreakStmt_class::check(Symbol type) {
    if (loop_stack.empty() != false) {
        semant_error(this) << "break must be used in a loop sentence.\n";
    }
}

Symbol Call_class::checkType(){
    bool type_error;
    int k1;
    int k2;
    int i;
    if (this->name == print) {
        if (this->actuals->len() == 0) {
            semant_error(this) << "printf() must has at last one parameter of type String.\n";
            return Void;
        }
        else {
            i = this->actuals->first();
            if (this->actuals->nth(i)->checkType() == String) {
                while (this->actuals->more(i) != 0) {
                    this->actuals->nth(i)->checkType();
                    i = this->actuals->next(i);
                }
                setType(Void);
                return Void;
            }
            else {
                semant_error(this) << "printf()\'s first parameter must be of typeString.\n";
                return Void;
            }
        }
    } 
    else {
        if (callTable.find(this->name) != callTable.end()) {
            type_error = false;
            k1 = this->actuals->first();
            k2 = callTable[this->name]->getVariables()->first();
            while (callTable[this->name]->getVariables()->more(k2) != 0) {
                if (this->actuals->more(k1) == 0) {
                    semant_error(this) << "Function " << this->name << " called with wrong number of arguments.\n";
                    break;
                }
                Symbol ttype = this->actuals->nth(k1)->checkType();
                if ((ttype != callTable[this->name]->getVariables()->nth(k2)->getType()) && (ttype != Anytypes)) {
                    semant_error(this) << "Function " << this->name << ", the " << k1 + 1 << " parameter should be " << callTable[this->name]->getVariables()->nth(k2)->getType() << " but provided a " << ttype << ".\n";
                    type_error = true;
                    break;
                }
                k1 = this->actuals->next(k1);
                k2 = this->actuals->next(k2);
            }
            if ((type_error) || (this->actuals->more(k1)) == 0) {
                type_error = false;
            }
            else {
                type_error = true;
            }
            if (type_error) {
                semant_error(this) << "Function " << this->name << " called with wrong number of arguments.\n";
            }
            setType(callTable[this->name]->getType());
            return this->type;
        }
        else {
            semant_error(this) << "Function " << this->name << " has not been defined.\n";
            return Anytypes;
        }
    }
}

Symbol Actual_class::checkType(){
    Symbol ttype = this->expr->checkType();
    setType(ttype);
    return ttype;
}

Symbol Assign_class::checkType(){
    if (objectEnv.lookup(this->lvalue) == 0) {
        semant_error(this) << "Left value " << this->lvalue << " has not been defined.\n";
        return Void;
    }
    else {
        Symbol rtype = this->value->checkType();
        if (*objectEnv.lookup(this->lvalue) == rtype) {
            setType(*objectEnv.lookup(this->lvalue));
        }
        else if (rtype == Anytypes) {
            setType(Anytypes);
        }
        else if ((*objectEnv.lookup(this->lvalue) == Float) || (rtype == Int)) {
            setType(Float);
        }
        else {
            setType(Anytypes);
            semant_error(this) << "Right value must have type " << *objectEnv.lookup(this->lvalue) << " , got " << rtype << ".\n";
        }
    }
    return type;
}

Symbol Add_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Float) && (rtype == Int)) {
        setType(Float);
    }
    else if ((ltype == Int) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Float) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot add a " << ltype << " and a " << rtype << ".\n";
    }
    return type;
}

Symbol Minus_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Float) && (rtype == Int)) {
        setType(Float);
    }
    else if ((ltype == Int) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Float) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot minus a " << ltype << " by a " << rtype << ".\n";
    }
    return type;
}

Symbol Multi_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Float) && (rtype == Int)) {
        setType(Float);
    }
    else if ((ltype == Int) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Float) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot multiply a " << ltype << " with a " << rtype << ".\n";
    }
    return type;
}

Symbol Divide_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Float) && (rtype == Int)) {
        setType(Float);
    }
    else if ((ltype == Int) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Float) && (rtype == Float)) {
        setType(Float);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot divide a " << ltype << " by a " << rtype << ".\n";
    }
    return type;
}

Symbol Mod_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot mod a " << ltype << " and a " << rtype << ".\n";
    }
    return type;
}

Symbol Neg_class::checkType(){
    Symbol ltype = e1->checkType();
    if ((ltype == Int) || (ltype == Float) || (ltype == Anytypes)) {
        setType(ltype);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "A " << ltype << " doesn\'t have a negative.\n";
    }
    return type;
}

Symbol Lt_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(less).\n";
    }
    return type;
}

Symbol Le_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(less than or equal).\n";
    }
    return type;
}

Symbol Equ_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Bool) && (rtype == Bool)) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(equal).\n";
    }
    return type;
}

Symbol Neq_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Bool) && (rtype == Bool)) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(not equal).\n";
    }
    return type;
}

Symbol Ge_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(greater than or equal).\n";
    }
    return type;
}

Symbol Gt_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if (((ltype == Int) || (ltype == Float)) && ((rtype == Int || (rtype == Float)))) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot compare a " << ltype << " and a " << rtype << "(greater).\n";
    }
    return type;
}

Symbol And_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Bool) && (rtype == Bool)) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use and(&&) between " << ltype << " and " << rtype << ".\n";
    }
    return type;
}

Symbol Or_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Bool) && (rtype == Bool)) {
        setType(Bool);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use or(||) between " << ltype << " and " << rtype << ".\n";
    }
    return type;
}

Symbol Xor_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Bool) && (rtype == Bool)) {
        setType(Bool);
    }
    else if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use xor(^) between " << ltype << " and " << rtype << ".\n";
    }
    return type;
}

Symbol Not_class::checkType(){
    Symbol ltype = e1->checkType();
    if (ltype == Bool) {
        setType(Bool);
    }
    else if (ltype == Anytypes) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use not(!) upon " << ltype << ".\n";
    }
    return type;
}

Symbol Bitand_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use bit-and(&) between " << ltype << " and " << rtype << ".\n";
    }
    return type;
}

Symbol Bitor_class::checkType(){
    Symbol ltype = e1->checkType();
    Symbol rtype = e2->checkType();
    if ((ltype == Int) && (rtype == Int)) {
        setType(Int);
    }
    else if ((ltype == Anytypes) || (rtype == Anytypes)) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use bit-or(|) between " << ltype << " and " << rtype << ".\n";
    }
    return type;
}

Symbol Bitnot_class::checkType(){
    Symbol ltype = e1->checkType();
    if (ltype == Int) {
        setType(Int);
    }
    else if (ltype == Anytypes) {
        setType(Anytypes);
    }
    else {
        setType(Anytypes);
        semant_error(this) << "Cannot use unary not(~) upon " << ltype << ".\n";
    }
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
    if (objectEnv.lookup(this->var) == 0) {
        setType(Anytypes);
        semant_error(this) << "object " << this->var << " has not been defined.\n";
        return type;
    } 
    else {
        setType(*objectEnv.lookup(this->var));
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



