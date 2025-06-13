%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
int yyerror(char *s);
extern int yylineno;
extern char * yytext;
%}

%union {
	int    iValue;
	char   cValue;
	char * sValue;
	};

%token <sValue> UID LID STRING_LIT PRIM_TYPE
%token <iValue> NUMBER
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM
%token EQUAL CMP LEQ LT GEQ GT NEQ
%token PLUS MINUS TIMES DIVIDE AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT


%%
program: type_decls

type_decls: | type_decl type_decls

type_decl: TYPE UID EQUAL type_constr

type_constr: PRIM_TYPE SEMICOLON
					| UID SEMICOLON
					| LBRACE RBRACE

%%

int main (void) {
	return yyparse ( );
}

int yyerror (char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
