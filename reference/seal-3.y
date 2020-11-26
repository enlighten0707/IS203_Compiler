/*
*  seal.y
*              Parser definition for the SEAL language.
*
*/
%{
  #include <iostream>
  #include "seal-decl.h"
  #include "seal-stmt.h"
  #include "seal-expr.h" /*注意一下以上三个头文件*/
  #include "stringtab.h"
  #include "utilities.h"

  extern char *curr_filename;
  /* Locations */
  #define YYLTYPE int              /* the type of locations */
  #define seal_yylloc curr_lineno  
  /* use the curr_lineno from the lexer for the location of tokens */
    
    extern int node_lineno;          /* set before constructing a tree node
    to whatever you want the line number
    for the tree node to be */
      
      
      #define YYLLOC_DEFAULT(Current, Rhs, N)         \
      Current = Rhs[1];                             \
      node_lineno = Current;
    
    
    #define SET_NODELOC(Current)  \
    node_lineno = Current;
    
    /* IMPORTANT NOTE ON LINE NUMBERS
    *********************************
    * The above definitions and macros cause every terminal in your grammar to 
    * have the line number supplied by the lexer. The only task you have to
    * implement for line numbers to work correctly, is to use SET_NODELOC()
    * before constructing any constructs from non-terminals in your grammar.
    * Example: Consider you are matching on the following very restrictive 
    * (fictional) construct that matches a plus between two integer constants. 
    * (SUCH A RULE SHOULD NOT BE PART OF YOUR PARSER):
    
    add_consts	: INT_CONST '+' INT_CONST 
    
    * where INT_CONST is a terminal for an integer constant. Now, a correct
    * action for this rule that attaches the correct line number to plus_const
    * would look like the following:
    
    add_consts	: INT_CONST '+' INT_CONST 
    {
      // Set the line number of the current non-terminal:
      // ***********************************************
      // You can access the line numbers of the i'th item with @i, just
      // like you acess the value of the i'th exporession with $i.
      //
      // Here, we choose the line number of the last INT_CONST (@3) as the
      // line number of the resulting expression (@$). You are free to pick
      // any reasonable line as the line number of non-terminals. If you 
      // omit the statement @$=..., bison has default rules for deciding which 
      // line number to use. Check the manual for details if you are interested.
      @$ = @3;
      
      
      // Observe that we call SET_NODELOC(@3); this will set the global variable
      // node_lineno to @3. Since the constructor call "plus" uses the value of 
      // this global, the plus node will now have the correct line number.
      SET_NODELOC(@3);
      
      // construct the result node:
      $$ = add(int_const($1), int_const($3));
    }
    
    */
    
    
    
    void yyerror(char *s);        /*  defined below; called for each parse error */
    extern int yylex();           /*  the entry point to the lexer  */
    
    /************************************************************************/
    /*                DONT CHANGE ANYTHING IN THIS SECTION                  */
    
    Program ast_root;	      /* the result of the parse  */
    //Decls parse_results;        /* for use in semantic analysis */
    int omerrs = 0;               /* number of errors in lexing and parsing */
    %}
    
    /* A union of all the types that can be the result of parsing actions. */
    %union {
      Boolean boolean;
      Symbol symbol;
      Program program;
      Decl decl;
      Decls decls;
      VariableDecl variableDecl;
      VariableDecls variableDecls;
      Variable variable;
      Variables variables;
      CallDecl callDecl;
      StmtBlock stmtBlock;
      Stmt stmt;
      Stmts stmts;
      IfStmt ifStmt;
      WhileStmt whileStmt;
      ForStmt forStmt;
      ReturnStmt returnStmt;
      ContinueStmt continueStmt;
      BreakStmt breakStmt;
      Expr expr;
      Exprs exprs;
      Call call;
      Actual actual;
      Actuals actuals;
      
      char *error_msg;
    }
    
    /* 
    Declare the terminals; a few have types for associated lexemes.
    The token ERROR is never used in the parser; thus, it is a parse
    error when the lexer returns it.
    
    The integer following token declaration is the numeric constant used
    to represent that token internally.  Typically, Bison generates these
    on its own, but we give explicit numbers to prevent version parity
    problems (bison 1.25 and earlier start at 258, later versions -- at
    257)
    */
    %token IF 258 ELSE 260 WHILE 261 FOR 262 BREAK 263 CONTINUE 264 
    %token FUNC 265 RETURN 266  
    %token VAR 271 ERROR 273
	  %token AND 274 OR 275 EQUAL 276 NE 277 GE 278 LE 279
	  %token INT 280 STRING 281 BOOL 282 FLOAT 283 
    %token <boolean> CONST_BOOL 267
    %token <symbol>  CONST_INT 268 CONST_STRING 269 CONST_FLOAT 270
    %token <symbol>  OBJECTID 284 TYPEID 285 CLASS 286
    
	  %token UMINUS
    /*  DON'T CHANGE ANYTHING ABOVE THIS LINE, OR YOUR PARSER WONT WORK       */
    /**************************************************************************/
    
    /* Complete the nonterminal list below, giving a type for the semantic
    value of each non terminal. (See section 3.6 in the bison 
    documentation for details). */
    
    /* Declare the type of non-terminals. */
    %type <program> program

    %type <decls> decls
    %type <decl> decl

    %type <variableDecl> variableDecl
    %type <variableDecls> variableDecls
    %type <variables> variables
    %type <variable> variable
	  %type <callDecl> callDecl

    %type <stmtBlock> stmtBlock
    %type <stmt> stmt
    %type <stmts> stmts

    %type <ifStmt> ifStmt
    %type <whileStmt> whileStmt
    %type <forStmt> forStmt
    %type <returnStmt> returnStmt
    %type <continueStmt> continueStmt
    %type <breakStmt> breakStmt

    %type <expr> expr
    %type <exprs> exprs

    %type <call> call
    %type <actual> actual
    %type <actuals> actuals

    /* Declare Precedence of operations */
      %nonassoc '='
      %right OR
      %right AND
      %nonassoc EQUAL NE
      %nonassoc '<' '>' LE GE
      %left '+' '-'
      %left '*' '/' '%'
      %nonassoc minusminus
      %nonassoc '!'    
      %left '~' '&' '|' '^'
%%

    /* 1、程序 -》 一系列声明 */
    program	: decls { 
    // program 是一个非终结符
    // decl_list 也是一个非终结符
        @$ = @1;
        ast_root = program($1); 
        //cout<<"------program	: decls-----"<<endl;
      }
      ;
    /* 2、一系列声明 -》 一个声明 */
    decls	: decl	{ /* single declaration */
        $$ = single_Decls($1);
        //cout<<"------decls	: decl-----"<<endl;
				}
				| decls decl { /* several declarations */
			  $$ = append_Decls($1, single_Decls($2)); 
        //cout<<"------decls	: decls decl-----"<<endl;
				}
				;
    
    /* 3、一个声明 -》 变量声明 | 函数声明 */
    decl : variableDecl{
        $$ = $1;
        //cout<<"------decl : variableDecl-----"<<endl;
				}
        | callDecl{
        $$ = $1;
        //cout<<"------decl : callDecl-----"<<endl;
        }
				;

    variableDecl : VAR variable ';'{
      $$ = variableDecl($2);
      //cout<<"------variableDecl : VAR variable ';'-----"<<endl;
    };

    variable : OBJECTID TYPEID{
      $$ = variable($1, $2);
      //cout<<"------variable : OBJECTID TYPEID-----"<<endl;
    }

    callDecl : FUNC OBJECTID '(' ')' TYPEID stmtBlock {
      $$ = callDecl($2, nil_Variables(), $5, $6);
      //cout<<"------callDecl : FUNC OBJECTID '(' ')' TYPEID stmtBlock-----"<<endl;
    }
    | FUNC OBJECTID '(' variables ')' TYPEID stmtBlock {
      $$ = callDecl($2, $4, $6, $7);
      //cout<<"------callDecl : FUNC OBJECTID '(' variables ')' TYPEID stmtBlock-----"<<endl;
    }
    ;

    variables : variable {
      $$ = single_Variables($1);
      //cout<<"------variables : variable-----"<<endl;
    }
    | variables ',' variable{
      $$ = append_Variables($1, single_Variables($3));
      //cout<<"------variables : variables variable-----"<<endl;
    }
    ;

    stmtBlock : '{' '}'{
      $$ = stmtBlock(nil_VariableDecls(), nil_Stmts());
      //cout<<"------ stmtBlock : '{' variableDecls '}'-----"<<endl;
    }
    | '{' variableDecls '}' {
      $$ = stmtBlock($2, nil_Stmts());
      //cout<<"------ stmtBlock : '{' variableDecls '}'-----"<<endl;
    }
    | '{' stmts '}' {
      $$ = stmtBlock(nil_VariableDecls(), $2);
      //cout<<"------stmtBlock : '{' stmts '}'-----"<<endl;
    }
    | '{' variableDecls stmts '}' {
      $$ = stmtBlock($2, $3);
      //cout<<"------stmtBlock : '{' variableDecls stmts '}'-----"<<endl;
    }
    ;

    variableDecls : variableDecl{
      $$ = single_VariableDecls($1);
      //cout<<"------variableDecls : -----"<<endl;
    }
    | variableDecls variableDecl{
      $$ = append_VariableDecls($1, single_VariableDecls($2));
      //cout<<"------variableDecls : variableDecls variableDecl-----"<<endl;
    }
    ;

    stmts : ';'{
      $$ = nil_Stmts();
    }
    | stmt {
      $$ = single_Stmts($1);
      //cout<<"------stmts : stmt-----"<<endl;
    } 
    | stmts stmt {
      $$ = append_Stmts($1, single_Stmts($2));
      //cout<<"------stmts : stmt stmts-----"<<endl;
    }
    | stmts ';' {
    }
    ;

    stmt : expr ';' | ifStmt | whileStmt | forStmt | breakStmt |
           continueStmt | returnStmt | stmtBlock {
      $$ = $1;
      //cout<<"------stmt : ';' | expr ';' | ifstmt | whileStmt | forStmt | breakStmt|continueStmt | returnStmt | stmtBlock-----"<<endl;
    }

    ifStmt : IF expr stmtBlock {
         $$ = ifstmt($2, $3, stmtBlock(nil_VariableDecls(),nil_Stmts()));
         //cout<<"------ifstmt : IF expr stmtBlock-----"<<endl;
    }
    | IF expr stmtBlock ELSE stmtBlock{
         $$ = ifstmt($2, $3, $5);
         //cout<<"------ifstmt : IF expr stmtBlock ELSE stmtBlock-----"<<endl;
    }
    ;

    whileStmt : WHILE expr stmtBlock{
        $$ = whilestmt($2, $3);
        //cout<<"------whileStmt : WHILE expr stmtBlock-----"<<endl;
    }
    ;

    forStmt : FOR  expr ';' expr ';' expr stmtBlock{
        $$ = forstmt($2, $4, $6, $7);
        //cout<<"------forStmt : FOR  expr ';' expr ';' expr stmtBlock-----"<<endl;
    }
    | FOR ';' expr ';'expr stmtBlock { 
        $$ = forstmt(no_expr(), $3, $5, $6);
        //cout<<"------forStmt : FOR ';' expr ';'expr stmtBlock-----"<<endl;
    }
    | FOR expr';'';'expr stmtBlock {
        $$ = forstmt($2, no_expr(), $5, $6);
        //cout<<"------forStmt : FOR'('expr';'';'expr')' stmtBlock-----"<<endl;
    }
    | FOR expr';'expr';' stmtBlock {
        $$ = forstmt($2, $4, no_expr(), $6);
        //cout<<"------forStmt : FOR'('expr';'expr';'')' stmtBlock-----"<<endl;
    }
    | FOR ';'';'expr stmtBlock {
        $$ = forstmt(no_expr(), no_expr(), $4, $5);
        //cout<<"------forStmt : FOR'('';'';'expr')' stmtBlock-----"<<endl;
    }
    | FOR ';' expr ';' stmtBlock {
        $$ = forstmt(no_expr(), $3, no_expr(), $5);
        //cout<<"------forStmt : FOR'('';'expr';'')' stmtBlock-----"<<endl;
    }
    | FOR expr ';' ';' stmtBlock {
        $$ = forstmt($2, no_expr(), no_expr(), $5);
        //cout<<"------forStmt : FOR expr ';' ';' stmtBlock-----"<<endl;
    }
    | FOR ';' ';' stmtBlock {
        $$ = forstmt(no_expr(), no_expr(), no_expr(), $4);
        //cout<<"------forStmt : FOR ';' ';' stmtBlock-----"<<endl;
    }
    ;

    returnStmt : RETURN expr ';'{
       $$ = returnstmt($2);
       //cout<<"------returnStmt : RETURN expr ';'-----"<<endl;
    }
    | RETURN ';'{
      $$ = returnstmt(no_expr());
      //cout<<"------returnStmt : RETURN ';'-----"<<endl;
    }
    ;

    continueStmt : CONTINUE ';' {
      $$ = continuestmt();
      //cout<<"------continueStmt : CONTINUE ';'-----"<<endl;
    }
    ;

    breakStmt : BREAK ';' {
       $$ = breakstmt();
       //cout<<"------breakStmt : BREAK ';'-----"<<endl;
    }
    ;

    expr :OBJECTID{
              $$ = object($1);
          }
          | OBJECTID '=' expr {
              $$ = assign($1, $3);
              //cout<<"------expr :OBJECTID '=' expr-----"<<endl;
          }
          | CONST_BOOL {
              $$ = const_bool($1);
              //cout<<"------expr :OBJECTID '=' expr-----"<<endl;
          }
          | CONST_FLOAT {
              $$ = const_float($1);
              //cout<<"------expr :CONST_FLOAT-----"<<endl;
          }
          | CONST_INT {
              $$ = const_int($1);
              //cout<<"------expr :CONST_INT-----"<<endl;
          }
          | CONST_STRING {
              $$ = const_string($1);
              //cout<<"------expr :CONST_STRING-----"<<endl;
          }
          | call {
              $$ = $1;
              //cout<<"------expr :call-----"<<endl;
          }
          | '(' expr ')' {
              $$ = $2;
              //cout<<"------expr :'(' expr ')'-----"<<endl;
          }
          |  expr '+' expr {
               $$ = add($1, $3);
               //cout<<"------expr :'(' expr ')'-----"<<endl;
          }
          |  expr '-' expr {
               $$ = minus($1, $3);
               //cout<<"------expr :expr '-' expr-----"<<endl;
          }
          |  expr '*' expr {
               $$ = multi($1, $3);
               //cout<<"------expr :expr '*' expr-----"<<endl;
          }
          |  expr '/' expr {
               $$ = divide($1, $3);
               //cout<<"------expr :expr '/' expr-----"<<endl;
          }
          |  expr '%' expr {
               $$ = mod($1, $3);
               //cout<<"------expr :expr '%' expr-----"<<endl;
          }
          |  '-' expr  %prec minusminus
          {
               $$ = neg($2);
               //cout<<"------expr :'-' expr-----"<<endl;
          }
          |  expr '<' expr {
               $$ = lt($1, $3);
               //cout<<"------expr :expr '<' expr-----"<<endl;
          }
          |  expr LE expr {
               $$ = le($1, $3);
               //cout<<"------expr :expr LE expr-----"<<endl;
          }
          |  expr EQUAL expr {
               $$ = equ($1, $3);
               //cout<<"------expr :expr EQUAL expr-----"<<endl;
          }
          |  expr NE expr {
               $$ = neq($1, $3);
               //cout<<"------expr :expr NE expr-----"<<endl;
          }
          |  expr GE expr {
               $$ = ge($1, $3);
               //cout<<"------expr :expr GE expr-----"<<endl;
          }
          |  expr '>' expr {
               $$ = gt($1, $3);
               //cout<<"------expr :expr '>' expr-----"<<endl;
          }
          |  expr AND expr {
               $$ = and_($1, $3);
               //cout<<"------expr : expr AND expr-----"<<endl;
          }
          |  expr OR expr {
               $$ = or_($1, $3);
             //cout<<"------expr : expr OR expr-----"<<endl;
          }
          |  expr '^' expr {
               $$ = xor_($1, $3);
            //cout<<"------expr : expr '^' expr-----"<<endl;
          }
          |  '!' expr {
               $$ = not_($2);
            //cout<<"------expr : '!' expr-----"<<endl;
          }
          |  '~' expr {
               $$ = bitnot($2);
            //cout<<"------expr : '~' expr-----"<<endl;
          }
          |  expr '&' expr {
            $$ = bitand_($1, $3);
          //cout<<"------expr : expr '&' expr-----"<<endl;
          }
          |  expr '|' expr {
            $$=bitor_($1, $3);
          //cout<<"------expr : expr '|' expr-----"<<endl;
          }
          ;

    call : OBJECTID '(' actuals ')'{
      $$ = call($1, $3);
     //cout<<"------call : OBJECTID '(' actuals ')'-----"<<endl;
    }
    |  OBJECTID '(' ')'{
      $$ = call($1, nil_Actuals());
     //cout<<"------call : OBJECTID '(' ')'-----"<<endl;
    }
    ; 

    actuals : actual{
      $$ = single_Actuals($1);
     //cout<<"------actuals : actual-----"<<endl;
    }
    | actuals ',' actual {
      $$ = append_Actuals($1, single_Actuals($3));
     //cout<<"------actuals : actual actuals-----"<<endl;
    }

    actual : {
      $$ = actual(no_expr());
     //cout<<"------ actual : -----"<<endl;
     }
    | expr {
      $$ = actual($1);
      //cout<<"------ actual : expr-----"<<endl;
    }
     
%%
    
    /* This function is called automatically when Bison detects a parse error. */
    void yyerror(char *s)
    {
      extern int curr_lineno;
      
      cerr << "\"" << curr_filename << "\", line " << curr_lineno + 1<< ": " \
      << s << " at or near ";
      print_seal_token(yychar);
      cerr << endl;
      omerrs++;
      
      if(omerrs>50) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
    }
