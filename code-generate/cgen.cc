
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
extern SymbolTable<Symbol ,char> *map;

static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7};

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

//////////////////////////////////////////////////////////////////
//
//
//    Helper Functions
//  
//
//////////////////////////////////////////////////////////////////

// you can add any helper functions here

static char* generateJumpFlag(int n) {
  int num;
  int k = n;
  if(n==0) {
    num =1;
  } else {
    num=0;
    while (n!=0) {
      n/=10;
      num++;
    }
  }
  char *buf = new char(4+num);
  buf[0]='.';
  buf[1]='P';
  buf[2]='O';
  buf[3]='S';
  for(int i=num+3;i>=4;i--) {
    buf[i]=(k%10)+48;
    k/=10;
  }
  return buf;
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

static void emit_irmov(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << immidiate << COMMA << dest_reg  
      << endl;
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

static void emit_movaps(const char *source, const char *dest, ostream& s)
{
  s << MOVAPS << source << COMMA << dest << endl;
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

static void emit_position(const char *p, ostream& s)
{
  s << p << ":" << endl;
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

// the following 2 functions are useless, please DO NOT care about them
// void FloatEntry::code_ref(ostream &s)
// {
//   s << FLOATTAG << index;
// }

// void IntEntry::code_def(ostream &s)
// {
//   s << GLOBAL;
// }

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

void code_global_data(Decls decls, ostream &str)
{
  // decls->dump(str, 0);
}

void code_calls(Decls decls, ostream &str) {
  // for(int i = decls->first(); decls->more(i); i = decls->next(i)) {
  //   decls->nth(i)->code(str);
  // }
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

  // if (cgen_debug) s << "Coding calls" << endl;
  // code_calls(decls, s);

  // if (cgen_debug) s << "Coding global data" << endl;
  // code_global_data(decls, s);
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
  map->enterscope();
  
  s << name->get_string() << ":" << endl;
  s << GLOBAL << name->get_string() << endl;
  s << SYMBOL_TYPE <<name->get_string() << COMMA << "@function" <<endl;
  
  emit_push(RBP, s);
  emit_mov(RSP, RBP, s);
  emit_push(RBX, s);
  emit_push(R12, s);
  emit_push(R13, s);
  emit_push(R14, s);
  emit_push(R15, s);
  rspAdd=-40;

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
        map->addid(paras->nth(i)->getName(),object);
        number_f++;
      }
      else{
        object->inRegister=0;
        object->add=generateParam(16+(paraNumInStack-index)*8,RBP);
        map->addid(paras->nth(i)->getName(),object);
        index++;
      }
    } 
    else {
      object->isFloat=false;
      if (number_i<6){
        object->add=CALL_REGS[number_i];
        object->inRegister=1;
        map->addid(paras->nth(i)->getName(),object);
        number_i++;
      }
      else{
        object->inRegister=0;
        object->add=generateParam(16+(paraNumInStack-index)*8,RBP);
        map->addid(paras->nth(i)->getName(),object);
        index++;
      }
    }
  }
  stmtIndex=0;
  body->code(s);
  s << SIZE << name->get_string()<<", .-"<< name->get_string()<<endl;
  map->exitscope();
}

void StmtBlock_class::code(ostream &s) {

  if (cgen_debug)
    s << "-----StmtBlock-----" <<endl;

  if(breakPoint->lookup(1)!=NULL) {
    breakPoint->lookup(1)->stmtNum++;
  }
  if(continuePoint->lookup(1)!=NULL) {
    continuePoint->lookup(1)->stmtNum++;
  }
  map->enterscope();
  stmtIndex++;

  emit_xor(R15,R15,s);

  flag = 0;
  for(int i = vars->first(); vars->more(i); i = vars->next(i)) {
    vars->nth(i)->code(s);
    flag = !flag;
    emit_add("$8",R15,s);
  }
  
  emit_push(R15,s);
  rspAdd-=8;
  for(int i = stmts->first(); stmts->more(i); i = stmts->next(i))
    stmts->nth(i)->code(s);

  emit_pop(R15,s);
  emit_add(R15,RSP,s);

  stmtIndex--;
  if(breakPoint->lookup(1)!=NULL) {
    breakPoint->lookup(1)->stmtNum--;
  }
  if(continuePoint->lookup(1)!=NULL) {
    continuePoint->lookup(1)->stmtNum--;
  }
  map->exitscope();

  if (cgen_debug)
    s << "-----/StmtBlock-----" <<endl;

}

void VariableDecl_class::code(ostream &s) {
  if (cgen_debug)
    s << "-----VariableDecl-----" <<endl;

  AddAndType* object = new AddAndType();
  emit_sub("$8", RSP, s);
  rspAdd-=8;
  object->inRegister=0;
  object->add = generateParam(rspAdd,RBP);
  object->isFloat = getType()->isFloat();

  if(cgen_debug) s << "<add var \t"<<variable->getName() << "\t"<< object->add << ">" << endl;
  
  map->addid(variable->getName(),object);

  if (cgen_debug) s<<"<the location of "<<variable->getName()<<" is "<<map->lookup(variable->getName())->add << ">"<<endl;
  
  if (cgen_debug)
    s << "-----/VariableDecl-----" <<endl;

}

void IfStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------if----------"<<endl;
  condition->code(s);
  emit_test(RAX,RAX,s);
  int n = jumpNum;
  jumpNum++;
  emit_je(generateJumpFlag(n),s);
  thenexpr->code(s);
  int k = jumpNum;
  jumpNum++;
  emit_jmp(generateJumpFlag(k),s);
  s << generateJumpFlag(n) <<":" <<endl;
  elseexpr->code(s);
  s << generateJumpFlag(k) <<":" <<endl;

  if (cgen_debug)
    s<<"-----------/if----------"<<endl;
}

void WhileStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------while----------"<<endl;

  breakPoint->enterscope();
  int n = jumpNum;
  jumpNum++;
  s << generateJumpFlag(n) <<":" <<endl;
  condition->code(s);
  emit_test("%rax","%rax",s);
  int k = jumpNum;
  jumpNum++;
  Help *object = new Help();
  object->flag=k;
  object->stmtNum=0;
  breakPoint->addid(1,object);
  emit_je(generateJumpFlag(k),s);
  body->code(s);
  emit_jmp(generateJumpFlag(n),s);
  s << generateJumpFlag(k) <<":" <<endl;
  breakPoint->exitscope();

  if (cgen_debug)
    s<<"-----------/while----------"<<endl;
}

void ForStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------for----------"<<endl;

  if(cgen_debug) s<<"<for-initexpr>"<<endl;
  initexpr->code(s);
  if(cgen_debug) s<<"</for-initexpr>"<<endl;

  breakPoint->enterscope();
  continuePoint->enterscope();
  int n = jumpNum;
  jumpNum++;
  s << generateJumpFlag(n) <<":" <<endl;

  if(cgen_debug) s<<"<for-condition>"<<endl;
  condition->code(s);
  if(cgen_debug) s<<"</for-condition>"<<endl;

  emit_test("%rax","%rax",s);
  int k = jumpNum;
  Help *breakObject = new Help();
  breakObject->flag=k;
  breakObject->stmtNum=0;
  breakPoint->addid(1,breakObject);
  jumpNum++;
  emit_je(generateJumpFlag(k),s);
  int j = jumpNum;
  jumpNum++;
  Help *continueObject = new Help();
  continueObject->flag=j;
  continueObject->stmtNum=0;
  continuePoint->addid(1,continueObject);
  body->code(s);
  s << generateJumpFlag(j) <<":" <<endl;
  loopact->code(s);
  emit_jmp(generateJumpFlag(n),s);
  s << generateJumpFlag(k) <<":" <<endl;
  breakPoint->exitscope();
  continuePoint->exitscope();

  if (cgen_debug)
    s<<"-----------/for----------"<<endl;

}

void ReturnStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------return----------"<<endl;

  value->code(s);
  if(value->isFloat()) {
    emit_xorpd("%xmm0", "%xmm0", s);
    emit_mov("%rax", "%xmm0", s);
  }
  // s<< "----------------释放callee-saved-registers------------------"<<endl;
  int k = stmtIndex;
  while(k!=0) {
    emit_pop("%r15",s);
    emit_add("%r15","%rsp",s);
    k--;
  }
  emit_pop("%r15", s);
  emit_pop("%r14", s);
  emit_pop("%r13", s);
  emit_pop("%r12", s);
  emit_pop("%rbx", s);
  emit_pop("%rbp", s);
  emit_ret(s);

  if (cgen_debug)
    s<<"-----------/return----------"<<endl;

}

void ContinueStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------continue----------"<<endl;
  int k = continuePoint->lookup(1)->stmtNum;
  while(k!=0) {
    emit_pop("%r15",s);
    emit_add("%r15","%rsp",s);
    k--;
  }
  emit_jmp(generateJumpFlag(continuePoint->lookup(1)->flag),s);

  if (cgen_debug)
    s<<"-----------/continue----------"<<endl;
}

void BreakStmt_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------break----------"<<endl;
  int k = breakPoint->lookup(1)->stmtNum;
  while(k!=0) {
    emit_pop("%r15",s);
    emit_add("%r15","%rsp",s);
    k--;
  }
  emit_jmp(generateJumpFlag(breakPoint->lookup(1)->flag),s);

  if (cgen_debug)
    s<<"-----------/break----------"<<endl;

}

static void pushFloatRegister(ostream &s, const char*regName) {
  emit_sub("$8","%rsp",s);
  emit_movsd(regName,generateParam(0,"%rsp"),s);
}

static void popFloatRegister(ostream &s, const char*regName) {
  emit_movsd(generateParam(0,"%rsp"),regName,s);
  emit_add("$8","%rsp",s);
}

static char* generateInt(int a) {
  int num=0;
  int k =a;
  while(a!=0) {
    num++;
    a/=10;
  }
  char* buf = new char(num);
  for(int i=num-1;i>=0;i--) {
    buf[i]=(k%10)+48;
    k/=10;
  }
  return buf;
}

void Call_class::code(ostream &s) {
  if (cgen_debug)
    s<<"-----------call----------"<<endl;

  emit_push("%rdi",s);
  emit_push("%rsi",s);
  emit_push("%rdx",s);
  emit_push("%rcx",s);
  emit_push("%r8",s);
  emit_push("%r9",s);
  pushFloatRegister(s, "%xmm0");
  pushFloatRegister(s, "%xmm1");
  pushFloatRegister(s, "%xmm2");
  pushFloatRegister(s, "%xmm3");
  // pushFloatRegister(s, "%xmm4");
  // pushFloatRegister(s, "%xmm5");
  // pushFloatRegister(s, "%xmm6");
  // pushFloatRegister(s, "%xmm7");

  int fNum=0;
  int notFNum=0;

  for(int i = actuals->first(); actuals->more(i); i = actuals->next(i)) {
    // if (cgen_debug) s << "actual("<<i<<")"<<endl;
    // if (cgen_debug) s << "type: " << actuals->nth(i)->getType() << endl<<endl;
    actuals->nth(i)->code(s);

    // if(actuals->nth(i)->getType()==Float){
    //   fNum++;
    //   emit_sub("$8","%rsp",s);
    //   emit_mov("%xmm8",generateParam(0,"%rsp"),s);
    // }
    if(actuals->nth(i)->getType()==Float) {
      switch (fNum)
      { 
      case 0:
        emit_mov("%xmm8","%xmm0",s);
        fNum++;
        break;
      case 1:
        emit_mov("%xmm8","%xmm1",s);
        fNum++;
        break;
      case 2:
        emit_mov("%xmm8","%xmm2",s);
        fNum++;
        break;
      case 3:
        emit_mov("%xmm8","%xmm3",s);
        fNum++;
        break;
      // case 4:
      //   emit_mov("%xmm8","%xmm4",s);
      //   fNum++;
      //   break;
      // case 5:
      //   emit_mov("%xmm8","%xmm5",s);
      //   fNum++;
      //   break;
      // case 6:
      //   emit_mov("%xmm8","%xmm6",s);
      //   fNum++;
      //   break;
      // case 7:
      //   emit_mov("%xmm8","%xmm7",s);
      //   fNum++;
      //   break;
      default:
        emit_sub("$8","%rsp",s);
        emit_mov("%xmm8",generateParam(0,"%rsp"),s);
        fNum++;
        break;
      }
    } 
    else 
    {switch (notFNum){
      case 0:
        emit_mov("%rax","%rdi",s);
        notFNum++;
        break;

      case 1:
        emit_mov("%rax","%rsi",s);
        notFNum++;
        break;
      
      case 2:
        emit_mov("%rax","%rdx",s);
        notFNum++;
        break;
      
      case 3:
        emit_mov("%rax","%rcx",s);
        notFNum++;
        break;
      
      case 4:
        emit_mov("%rax","%r8",s);
        notFNum++;
        break;
      
      case 5:
        emit_mov("%rax","%r9",s);
        notFNum++;
        break;

      default:
        emit_sub("$8","%rsp",s);
        emit_mov("%rax",generateParam(0,"%rsp"),s);
        notFNum++;
        break;
      }
    }
  }

  if(name==print){
    if(flag) emit_sub("$8","%rsp",s);
    if(fNum) {
      emit_irmovl(generateInt(fNum), EAX, s);
    } 
    else {
      emit_irmovl("0", EAX, s);
    } 
  }

  emit_call(name->get_string(),s);

  if(name==print){
    if(flag) emit_add("$8","%rsp",s);
  }
  
  // popFloatRegister(s, "%xmm7");
  // popFloatRegister(s, "%xmm6");
  // popFloatRegister(s, "%xmm5");
  // popFloatRegister(s, "%xmm4");
  popFloatRegister(s, "%xmm3");
  popFloatRegister(s, "%xmm2");
  popFloatRegister(s, "%xmm1");
  popFloatRegister(s, "%xmm0");
  emit_pop("%r9",s);
  emit_pop("%r8",s);
  emit_pop("%rcx",s);
  emit_pop("%rdx",s);
  emit_pop("%rsi",s);
  emit_pop("%rdi",s);

  //
  /*
   if function name is printf

    // please set %eax to the number of Float parameters, num.
    //  把%eax赋值为Float类型的参数个数, num
    emit_sub("$8", RSP, s);
    emit_irmovl(num, EAX, s);
    emit_call("printf", s);
      
    return;
  }
  */
  //

  if (cgen_debug)
    s<<"-----------/call----------"<<endl;

}

void Actual_class::code(ostream &s) {
  if (cgen_debug)
    s<<"<actual>"<<endl;
  // s<<"<call expr->code(s)>"<<endl;

  expr->code(s);
  // s<<"</call expr->code(s)>"<<endl;
  
  if (cgen_debug)
    s<<"</actual>"<<endl;
}

void Assign_class::code(ostream &s) {
  if (cgen_debug)
    s<<"<assign>"<<endl;

  value->code(s);
  if(map->lookup(lvalue)->isFloat) {
    if(value->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    if(map->lookup(lvalue)->inRegister) {
      emit_mov("%xmm8",map->lookup(lvalue)->add,s);
    } else {
      emit_movsd("%xmm8",map->lookup(lvalue)->add,s);
    }
  } else {
    emit_mov("%rax",map->lookup(lvalue)->add,s);
  }
  // if(value->isFloat()&&map->lookup(lvalue)->isFloat) {
  //   emit_movsd("%rax","%xmm8");
  //   emit_movsd
  // }

  if (cgen_debug)
    s<<"</assign>"<<endl;

}

void Add_class::code(ostream &s) {
  if (cgen_debug) s<<"<add>"<<endl;

  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_addsd("%xmm9", "%xmm8", s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx", s);
    emit_add("%rbx","%rax",s);
  }

  if (cgen_debug) s<<"</add>"<<endl;
}

void Minus_class::code(ostream &s) {
  if (cgen_debug) s<<"<minus>"<<endl;

  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_subsd("%xmm9", "%xmm8", s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx", s);
    emit_sub("%rbx","%rax",s);
  }

  if (cgen_debug) s<<"</minus>"<<endl;

}

void Multi_class::code(ostream &s) {
  
  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_mulsd("%xmm9", "%xmm8", s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx", s);
    emit_mul("%rbx","%rax",s);
  }

}

void Divide_class::code(ostream &s) {
  
  if(getType()==Float) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_divsd("%xmm9", "%xmm8", s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx", s);
    emit_div("%rbx",s);
  }

}

void Mod_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_xor("%rdx","%rdx",s);
  emit_pop("%r13",s);
  emit_div("%r13",s);
  emit_mov("%rdx","%rax",s);
}

void Neg_class::code(ostream &s) {
  if (cgen_debug) s<<"<neg>"<<endl;
  if(e1->getType()==Float) {
    e1->code(s);
    emit_mov("%xmm8","%xmm9",s);
    emit_mov("$0","%rax",s);
    emit_int_to_float("%rax","%xmm8",s);
    emit_subsd("%xmm9","%xmm8",s);
  } else {
    e1->code(s);
    emit_not("%rax",s);
    emit_add("$1","%rax",s);
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
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_setg("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_setg("%r13b",s);
    // emit_setge("%r13b",s);
    emit_mov("%r13","%rax",s);
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
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_setge("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_setge("%r13b",s);
    emit_mov("%r13","%rax",s);
    // emit_cmp("%rbx","%rax",s);
  }

  if (cgen_debug) s<<"</le>"<<endl;
}

// e1 == e2
void Equ_class::code(ostream &s) {
  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_sete("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_sete("%r13b",s);
    emit_mov("%r13","%rax",s);
  }
}

// e1!=e2
void Neq_class::code(ostream &s) {
  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_setne("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_setne("%r13b",s);
    emit_mov("%r13","%rax",s);
  }
}


// e1 >= e2
void Ge_class::code(ostream &s) {
  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_setle("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_setle("%r13b",s);
    emit_mov("%r13","%rax",s);
  }
}

// e1 > e2
void Gt_class::code(ostream &s) {
  bool hasFloat = (e1->getType()==Float)||(e2->getType()==Float);
  if(hasFloat) {
    e2->code(s);
    if(e2->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8",s);
    }
    emit_mov("%xmm8","%r12",s);
    emit_push("%r12",s);
    e1->code(s);
    if(e1->getType()==Int) {
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_int_to_float("%rax", "%xmm8", s);
    }
    emit_pop("%r13", s);
    emit_xorpd("%xmm9", "%xmm9", s);
    emit_mov("%r13","%xmm9",s);
    emit_xor("%r13","%r13",s);
    emit_ucompisd("%xmm8","%xmm9",s);
    emit_setl("%r13b",s);
    emit_mov("%r13","%rax",s);
  } else {
    e2->code(s);
    emit_push("%rax", s);
    e1->code(s);
    emit_pop("%rbx",s);
    emit_xor("%r13","%r13",s);
    emit_cmp("%rax","%rbx",s);
    emit_setl("%r13b",s);
    emit_mov("%r13","%rax",s);
  }
}

void And_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_pop("%rbx", s);
  emit_and("%rbx","%rax", s);
}

void Or_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_pop("%rbx", s);
  emit_or("%rbx","%rax", s);
}

void Xor_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_pop("%rbx", s);
  emit_xor("%rbx","%rax", s);
}

void Not_class::code(ostream &s) {
  e1->code(s);
  emit_not("%rax",s);
}

void Bitnot_class::code(ostream &s) {

  e1->code(s);

  emit_not("%rax",s);
}

void Bitand_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_pop("%rbx", s);
  emit_and("%rbx","%rax", s);
}

void Bitor_class::code(ostream &s) {
  e2->code(s);
  emit_push("%rax", s);
  e1->code(s);
  emit_pop("%rbx", s);
  emit_or("%rbx","%rax", s);
}


void Const_int_class::code(ostream &s) {
  if (cgen_debug) s<<"<int const>"<<endl;

  s<< MOV << "$" << value->get_string()<< COMMA <<RAX<<endl;

  if (cgen_debug) s<<"</int const>"<<endl;
}

void Const_string_class::code(ostream &s) {
  s<< MOV;
  stringtable.lookup_string(value->get_string())->code_ref(s);
  s<< COMMA << RAX << endl;
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
    if(inReg()) {
      emit_movsd(map->lookup(var)->add,generateParam(-8,"%rsp"),s);
      emit_mov(generateParam(-8,"%rsp"),"%xmm8",s);
    } else {
      emit_mov(map->lookup(var)->add,"%r12",s);
      emit_mov("%r12",generateParam(-8,"%rsp"),s);
      emit_xorpd("%xmm8", "%xmm8", s);
      emit_mov(generateParam(-8,"%rsp"),"%xmm8",s);
    }
  } 
  else emit_mov(map->lookup(var)->add,RAX,s);
  
  if (cgen_debug) s<<"</object>"<<endl;
}

void No_expr_class::code(ostream& s) {
  emit_mov("$1","%rax",s);
}