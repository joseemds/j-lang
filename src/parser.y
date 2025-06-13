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
program: imports type_decls val_decls funcs {}

imports: | imports import {}

import: IMPORT LID SEMICOLON {}

funcs: | func funcs {}

func: FUNC LID LPAREN RPAREN COLON UID LBRACE stmts RBRACE {}

stmts: stmt | stmts stmt {}

stmt: func_call SEMICOLON {}

func_call: LID LPAREN func_call_rest {}

func_call_rest: RPAREN | arg_list RPAREN {}

arg_list: arg | arg_list COMMA arg {}

arg: STRING_LIT | LID {}

type_decls: | type_decls type_decl {}

val_decls : | val_decls val_decl

val_decl: VAL idents COLON UID SEMICOLON {}

type_decl: TYPE UID EQUAL type_constr {}

type_constr: PRIM_TYPE SEMICOLON
					| UID SEMICOLON
					| LBRACE RBRACE {}

idents: LID | idents COMMA LID  {}

%%

int main (void) {
	return yyparse ( );
}

int yyerror (char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
