
//**************************************************************
//
// Code generator SKELETON
//
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

using namespace std;

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7};
static char *CALLEE_REGS[] = {RBP, RBX, R12, R13, R14, R15};

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

int pos_index;
int stmtBlock_level;
int rsp_offset;

struct Jump_Stmt {
  int pos_goto;
  int level;
};

struct AddAndType {
  char* add;
  bool isFloat;
  bool inRegister;
};

SymbolTable<int, Jump_Stmt> *break_table;
SymbolTable<int, Jump_Stmt> *continue_table;
SymbolTable<Symbol ,AddAndType> *object_table;

//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_mov(const char *source, const char *dest, ostream& s)
{
  s << MOV << source << COMMA << dest << endl;
}

static void emit_add(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADD << source_reg << COMMA << dest_reg << endl;
}

static void emit_sub(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUB << source_reg << COMMA << dest_reg << endl;
}

static void emit_mul(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MUL << source_reg << COMMA << dest_reg << endl;
}

static void emit_div(const char *dest_reg, ostream& s)
{
  s << DIV << dest_reg << endl;
}

static void emit_neg(const char *dest_reg, ostream& s)
{
  s << NEG << dest_reg << endl;
}

static void emit_and(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << AND << source_reg << COMMA << dest_reg << endl;
}

static void emit_or(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << OR << source_reg << COMMA << dest_reg << endl;
}

static void emit_xor(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XOR << source_reg << COMMA << dest_reg << endl;
}

static void emit_not(const char *dest_reg, ostream& s)
{
  s << NOT << " " << dest_reg << endl;
}

static void emit_movsd(const char *source, const char *dest, ostream& s)
{
  s << MOVSD << source << COMMA << dest << endl;
}

static void emit_addsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADDSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_subsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUBSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_mulsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MULSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_divsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << DIVSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_cmp(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << CMP << source_reg << COMMA << dest_reg << endl;
}

static void emit_test(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << TEST << source_reg << COMMA << dest_reg << endl;
}

static void emit_ucompisd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << UCOMPISD << source_reg << COMMA << dest_reg << endl;
}

static void emit_xorpd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XORPD << source_reg << COMMA << dest_reg << endl;
}
static void emit_jmp(const char *dest, ostream& s)
{
  s << JMP << " " << dest << endl;
}

static void emit_jl(const char *dest, ostream& s)
{
  s << JL << " " << dest << endl;
}

static void emit_jle(const char *dest, ostream& s)
{
  s << JLE << " " << dest << endl;
}

static void emit_je(const char *dest, ostream& s)
{
  s << JE << " " << dest << endl;
}

static void emit_jne(const char *dest, ostream& s)
{
  s << JNE << " " << dest << endl;
}

static void emit_jg(const char *dest, ostream& s)
{
  s << JG << " " << dest << endl;
}

static void emit_jge(const char *dest, ostream& s)
{
  s << JGE << " " << dest << endl;
}

static void emit_jb(const char *dest, ostream& s)
{
  s << JB << " " << dest << endl;
}

static void emit_jbe(const char *dest, ostream& s)
{
  s << JBE << " " << dest << endl;
}

static void emit_ja(const char *dest, ostream& s)
{
  s << JA << " " << dest << endl;
}

static void emit_jae(const char *dest, ostream& s)
{
  s << JAE << " " << dest << endl;
}

static void emit_jp(const char *dest, ostream& s)
{
  s << JP << " " << dest << endl;
}

static void emit_jz(const char *dest, ostream& s)
{
  s << JZ << " " << dest << endl;
}

static void emit_jnz(const char *dest, ostream& s)
{
  s << JNZ << " " << dest << endl;
}

static void emit_call(const char *dest, ostream& s)
{
  s << CALL << " " << dest << endl;
}

static void emit_ret(ostream& s)
{
  s << RET << endl;
}

static void emit_push(const char *reg, ostream& s)
{
  s << PUSH << " " << reg << endl;
}

static void emit_pop(const char *reg, ostream& s)
{
  s << POP << " " << reg << endl;
}

static void emit_leave(ostream& s)
{
  s << LEAVE << endl;
}

static void emit_float_to_int(const char *float_mmx, const char *int_reg, ostream& s)
{
  s << CVTTSD2SIQ << float_mmx << COMMA << int_reg << endl;
}

static void emit_int_to_float(const char *int_reg, const char *float_mmx, ostream& s)
{
  s << CVTSI2SDQ << int_reg << COMMA << float_mmx << endl;
}

static void emit_sete(const char* reg, ostream &s) {
  s << "\tsete\t" << reg << endl; 
}

static void emit_setne(const char * reg, ostream &s) {
  s << "\tsetne\t" << reg <<endl;
}

static void emit_setg(const char*reg, ostream &s) {
  s << "\tsetg\t" << reg << endl;
}

static void emit_setge(const char * reg, ostream &s) {
  s << "\tsetge\t" << reg <<endl;
}

static void emit_setl(const char * reg, ostream &s) {
  s << "\tsetl\t" << reg <<endl;
}

static void emit_setle(const char * reg, ostream &s) {
  s << "\tsetle\t" << reg <<endl;
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
Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print
    ;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    //idtable
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    Main        = idtable.add_string("main");
    print        = idtable.add_string("printf");

    //other constants
    pos_index=0;
    stmtBlock_level=0;
    rsp_offset=0;

    //other tables
    break_table = new SymbolTable<int,Jump_Stmt>();
    continue_table = new SymbolTable<int,Jump_Stmt>();
    object_table = new SymbolTable<Symbol,AddAndType>();
}


//*********************************************************
//
// Define method for code generation
//
//
//*********************************************************


void Program_class::cgen(ostream &os) 
{

  cgen_debug=0;
  // cgen_debug=1;

  // spim wants comments to start with '#'
  os << "# start of generated code\n";
  initialize_constants();
  cgen_helper(decls,os);
  os << "\t.section\t.rodata"<<endl;
  stringtable.code_string_table(os);
  os << "\n# end of generated code\n";
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Seal has four kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// If you like, you can add any ***Entry::code_def() and ***Entry::code_ref()
// functions to help.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << "$" << STRINGCONST_PREFIX << index;
}

//
// Emit code for a constant String.
//

void StringEntry::code_def(ostream& s)
{
  s << STRINGCONST_PREFIX << index << ":" << endl;
  s  << STRINGTAG ; emit_string_constant(s,str);                                                // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s);
}

//***************************************************
//
//  Emit global var and functions.
//
//***************************************************

static void emit_global_int(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  INTTAG << 0 << endl;
}

static void emit_global_float(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl <<
  FLOATTAG << 0 << endl <<
  FLOATTAG << 0 << endl;
}

static void emit_global_bool(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  BOOLTAG << 0 << endl;
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

//********************************************************
//
// Cgen helper helps to initialize and call code() function.
// You can do any initializing operations here
//
//********************************************************

void cgen_helper(Decls decls, ostream& s)
{
  code(decls, s);
}


void code(Decls decls, ostream& s)
{
  s << TEXT << endl;
  for(int i = decls->first(); decls->more(i); i = decls->next(i)) {
    decls->nth(i)->code(s);
  }
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `seal-decl.h', `seal-expr.h' and `seal-stmt.h'
//   Sample code for constant integers, strings, and booleans are provided.
//   
//*****************************************************************

char* generateParam(int n, char* str) {
    int size = 0;
    char *retStr;
    while(str[size]!='\0') {
        size++;
    }
    if(n==0) {
        retStr = new char[size+2];
        retStr[0]='(';
        for(size = 0;str[size]!='\0';size++){
            retStr[size+1]=str[size];
        }
        retStr[size+1]=')';
    }
    if(n<0) {
        n=-n;
        int k = n;
        size++;
        int num=0;
        while(n!=0) {
            n/=10;
            num++;
        }
        size+=num;
        retStr = new char[size+2];
        retStr[0]='-';
        int j = num;
        while(k!=0) {
            retStr[num]=(k%10)+48;
            k/=10;
            num--;
        }
        j++;
        retStr[j]='(';
        for(size = 0;str[size]!='\0';size++){
            retStr[size+1+j]=str[size];
        }
        retStr[size+1+j]=')';
    }
    if(n>0) {
        int k = n;
        int num=0;
        while(n!=0) {
            n/=10;
            num++;
        }
        size+=num;
        retStr = new char[size+2];
        int j = num;
        num--;
        while(k!=0) {
            retStr[num]=(k%10)+48;
            k/=10;
            num--;
        }
        retStr[j]='(';
        for(size = 0;str[size]!='\0';size++){
            retStr[size+1+j]=str[size];
        }
        retStr[size+1+j]=')';
    }
    return retStr;
}


void CallDecl_class::code(ostream &s) {
  object_table->enterscope();
  
  s << name->get_string() << ":" << endl;
  s << GLOBAL << name->get_string() << endl;
  s << SYMBOL_TYPE <<name->get_string() << COMMA << FUNCTION <<endl;
  
  emit_push(RBP, s);
  emit_mov(RSP, RBP, s);
  for(int i=1;i<6;++i)
    emit_push(CALLEE_REGS[i], s);
  
  rsp_offset=-40;

  int number_i = 0;
  int number_f = 0;
  int paraNumInStack = 0;

  for(int i = paras->first(),j=0,k=0;paras->more(i); i = paras->next(i)) {
    if(paras->nth(i)->isFloat()) {
      if(k<8) k++;
      else paraNumInStack++;
    } 
    else {
      if(j<6) j++;
      else paraNumInStack++;
    }
  }

  for(int i = paras->first(),index=1; paras->more(i); i = paras->next(i)) {
    /*
    struct AddAndType {
    char* add;
    bool isFloat;
    bool inRegister;
    };
    */

    AddAndType *object = new AddAndType();
    if(paras->nth(i)->isFloat()) {
      object->isFloat=true;
      if(number_f<8){
        object->add=CALL_XMM[number_f];
        object->inRegister=1;
        object_table->addid(paras->nth(i)->getName(),object);
        number_f++;
      }
      else{
        object->inRegister=0;
        object->add=generateParam(16+(paraNumInStack-index)*8,RBP);
        object_table->addid(paras->nth(i)->getName(),object);
        index++;
      }
    } 
    else {
      object->isFloat=false;
      if (number_i<6){
        object->add=CALL_REGS[number_i];
        object->inRegister=1;
        object_table->addid(paras->nth(i)->getName(),object);
        number_i++;
      }
      else{
        object->inRegister=0;
        object->add=generateParam(16+(paraNumInStack-index)*8,RBP);
        object_table->addid(paras->nth(i)->getName(),object);
        index++;
      }
    }
  }
  stmtBlock_level=0;
  body->code(s);
  s << SIZE << name->get_string()<<", .-"<< name->get_string()<<endl;
  object_table->exitscope();
}

void StmtBlock_class::code(ostream &s) {
  if (cgen_debug) s << "-----StmtBlock-----" <<endl;

  if(break_table->lookup(1)) 
    break_table->lookup(1)->level++;
  if(continue_table->lookup(1)!=NULL)
    continue_table->lookup(1)->level++;

  object_table->enterscope();
  stmtBlock_level++;

  emit_xor(R15,R15,s);

  flag = 0;
  for(int i = vars->first(); vars->more(i); i = vars->next(i)) {
    vars->nth(i)->code(s);
    flag = !flag;
    emit_add("$8",R15,s);
  }
  
  emit_push(R15,s);
  rsp_offset-=8;
  for(int i = stmts->first(); stmts->more(i); i = stmts->next(i))
    stmts->nth(i)->code(s);

  emit_pop(R15,s);
  emit_add(R15,RSP,s);

  stmtBlock_level--;
  if(break_table->lookup(1)!=NULL) {
    break_table->lookup(1)->level--;
  }
  if(continue_table->lookup(1)!=NULL) {
    continue_table->lookup(1)->level--;
  }
  object_table->exitscope();

  if (cgen_debug) s << "-----/StmtBlock-----" <<endl;
}

void VariableDecl_class::code(ostream &s) {
  if (cgen_debug) s << "-----VariableDecl-----" <<endl;

  AddAndType* object = new AddAndType();
  emit_sub("$8", RSP, s);
  rsp_offset-=8;
  object->inRegister=0;
  object->add = generateParam(rsp_offset,RBP);
  object->isFloat = getType()->isFloat();

  if(cgen_debug) s << "<add var \t"<<variable->getName() << "\t"<< object->add << ">" << endl;
  
  object_table->addid(variable->getName(),object);

  if (cgen_debug) s<<"<the location of "<<variable->getName()<<" is "<<object_table->lookup(variable->getName())->add << ">"<<endl;
  
  if (cgen_debug) s << "-----/VariableDecl-----" <<endl;
}

void IfStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------if----------"<<endl;

  condition->code(s);

  emit_test(RAX,RAX,s);
  int pos_else = pos_index;
  pos_index++;
  s << JE << "    " << POSITION << to_string(pos_else) << endl;

  thenexpr->code(s);
  int pos_next = pos_index;
  pos_index++;
  s << JMP << "   " << POSITION << to_string(pos_next) << endl;

  s << POSITION << to_string(pos_else) <<":" <<endl;
  elseexpr->code(s);

  s << POSITION << to_string(pos_next) <<":" <<endl;

  if (cgen_debug) s<<"-----------/if----------"<<endl;
}

void WhileStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------while----------"<<endl;

  break_table->enterscope();

  int pos_condition = pos_index;
  pos_index++;
  s << POSITION << to_string(pos_condition) <<":" <<endl;
  if(cgen_debug) s<<"<while-condition>"<<endl;
  condition->code(s);
  if(cgen_debug) s<<"</while-condition>"<<endl;

  emit_test(RAX,RAX,s);
  int pos_next = pos_index;
  pos_index++;
  Jump_Stmt *p = new Jump_Stmt();
  p->pos_goto=pos_next;
  p->level=0;
  break_table->addid(1,p);
  s << JE << "   " << POSITION << to_string(pos_next) << endl;

  if(cgen_debug) s<<"<while-body>"<<endl;
  body->code(s);
  if(cgen_debug) s<<"</while-body>"<<endl;
  s << JMP << "   " << POSITION << to_string(pos_condition) << endl;

  s << POSITION << to_string(pos_next) <<":" <<endl;

  break_table->exitscope();

  if (cgen_debug) s<<"-----------/while----------"<<endl;
}

void ForStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------for----------"<<endl;

  if(cgen_debug) s<<"<for-initexpr>"<<endl;
  initexpr->code(s);
  if(cgen_debug) s<<"</for-initexpr>"<<endl;

  break_table->enterscope();
  continue_table->enterscope();

  int pos_condition = pos_index;
  pos_index++;
  s << POSITION << to_string(pos_condition) <<":" <<endl;
  if(cgen_debug) s<<"<for-condition>"<<endl;
  condition->code(s);
  if(cgen_debug) s<<"</for-condition>"<<endl;

  emit_test(RAX,RAX,s);
  int pos_next = pos_index;
  Jump_Stmt *p = new Jump_Stmt();
  p->pos_goto=pos_next;
  p->level=0;
  break_table->addid(1,p);
  pos_index++;
  s << JE << "   " << POSITION << to_string(pos_next) << endl;

  int pos_loopact = pos_index;
  pos_index++;
  Jump_Stmt *q = new Jump_Stmt();
  q->pos_goto=pos_loopact;
  q->level=0;
  continue_table->addid(1,q);
  if(cgen_debug) s<<"<for-body>"<<endl;
  body->code(s);
  if(cgen_debug) s<<"</for-body>"<<endl;

  s << POSITION << to_string(pos_loopact) <<":" <<endl;
  loopact->code(s);
  s << JMP << "   " << POSITION << to_string(pos_condition) << endl;

  s << POSITION << to_string(pos_next)  <<":" <<endl;

  break_table->exitscope();
  continue_table->exitscope();

  if (cgen_debug) s<<"-----------/for----------"<<endl;
}

void ReturnStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------return----------"<<endl;

  value->code(s);
  if(value->isFloat()) {
    emit_xorpd(XMM0, XMM0, s);
    emit_mov(RAX, XMM0, s);
  }

  int k = stmtBlock_level;
  while(k) {
    emit_pop(R15,s);
    emit_add(R15,RSP,s);
    k--;
  }

  for (int i=5;i>=0;--i) emit_pop(CALLEE_REGS[i], s);
  emit_ret(s);

  if (cgen_debug) s<<"-----------/return----------"<<endl;
}

void ContinueStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------continue----------"<<endl;

  int k = continue_table->lookup(1)->level;
  while(k!=0) {
    emit_pop(R15,s);
    emit_add(R15,RSP,s);
    k--;
  }
  s << JMP << "  " << POSITION << to_string(continue_table->lookup(1)->pos_goto) << endl;

  if (cgen_debug) s<<"-----------/continue----------"<<endl;
}

void BreakStmt_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------break----------"<<endl;

  int k = break_table->lookup(1)->level;
  while(k!=0) {
    emit_pop(R15,s);
    emit_add(R15,RSP,s);
    k--;
  }
  s << JMP << "  " << POSITION << to_string(break_table->lookup(1)->pos_goto) << endl;

  if (cgen_debug) s<<"-----------/break----------"<<endl;
}

void Call_class::code(ostream &s) {
  if (cgen_debug) s<<"-----------call----------"<<endl;

  for(int i=0;i<6;++i) emit_push(CALL_REGS[i],s);

  for(int i=0;i<8;++i){
    emit_sub("$8",RSP,s);
    emit_movsd(CALL_XMM[i],generateParam(0,RSP),s);
  }

  int fNum=0;
  int notFNum=0;

  for(int i = actuals->first(); actuals->more(i); i = actuals->next(i)) {
    actuals->nth(i)->code(s);

    if(actuals->nth(i)->getType()==Float) {
      if (fNum<8){
        emit_mov(XMM8,CALL_XMM[fNum],s);
        fNum++;
      }
      else{
        emit_sub("$8",RSP,s);
        emit_mov(XMM8,generateParam(0,RSP),s);
        fNum++;
      }
    } 
    else {
      if(notFNum<6){
        emit_mov(RAX,CALL_REGS[notFNum],s);
        notFNum++;
      }
      else{
        emit_sub("$8",RSP,s);
        emit_mov(RAX,generateParam(0,RSP),s);
        notFNum++;
      }
    }
  }

  if(name==print){
    if(flag) emit_sub("$8",RSP,s);
    s << MOVL << "$" << to_string(fNum) << COMMA << EAX << endl;
  }

  emit_call(name->get_string(),s);

  if(name==print){
    if(flag) emit_add("$8",RSP,s);
  }
  
  for(int i=7;i>=0;--i){
    emit_movsd(generateParam(0,RSP),CALL_XMM[i],s);
    emit_add("$8",RSP,s);
  }
  for(int i=5;i>=0;--i) emit_pop(CALL_REGS[i],s);

  if (cgen_debug) s<<"-----------/call----------"<<endl;
}

void Actual_class::code(ostream &s) {
  if (cgen_debug) s<<"<actual>"<<endl;

  expr->code(s);
  
  if (cgen_debug) s<<"</actual>"<<endl;
}

void Assign_class::code(ostream &s) {
  if (cgen_debug) s<<"<assign>"<<endl;

  value->code(s);

  if(object_table->lookup(lvalue)->isFloat) {
    if(value->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    if(object_table->lookup(lvalue)->inRegister) 
      emit_mov(XMM8, object_table->lookup(lvalue)->add,s);
    else 
      emit_movsd(XMM8, object_table->lookup(lvalue)->add,s);
  } 
  else emit_mov(RAX,object_table->lookup(lvalue)->add,s);

  if (cgen_debug) s<<"</assign>"<<endl;
}

void Add_class::code(ostream &s) {
  if (cgen_debug) s<<"<add>"<<endl;

  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_addsd(XMM9, XMM8, s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX, s);
    emit_add(RBX, RAX, s);
  }

  if (cgen_debug) s<<"</add>"<<endl;
}

void Minus_class::code(ostream &s) {
  if (cgen_debug) s<<"<minus>"<<endl;
  
  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_subsd(XMM9, XMM8, s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX, s);
    emit_sub(RBX, RAX, s);
  }

  if (cgen_debug) s<<"</minus>"<<endl;
}

void Multi_class::code(ostream &s) {
  if (cgen_debug) s<<"<mul>"<<endl;
  
  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_mulsd(XMM9, XMM8, s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX, s);
    emit_mul(RBX, RAX, s);
  }

  if (cgen_debug) s<<"</mul>"<<endl;
}

void Divide_class::code(ostream &s) {
  if (cgen_debug) s<<"<div>"<<endl;
  
  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_divsd(XMM9, XMM8, s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX, s);
    emit_div(RBX,s);
  }

  if (cgen_debug) s<<"</div>"<<endl;

}

void Mod_class::code(ostream &s) {
  if (cgen_debug) s<<"<mod>"<<endl;

  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_xor(RDX,RDX,s);
  emit_pop(R13,s);
  emit_div(R13,s);
  emit_mov(RDX,RAX,s);

  if (cgen_debug) s<<"</mod>"<<endl;

}

void Neg_class::code(ostream &s) {
  if (cgen_debug) s<<"<neg>"<<endl;

  if(e1->getType()==Float) {
    e1->code(s);
    emit_mov(XMM8,XMM9,s);
    emit_mov("$0",RAX,s);
    emit_int_to_float(RAX,XMM8,s);
    emit_subsd(XMM9,XMM8,s);
  } 
  else {
    e1->code(s);
    emit_not(RAX,s);
    emit_add("$1",RAX,s);
  }

  if (cgen_debug) s<<"</neg>"<<endl;
}

// e1 < e2
void Lt_class::code(ostream &s) {
  if (cgen_debug) s<<"<lt>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_setg("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_setg("%r13b",s);
    emit_mov(R13,RAX,s);
  }
  if (cgen_debug) s<<"</lt>"<<endl;

}

// e1 <= e2
void Le_class::code(ostream &s) {
  if (cgen_debug) s<<"<le>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_setge("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_setge("%r13b",s);
    emit_mov(R13,RAX,s);
  }

  if (cgen_debug) s<<"</le>"<<endl;
}

// e1 == e2
void Equ_class::code(ostream &s) {
  if (cgen_debug) s<<"<eq>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_sete("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_sete("%r13b",s);
    emit_mov(R13,RAX,s);
  }

  if (cgen_debug) s<<"</eq>"<<endl;
}

// e1!=e2
void Neq_class::code(ostream &s) {
  if (cgen_debug) s<<"<neq>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_setne("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_setne("%r13b",s);
    emit_mov(R13,RAX,s);
  }

  if (cgen_debug) s<<"</neq>"<<endl;
}

// e1 >= e2
void Ge_class::code(ostream &s) {
  if (cgen_debug) s<<"<ge>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_setle("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_setle("%r13b",s);
    emit_mov(R13,RAX,s);
  }

  if (cgen_debug) s<<"</ge>"<<endl;
}

// e1 > e2
void Gt_class::code(ostream &s) {
  if (cgen_debug) s<<"<gt>"<<endl;

  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8,s);
    }
    emit_mov(XMM8,R12,s);
    emit_push(R12,s);

    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd(XMM8, XMM8, s);
      emit_int_to_float(RAX, XMM8, s);
    }
    emit_pop(R13, s);
    emit_xorpd(XMM9, XMM9, s);
    emit_mov(R13,XMM9,s);
    emit_xor(R13,R13,s);
    emit_ucompisd(XMM8,XMM9,s);
    emit_setl("%r13b",s);
    emit_mov(R13,RAX,s);
  } 
  else {
    e2->code(s);
    emit_push(RAX, s);
    e1->code(s);
    emit_pop(RBX,s);
    emit_xor(R13,R13,s);
    emit_cmp(RAX,RBX,s);
    emit_setl("%r13b",s);
    emit_mov(R13,RAX,s);
  }

  if (cgen_debug) s<<"</gt>"<<endl;

}

void And_class::code(ostream &s) {
  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_pop(RBX, s);
  emit_and(RBX,RAX, s);
}

void Or_class::code(ostream &s) {
  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_pop(RBX, s);
  emit_or(RBX, RAX, s);
}

void Xor_class::code(ostream &s) {
  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_pop(RBX, s);
  emit_xor(RBX,RAX, s);
}

void Not_class::code(ostream &s) {
  e1->code(s);
  emit_not(RAX,s);
}

void Bitnot_class::code(ostream &s) {
  e1->code(s);
  emit_not(RAX,s);
}

void Bitand_class::code(ostream &s) {
  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_pop(RBX, s);
  emit_and(RBX,RAX, s);
}

void Bitor_class::code(ostream &s) {
  e2->code(s);
  emit_push(RAX, s);
  e1->code(s);
  emit_pop(RBX, s);
  emit_or(RBX,RAX, s);
}


void Const_int_class::code(ostream &s) {
  if (cgen_debug) s<<"<int const>"<<endl;

  s<< MOV << "$" << value->get_string()<< COMMA <<RAX<<endl;

  if (cgen_debug) s<<"</int const>"<<endl;
}

void Const_string_class::code(ostream &s) {
  if (cgen_debug) s<<"<string const>"<<endl;

  s<< MOV;
  stringtable.lookup_string(value->get_string())->code_ref(s);
  s<< COMMA << RAX << endl;

  if (cgen_debug) s<<"</string const>"<<endl;

}


static char* getImmediateFloat(char* str) {
  double x = stof(str);
  long long unsigned k = *(long*)(&x);
  char*buf = new char(19);
  buf[0]='$';
  sprintf(buf+1, "0x%llx", k);
  return buf;
}

void Const_float_class::code(ostream &s) {
  if (cgen_debug) s<<"<float const>"<<endl;

  emit_mov(getImmediateFloat(value->get_string()),RAX,s);
  emit_xorpd(XMM8, XMM8, s);
  emit_mov(RAX,XMM8,s);

  if (cgen_debug) s<<"</float const>"<<endl;
}

void Const_bool_class::code(ostream &s) {
  if (cgen_debug) s<<"<bool const>"<<endl;

  if(value) emit_mov("$1",RAX,s);
  else emit_xor(RAX,RAX,s); 

  if (cgen_debug) s<<"</bool const>"<<endl;
}

void Object_class::code(ostream &s) {
  if (cgen_debug) s<<"<object>"<<endl;

  if(getType()==Float) {
    if(object_table->lookup(var)->inRegister) {
      emit_movsd(object_table->lookup(var)->add,generateParam(-8,"%rsp"),s);
      emit_mov(generateParam(-8,"%rsp"),"%xmm8",s);
    } else {
      emit_mov(object_table->lookup(var)->add,"%r12",s);
      emit_mov("%r12",generateParam(-8,"%rsp"),s);
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_mov(generateParam(-8,"%rsp"),"%xmm8",s);
    }
  } 
  else emit_mov(object_table->lookup(var)->add,RAX,s);
  
  if (cgen_debug) s<<"</object>"<<endl;
}

void No_expr_class::code(ostream& s) {
  if (cgen_debug) s<<"<no_expr>"<<endl;

  emit_mov("$1",RAX,s);

  if (cgen_debug) s<<"</no_expr>"<<endl;
}