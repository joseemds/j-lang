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
%define parse.error detailed

%%
program: stmt_list {}

stmt_list: %empty | stmts


import: IMPORT LID SEMICOLON {}

func: FUNC LID LPAREN RPAREN COLON UID LBRACE stmt_list RBRACE {}

stmts: stmt | stmts stmt {}

stmt: import | type_decl | variable_stmt | func | expr

expr: func_call

variable_stmt: assign | val_decl

func_call: LID LPAREN func_call_rest {}

func_call_rest: RPAREN | arg_list RPAREN {}

arg_list: arg | arg_list COMMA arg {}

arg: STRING_LIT | LID {}

val_decl: VAL idents COLON UID val_initialization_opt SEMICOLON {}

val_initialization_opt: %empty | val_initialization {}

val_initialization: EQUAL NUMBER {}

assign: LID EQUAL NUMBER SEMICOLON {}

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
