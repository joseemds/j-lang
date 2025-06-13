%{
#include <stdio.h>
#include <string.h>
#include "include/y.tab.h"

  extern int yyerror (char *msg);  
%}

%option noyywrap nounput noinput

DIGIT    [0-9]
UPPERCASE_IDENTIFIER [A-Z][a-zA-Z0-9_]*
LOWERCASE_IDENTIFIER [a-z_][a-zA-Z0-9_]*
WHITESPACE [ \n\t\r]+

STRING_LITERAL \"[^"]*\"

%%

{WHITESPACE}          {}

"type"                { return TYPE; }
"func"                { return FUNC; }
"val"                 { return VAL; }
"if"                  { return IF; }
"else"                { return ELSE; }
"while"               { return WHILE; }
"for"                 { return FOR; }
"return"              { return RETURN; }
"import"              { return IMPORT; }
"enum"                { return ENUM; }

{DIGIT}+              { return NUMBER;}
{STRING_LITERAL}      {
                        return STRING_LIT;
                      }

{UPPERCASE_IDENTIFIER} { return UID; }
{LOWERCASE_IDENTIFIER} { return LID;}

"="                   { return EQUAL; }
"=="                  { return CMP; }
"<="                  { return LEQ; }
"<"                   { return LT; }
">="                  { return GEQ; }
">"                   { return GT; }
"!="                  { return NEQ; }

"+"                   { return PLUS; }
"-"                   { return MINUS; }
"*"                   { return TIMES; }
"/"                   { return DIVIDE; }
"&&"                  { return AND; }
"||"                  { return OR; }
"!"                   { return NOT; }


"("                   { return LPAREN; }
")"                   { return RPAREN; }
"{"                   { return LBRACE; }
"}"                   { return RBRACE; }
"["                   { return LBRACKET; }
"]"                   { return RBRACKET; }
":"                   { return COLON; }
";"                   { return SEMICOLON; }
","                   { return COMMA; }
"."                   { return DOT; }

.                     {yyerror("Unknown character");}

%%

#ifndef yywrap
   int yywrap (void) {return 1;}
#endif
