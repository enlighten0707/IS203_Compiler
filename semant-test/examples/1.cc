#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream &error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;
static bool ifExistReturnStmt = false;
Symbol return_type;
int curr_stmtBlock_level = 0;
int loop_stmtBlock_level = 0; // curr > loop means being inside a loop
int callDecl_level = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;
SymbolTable<Symbol, Symbol> *func_table = new SymbolTable<Symbol, Symbol>();
SymbolTable<Symbol, Variables> *func_paras_table = new SymbolTable<Symbol, Variables>();
SymbolTable<Symbol, Symbol> *formalPara_table = new SymbolTable<Symbol, Symbol>();
SymbolTable<Symbol, Symbol> *var_table = new SymbolTable<Symbol, Symbol>();
SymbolTable<Symbol, Symbol> *localVar_table = new SymbolTable<Symbol, Symbol>();
///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////

static ostream &semant_error()
{
    semant_errors++;
    return error_stream;
}

static ostream &semant_error(tree_node *t)
{
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream &internal_error(int lineno)
{
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
    print;

bool isValidCallName(Symbol type)
{
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type)
{
    return type != Void;
}

//
// Initializing the predefined symbols.
//

static void initialize_constants(void)
{
    // 4 basic types and Void type
    Bool = idtable.add_string("Bool");
    Int = idtable.add_string("Int");
    String = idtable.add_string("String");
    Float = idtable.add_string("Float");
    Void = idtable.add_string("Void");
    // Main function
    Main = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/

static bool sameType(Symbol name1, Symbol name2)
{
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}

// create a table of func definitions
static void install_calls(Decls decls)
{
    func_table->enterscope();
    func_paras_table->enterscope();
    // printf
    func_table->addid(print, new Symbol(Void));

    for (int i = decls->first(); decls->more(i); i = decls->next(i))
    {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl() == 1)
        {
            if (curr_decl->getName() == print)
            {
                semant_error(curr_decl) << "Function printf cannot be redefined.\n";
                continue;
            }
            //no match, initialize
            if (func_table->lookup(curr_decl->getName()) == NULL)
            {
                func_table->addid(curr_decl->getName(), new Symbol(curr_decl->getType()));
                func_paras_table->addid(curr_decl->getName(), new Variables(curr_decl->getVariables()));
            }
            //match
            else
                semant_error(curr_decl) << "Function " << curr_decl->getName()
                                        << " was previously defined.\n";
        }
    }
}

// create a table of global variable definitions
static void install_globalVars(Decls decls)
{
    var_table->enterscope();
    for (int i = decls->first(); decls->more(i); i = decls->next(i))
    {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl() == 0)
        {
            if (curr_decl->getType() == Void)
            {
                semant_error(curr_decl) << "var " << curr_decl->getName()
                                        << " cannot be of type Void. Void can just be used as return type.\n";
                continue;
            }

            //no match, initialize
            if (var_table->lookup(curr_decl->getName()) == NULL)
                var_table->addid(curr_decl->getName(), new Symbol(curr_decl->getType()));
            //match
            else
                semant_error(curr_decl) << "var " << curr_decl->getName()
                                        << " was previously defined.\n";
        }
    }
}

static void check_calls(Decls decls)
{
    for (int i = decls->first(); decls->more(i); i = decls->next(i))
    {
        curr_decl = decls->nth(i);
        if (curr_decl->isCallDecl())
            curr_decl->check();
    }
}

static void check_main()
{
    // check existence
    if (func_table->lookup(Main) == NULL)
        semant_error() << "Main function is not defined.\n";
}

void VariableDecl_class::check()
{
    if (getType() == Void)
        semant_error(this) << "var " << getName()
                           << " cannot be of type Void. Void can just be used as return type.\n";
}

void CallDecl_class::check()
{
    callDecl_level++;
    // return type not in <Int, Float, Bool, Void, String>
    if (getType() != Int && getType() != Void && getType() != String && getType() != Float && getType() != Bool)
        semant_error(this) << "Incorrect return type: " << getType() << '\n';

    // check main return, paras
    if (getName() == Main)
    {
        if (getType() != Void)
            semant_error(this) << "Main function should have return type Void.\n";
        if (getVariables()->len() != 0)
            semant_error(this) << "Main function should not have any parameters.\n";
    }

    //check formal paras
    Variables vs = getVariables();
    if (vs->len() > 6)
        semant_error(this) << "Function " << getName() << " has more than 6 parameters.\n";
    formalPara_table->enterscope();
    for (int i = vs->first(); vs->more(i); i = vs->next(i))
    {
        // no para with type Void
        if (vs->nth(i)->getType() == Void)
        {
            semant_error(this) << "Function " << getName()
                               << "'s parameter has an invalid type Void.\n";
            continue;
        }
        // no repeatedly named para
        if (formalPara_table->probe(vs->nth(i)->getName()) == NULL)
            formalPara_table->addid(vs->nth(i)->getName(), new Symbol(vs->nth(i)->getType()));
        else
            semant_error(this) << "Function " << getName()
                               << "'s parameter has a duplicate name "
                               << vs->nth(i)->getName() << ".\n";
    }

    ifExistReturnStmt = false;
    return_type = getType();

    // check stmtblock
    getBody()->check(Int);

    //check return stmt
    if (!ifExistReturnStmt)
        semant_error(this) << "Function " << getName()
                           << " must have an overall return statement.\n";

    callDecl_level--;
}

void StmtBlock_class::check(Symbol type)
{
    curr_stmtBlock_level++;
    // check local variable declares, add to symTable
    VariableDecls vDecls = getVariableDecls();
    localVar_table->enterscope();
    for (int i = vDecls->first(); vDecls->more(i); i = vDecls->next(i))
    {
        vDecls->nth(i)->check();
        if (vDecls->nth(i)->getType() == Void)
            continue;
        
        if (localVar_table->probe(vDecls->nth(i)->getName()) == NULL)
            localVar_table->addid(vDecls->nth(i)->getName(), new Symbol(vDecls->nth(i)->getType()));
        else
            semant_error(vDecls->nth(i)) << "var " << vDecls->nth(i)->getName() << " was previously defined.\n";
    }

    // check stmts
    Stmts stmts = getStmts();
    for (int i = stmts->first(); stmts->more(i); i = stmts->next(i))
    {
        stmts->nth(i)->check(Int);
    }

    curr_stmtBlock_level--;
    localVar_table->exitscope();
}

void IfStmt_class::check(Symbol type)
{
    //cout << "IF\n";
    getCondition()->check(Int);
    if (getCondition()->getType() != Bool)
        semant_error(this) << "Condition must be a Bool, got "
                           << getCondition()->getType() << '\n';
    getThen()->check(Int);
    getElse()->check(Int);
}

void WhileStmt_class::check(Symbol type)
{
    loop_stmtBlock_level++;
    //cout << "WHILE\n";
    getCondition()->check(Int);
    if (getCondition()->getType() != Bool)
        semant_error(this) << "Condition must be a Bool, got "
                           << getCondition()->getType() << '\n';
    getBody()->check(Int);
    loop_stmtBlock_level--;
}

void ForStmt_class::check(Symbol type)
{
    loop_stmtBlock_level++;
    //cout << "FOR\n";
    getInit()->check(Int);
    getCondition()->check(Int);
    if (getCondition()->is_empty_Expr() == false)
        if (getCondition()->getType() != Bool)
            semant_error(this) << "Condition must be a Bool, got "
                               << getCondition()->getType() << '\n';
    getLoop()->check(Int);
    getBody()->check(Int);
    loop_stmtBlock_level--;
}

void ReturnStmt_class::check(Symbol type)
{
    //cout << "RETURN\n";
    if (curr_stmtBlock_level == callDecl_level)
        ifExistReturnStmt = true;
    getValue()->check(Int);
    if (return_type != getValue()->getType())
        semant_error(this) << "Returns " << getValue()->getType() << " , but need "
                           << return_type << '\n';
}

void ContinueStmt_class::check(Symbol type)
{
    //cout << "CONTINUE;\n";
    if (loop_stmtBlock_level == 0)
    {
        semant_error(this) << "continue must be used in a loop sentence.\n";
    }
}

void BreakStmt_class::check(Symbol type)
{
    //cout << "BREAK\n";
    if (loop_stmtBlock_level == 0)
    {
        semant_error(this) << "break must be used in a loop sentence.\n";
    }
}

Symbol Call_class::checkType()
{
    //cout << "CALL\n";
    // printf
    if (getName() == print)
    {
        Actuals acs = getActuals();
        if (acs->len() < 1)
        {
            semant_error(this) << "printf() must have at least one parameter of type String.\n";
            setType(Void);
            return type;
        }
        acs->nth(0)->check(Int);
        if (acs->nth(0)->getType() != String)
            semant_error(this) << "printf()'s first parameter must be of type String.\n";
        else
        {
            for (int i = acs->first(); acs->more(i); i = acs->next(i))
                acs->nth(i)->check(Int);
        }
        setType(Void);
        return type;
    }
    if (func_table->lookup(getName()) == NULL)
    {
        semant_error(this) << "Function " << getName() << " has not been defined.\n";
        setType(Void);
        return type;
    }
    else
    {
        // check actuals
        if (func_paras_table->lookup(getName()) != NULL)
        {
            Variables v = *func_paras_table->lookup(getName());
            int acts_len = v->len();
            // actual number
            if (getActuals()->len() != acts_len)
                semant_error(this) << "Function " << getName()
                                   << " called with wrong number of arguments.\n";
            else
            { //actual type
                Actuals acts = getActuals();
                for (int i = acts->first(); acts->more(i); i = acts->next(i))
                {
                    acts->nth(i)->check(Int);
                    if (acts->nth(i)->getType() != v->nth(i)->getType())
                    {
                        semant_error(this) << "Function " << getName() << ", the " << i + 1
                                           << " parameter should be " << v->nth(i)->getType()
                                           << " but provided a " << acts->nth(i)->getType()
                                           << ".\n";
                        break;
                    }
                }
            }
        }
        Symbol ty = *func_table->lookup(getName());
        setType(ty);
        return type;
    }
}

Symbol Actual_class::checkType()
{
    //cout << "ACTUAL\n";
    getList()->check(Int);
    setType(getList()->getType());
    return type;
}

Symbol Assign_class::checkType()
{
    //cout << "ASSIGN\n";
    getRight()->check(Int);
    Symbol s;
    // check lvalue
    if (localVar_table->lookup(getLeft()) == NULL && formalPara_table->probe(getLeft()) == NULL && var_table->lookup(getLeft()) == NULL)
    {
        semant_error(this) << "Left value " << getLeft() << " has not been defined.\n";
        setType(Void);
        return type;
    }
    // check right value
    else if (localVar_table->lookup(getLeft()) != NULL)
    {
        s = *localVar_table->lookup(getLeft());
        if (s != getRight()->getType())
            if (! (s == Float && getRight()->getType() == Int))
                semant_error(this) << "Right value must have type " << s
                                << " , got " << getRight()->getType() << ".\n";
    }
    else if (formalPara_table->probe(getLeft()) != NULL)
    {
        s = *formalPara_table->probe(getLeft());
        if (s != getRight()->getType())
            if (! (s == Float && getRight()->getType() == Int))
                semant_error(this) << "Right value must have type " << s
                                << " , got " << getRight()->getType() << ".\n";
    }
    else if (var_table->lookup(getLeft()) != NULL)
    {
        s = *var_table->lookup(getLeft());
        if (s != getRight()->getType())
            if (! (s == Float && getRight()->getType() == Int))
                semant_error(this) << "Right value must have type " << s
                                << " , got " << getRight()->getType() << ".\n";
    }
    setType(s);
    return type;
}

Symbol Add_class::checkType()
{
    //cout << "ADD\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float))
    {
        setType(Float);
        return type;
    }
    // Int and Int
    else if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot add a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Minus_class::checkType()
{
    //cout << "MINUS\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float))
    {
        setType(Float);
        return type;
    }
    // Int and Int
    else if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot minus a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Multi_class::checkType()
{
    //cout << "MULTI\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float))
    {
        setType(Float);
        return type;
    }
    // Int and Int
    else if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot multi a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Divide_class::checkType()
{
    //cout << "DIVIDE\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float))
    {
        setType(Float);
        return type;
    }
    // Int and Int
    else if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot div a " << getLeft()->getType()
                           << " by a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Mod_class::checkType()
{
    //cout << "MOD\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Int
    if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot mod a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Neg_class::checkType()
{
    //cout << "NEG\n";
    getRight()->check(Int);
    if (getRight()->getType() == Int || getRight()->getType() == Float)
    {
        setType(getRight()->getType());
        return type;
    }
    else
    {
        semant_error(this) << "A " << getRight()->getType()
                           << " doesn't have a negative.\n";
        setType(Void);
        return type;
    }
}

Symbol Lt_class::checkType()
{
    //cout << "LT\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Le_class::checkType()
{
    //cout << "LE\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Equ_class::checkType()
{
    //cout << "EQU\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int, or Bool and Bool
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int) ||
        (getLeft()->getType() == Bool && getRight()->getType() == Bool))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Neq_class::checkType()
{
    //cout << "NEQ\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int, or Bool and Bool
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int) ||
        (getLeft()->getType() == Bool && getRight()->getType() == Bool))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Ge_class::checkType()
{
    //cout << "GE\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Gt_class::checkType()
{
    //cout << "GT\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Float, or Float and Float, or Int and Int
    if ((getLeft()->getType() == Int && getRight()->getType() == Float) ||
        (getLeft()->getType() == Float && getRight()->getType() == Int) ||
        (getLeft()->getType() == Float && getRight()->getType() == Float) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int))
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot compare a " << getLeft()->getType()
                           << " and a " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol And_class::checkType()
{
    //cout << "AND\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Bool and Bool
    if (getLeft()->getType() == Bool && getRight()->getType() == Bool)
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use && between " << getLeft()->getType()
                           << " and " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Or_class::checkType()
{
    //cout << "OR\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Bool and Bool
    if (getLeft()->getType() == Bool && getRight()->getType() == Bool)
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use || between " << getLeft()->getType()
                           << " and " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Xor_class::checkType()
{
    //cout << "XOR\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Bool and Bool, or Int and Int
    if ((getLeft()->getType() == Bool && getRight()->getType() == Bool) ||
        (getLeft()->getType() == Int && getRight()->getType() == Int))
    {
        setType(getLeft()->getType());
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use ^ between " << getLeft()->getType()
                           << " and " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Not_class::checkType()
{
    //cout << "NOT\n";
    getRight()->check(Int);
    // Bool and Bool
    if (getRight()->getType() == Bool)
    {
        setType(Bool);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use ! upon " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Bitand_class::checkType()
{
    //cout << "BITAND\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Int
    if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use & between " << getLeft()->getType()
                           << " and " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Bitor_class::checkType()
{
    //cout << "BITOR\n";
    getLeft()->check(Int);
    getRight()->check(Int);
    // Int and Int
    if (getLeft()->getType() == Int && getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use | between " << getLeft()->getType()
                           << " and " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Bitnot_class::checkType()
{
    //cout << "BITNOT\n";
    getRight()->check(Int);
    // Int and Int
    if (getRight()->getType() == Int)
    {
        setType(Int);
        return type;
    }
    else
    {
        semant_error(this) << "Cannot use unary op ~ upon " << getRight()->getType() << ".\n";
        setType(Void);
        return type;
    }
}

Symbol Const_int_class::checkType()
{
    setType(Int);
    return type;
}

Symbol Const_string_class::checkType()
{
    setType(String);
    return type;
}

Symbol Const_float_class::checkType()
{
    setType(Float);
    return type;
}

Symbol Const_bool_class::checkType()
{
    setType(Bool);
    return type;
}

Symbol Object_class::checkType()
{
    Symbol Obt;
    //cout << "OBJECTID\n";
    if (localVar_table->lookup(getValue()) == NULL && formalPara_table->probe(getValue()) == NULL && var_table->lookup(getValue()) == NULL)
    {
        semant_error(this) << "object " << getValue() << " has not been defined.\n";
        setType(Void);
        return type;
    }
    else if (localVar_table->lookup(getValue()) != NULL)
        Obt = *localVar_table->lookup(getValue());

    else if (formalPara_table->probe(getValue()) != NULL)
        Obt = *formalPara_table->probe(getValue());

    else if (var_table->lookup(getValue()) != NULL)
        Obt = *var_table->lookup(getValue());
    setType(Obt);
    //cout << getValue() << ' ' <<Obt<<'\n';
    return type;
}

Symbol No_expr_class::checkType()
{
    setType(Void);
    return getType();
}

void Program_class::semant()
{
    initialize_constants();
    install_globalVars(decls);
    install_calls(decls);
    check_main();
    check_calls(decls);

    if (semant_errors > 0)
    {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}
