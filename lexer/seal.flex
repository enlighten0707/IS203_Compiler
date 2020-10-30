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
#include <string>

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

int commentLevel = 0;
bool flag = true;
int str_type;

long hex2Dec (char* hex) {
  char* res = new char[MAX_STR_CONST];
  long number = std::stoi(hex, nullptr, 16);
  return number;
}

char* getstr (const char* str) {
  char *result;
  int i = 1, j = 0;
  int len = strlen(str);
  result = new char[MAX_STR_CONST];

  while (i < len - 1) {
    if (i < len-2 && str[i] == '\\') {
      if (str[i+1] == '\n') {
        result[j] = '\n';
        i += 2; 
      } else if (str[i+1] == '\\') {
        result[j] = '\\';
        i += 2;
      } else if (str[i+1] == '0') {
        result[j] = '\\';
        i += 1;
      } else if (str[i+1] == 't') {
        result[j] = '\t';
        i += 2;
      } else {
        result[j] = '\\';
        j++;
        result[j] = str[i+1];
        i += 2;
      }
    } else {
      result[j] = str[i];
      i++;
    }
    j++;
  }
  result[j] = '\0';
  return result;
}
/*
 *  Add Your own definitions here
 */


%}

%option noyywrap
 /*
  * Define names for regular expressions here.
  */
HEX         0[xX][A-Fa-f0-9]+
NUMBER      (0|[1-9][0-9]*)
FLOAT       (0|[1-9][0-9]*).[0-9]+

 // Basic Data Type
TYPE_IDENTIFIER   (Float|Int|Bool|String|Void)

 //OBJECT_IDENTIFIER: begin with a-z, then a-z,A-z,0-9,_
OBJECT_IDENTIFIER    [a-z][a-zA-Z0-9_]* 

 // WRONG_IDENTIFIER, begin with 0-9,_? TODO
WRONG_IDENTIFIER  [A-Z][a-zA-Z0-9_]*

%Start COMMENT 
%Start STRING
%Start COMMENT_LINE

%%
 /* Operators */
<INITIAL>"+"        { return ('+'); }
<INITIAL>"-"        { return ('-'); }
<INITIAL>"*"        { return ('*'); }
<INITIAL>"/"        { return ('/'); }
<INITIAL>"%"        { return ('%'); }

<INITIAL>"="        { return ('='); }
<INITIAL>"=="       { return EQUAL; }
<INITIAL>"!="       { return NE; }
<INITIAL>">="       { return GE; }
<INITIAL>"<="       { return LE; }
<INITIAL>"&&"       { return AND; }
<INITIAL>"||"       { return OR; }
<INITIAL>"<"        { return ('<'); }
<INITIAL>">"        { return ('>'); }

<INITIAL>\{         { return ('{'); }
<INITIAL>\}         { return ('}'); }
<INITIAL>\(         { return ('('); }
<INITIAL>\)         { return (')'); }

<INITIAL>";"        { return (';'); }
<INITIAL>","        { return (','); }
<INITIAL>"."        { return ('.'); }
<INITIAL>":"        { return (':'); }
<INITIAL>"~"        { return ('~'); }
<INITIAL>"!"        { return ('!'); }
<INITIAL>"|"        { return ('|'); }


  /* int */
<INITIAL>{NUMBER} {
                    seal_yylval.symbol = inttable.add_string(yytext);
                    return CONST_INT;
                  }
<INITIAL>{HEX}    {
                    long number = hex2Dec(yytext);
                    seal_yylval.symbol = inttable.add_int(number);
                    return CONST_INT;
                  }

  /* float */
<INITIAL>{FLOAT} {
                    seal_yylval.symbol = floattable.add_string(yytext);
                    return CONST_FLOAT;
                  }
  /* boolean */
<INITIAL>true     { 
                    seal_yylval.boolean = 1;
                    return CONST_BOOL;
                  }
<INITIAL>false    {
                    seal_yylval.boolean = 0;
                    return CONST_BOOL;
                  }


  /* identifiers */
<INITIAL>var        { return VAR; }
<INITIAL>if         { return IF; }
<INITIAL>else       { return ELSE; }
<INITIAL>while      { return WHILE; }
<INITIAL>for        { return FOR; }
<INITIAL>break      { return BREAK; }
<INITIAL>continue   { return CONTINUE; }
<INITIAL>func       { return FUNC; }
<INITIAL>return     { return RETURN; }
<INITIAL>struct     { return STRUCT; }

<INITIAL>{OBJECT_IDENTIFIER} {
                              seal_yylval.symbol = idtable.add_string(yytext);
                              return OBJECTID;
                            }
<INITIAL>{TYPE_IDENTIFIER}  {
                              seal_yylval.symbol = idtable.add_string(yytext);
                              return TYPEID;
                            }
<INITIAL>{WRONG_IDENTIFIER} {
                              strcpy(string_buf, "illegal TYPEID ");
                              strcpy(seal_yylval.error_msg, strcat(string_buf, yytext));
                              return ERROR;
                            }
 
 
 /* comment */

<INITIAL>"//" {BEGIN COMMENT_LINE;}
<COMMENT_LINE>\n {
                    curr_lineno++; 
                    BEGIN INITIAL;
                  }
<COMMENT_LINE>. {}

<INITIAL>"/*" { 
                commentLevel += 1;
                BEGIN COMMENT;
              }
<INITIAL>"*/" { 
                strcpy(seal_yylval.error_msg,"Unmatched */ for comment."); 
                return ERROR; 
              }
<COMMENT>"/*" {
                commentLevel += 1;
              }
<COMMENT><<EOF>>  {
                    strcpy(seal_yylval.error_msg, "EOF in comment.");
                    BEGIN INITIAL;
                    return ERROR;
                  }
<COMMENT>"*/" { 
                if (--commentLevel == 0) BEGIN INITIAL; 
              }
<COMMENT>. {}


 /* string */
<INITIAL>\" { BEGIN STRING; str_type = 0; yymore(); }
<INITIAL>` { BEGIN STRING; str_type = 1; yymore(); }
<STRING>\"  {
              if (flag == false) {
                flag = true;
                BEGIN INITIAL;
                return ERROR;
              } 
              else {
                if (yyleng >= MAX_STR_CONST) {
                  strcpy(seal_yylval.error_msg, "String is too long");
                  BEGIN INITIAL;
                  return ERROR;
                } 
                else {
                  char* result = getstr(yytext);
                  seal_yylval.symbol = stringtable.add_string(result);
                  BEGIN INITIAL;
                  return CONST_STRING;
                }
              }                 
            }
<STRING>`  {
              if (flag == false) {
                flag = true;
                BEGIN INITIAL;
              } 
              else {
                if (yyleng >= MAX_STR_CONST) {
                  strcpy(seal_yylval.error_msg, "String is too long.");
                  BEGIN INITIAL;
                  return ERROR;
                } 
                else {
                  char* result = getstr(yytext);
                  seal_yylval.symbol = stringtable.add_string(result);
                  BEGIN INITIAL;
                  return CONST_STRING;
                }
              }                 
            }
<STRING>\\0 { 
              if (str_type == 0) {
                strcpy(seal_yylval.error_msg, "String contains null character '\\0'"); 
                flag = false;
              } else {
                yymore();
              } 
            }
<STRING><<EOF>>   {
                    strcpy(seal_yylval.error_msg, "EOF in string.");
                    BEGIN INITIAL;
                    yyrestart(yyin);
                    return ERROR;
                  }
<STRING>[ ] { yymore(); }
<STRING>[^\\\"\`\n]* { yymore(); }
<STRING>\\[^\n] { yymore(); }
<STRING>\\\n { curr_lineno++; yymore(); }
<STRING>\n {
  if (str_type == 0) {
    strcpy(seal_yylval.error_msg, "newline in quotation must use a '\\'");
    curr_lineno += 1;
    BEGIN INITIAL;
    return ERROR;
  } else {
    curr_lineno += 1;
    yymore();
  }
}

  /* space */
[ \t] {}
\n    { curr_lineno++; }

 /* error */
<INITIAL>.	{
  strcpy(yylval.error_msg, yytext);
  return ERROR;
}
%%
