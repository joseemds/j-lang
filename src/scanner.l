%{
#include <stdio.h>
#include <string.h>
#include "include/y.tab.h"

int yycol = 1;

#define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycol; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycol + yyleng - 1; \
    yycol += yyleng;

extern int yyerror (char *msg);  
%}


%option noyywrap nounput noinput
%option yylineno

DIGIT     [0-9]
UPPERCASE_IDENTIFIER   [A-Z][a-zA-Z0-9_]*
LOWERCASE_IDENTIFIER   [a-z_][a-zA-Z0-9_]*
WHITESPACE   [ \n\t\r]+

PRIM_TYPE  (Int|String|Bool|Float|Frac|Void|Char)

CHAR_LITERAL  '[^']'
STRING_LITERAL  \"[^"]*\"

%%

"--".*                  {}

{WHITESPACE}            {}

{PRIM_TYPE}           { yylval.sValue = strdup(yytext); return PRIM_TYPE; }

"true"                { return TRUE; }
"false"               { return FALSE; }
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
"break"               { return BREAK; }
"continue"            { return CONTINUE; }

{DIGIT}+              { yylval.iValue = atoi(yytext); return NUMBER;}
{DIGIT}*\.{DIGIT}+    { return FLOAT; }
{CHAR_LITERAL}        {
                        yylval.cValue = strdup(yytext);
                        return CHAR_LIT;
                      }
{STRING_LITERAL}      {
                        yylval.sValue = strdup(yytext);
                        return STRING_LIT;
                      }

{UPPERCASE_IDENTIFIER} { yylval.sValue = strdup(yytext); return UID; }
{LOWERCASE_IDENTIFIER} { yylval.sValue = strdup(yytext); return LID;}

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
"%"                   { return MOD; }
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
