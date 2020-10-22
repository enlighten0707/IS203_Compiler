 /*
  *  The scanner definition for seal.
  */

 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{

#include <seal-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* The compiler assumes these identifiers. */
#define yylval seal_yylval
#define yylex  seal_yylex

/* Max size of string constants */
#define MAX_STR_CONST 256
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the seal compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE seal_yylval;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


/*
 *  Add Your own definitions here
 */


%}

%option noyywrap
 /*
  * Define names for regular expressions here.
  */

%%


\n {
  curr_lineno  = curr_lineno+1;
}

"/*"(.|\n)*"*/" {}

={2} {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (EQUAL);
}

= {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('=');
}

\+ {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('+');
}

- {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('-');
}

\* {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('*');
}

% {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('%');
}

>= {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (GE);
}

> {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('>');
}

\| {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('|');
}

; {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (';');
}

, {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (',');
}

\{ {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('{');
}

\} {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('}');
}

\( {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return ('(');
}

\) {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (')');
}

if {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (IF);
}

while {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (WHILE);
}

func {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (FUNC);
}

return {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (RETURN);
}

true {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (CONST_BOOL);
}

false {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (CONST_BOOL);
}

[`\"][0-9a-zA-Z\\t\n\0 ]*[`\"] {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (CONST_STRING);
}

0x[0-9a-f]+ {
  seal_yylval.symbol = inttable.add_string(yytext); 
	return (CONST_INT);
}

[0-9]+ {
  seal_yylval.symbol = inttable.add_string(yytext); 
	return (CONST_INT);
  }

[0-9]+\.[0-9]+ {
  seal_yylval.symbol = inttable.add_string(yytext); 
	return (CONST_FLOAT);
  }


var {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (VAR);
}

Int {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (TYPEID);
}

Float {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (TYPEID);
}

String {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (TYPEID);
}

[a-z][a-zA-Z0-9_]*  {
  seal_yylval.symbol = stringtable.add_string(yytext); 
	return (OBJECTID);
}

[A-Z0-9][a-z0-9A-Z]* {
  strcpy(seal_yylval.error_msg, yytext); 
	return (ERROR); 
}

. {}

%%
