%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
int yyerror(const char *s);
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
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM BREAK CONTINUE
%nonassoc EQUAL CMP LEQ LT GEQ GT NEQ TRUE FALSE
%token PLUS MINUS TIMES DIVIDE AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT
%define parse.error detailed

%left TIMES DIVIDE AND
%left PLUS MINUS OR

%%

program: stmt_list {}

stmt_list: %empty | stmts // não é melhor só colocar uma regra vazia em stmts?

stmts: stmt | stmts stmt {}

stmt: import 
    | type_decl 
    | variable_stmt 
    | func 
    | expr SEMICOLON 
    | return_stmt 
    | for_stmt 
    | if_stmt
    | while_stmt
    | BREAK
    | CONTINUE

import: IMPORT LID SEMICOLON {}

type_decl: TYPE UID EQUAL type_constr {}

type_constr: PRIM_TYPE SEMICOLON
					 | UID SEMICOLON
					 | LBRACE RBRACE {} // faltando algo entre as chaves??

variable_stmt: assign | val_decl

assign: LID EQUAL expr SEMICOLON {}

val_decl: VAL idents COLON UID val_initialization_opt SEMICOLON {} // e o PRIM_TYPE? Essa regra não permite "val w = 10,y,z = w : Int;", apenas a inicialização da última variável declarada (ou de todas?)

idents: LID | idents COMMA LID  {}

val_initialization_opt: %empty | val_initialization {}

val_initialization: EQUAL expr {}

func: FUNC LID LPAREN func_params_opt RPAREN COLON UID LBRACE stmt_list RBRACE {}

func_params_opt: %empty | func_params

func_params: func_param | func_params COMMA func_param

func_param: idents COLON UID

return_stmt: RETURN expr SEMICOLON

expr: arith_expr 
    | expr OR expr // ambiguidade da recursão? 
    | expr AND expr // ambiguidade da recursão?
		| expr LT expr // ambiguidade da recursão?
		| expr LEQ expr // ambiguidade da recursão?
		| expr GT expr // ambiguidade da recursão?
		| expr GEQ expr // ambiguidade da recursão?
    | arith_expr CMP arith_expr // ambiguidade da recursão? posição errada?
    | arith_expr NEQ arith_expr // ambiguidade da recursão? posição errada?

// arith_expr:
//     atomic_expr
//     | arith_expr PLUS arith_expr  {} // ambiguidade da recursão?
//     | arith_expr MINUS arith_expr {} // ambiguidade da recursão?
//     | arith_expr TIMES arith_expr {} // ambiguidade da recursão?
//     | arith_expr DIVIDE arith_expr{} // ambiguidade da recursão?
//     | NOT atomic_expr            {}
//     | MINUS atomic_expr {}
//     ;

arith_expr: atomic_expr
          | arith_expr PLUS atomic_expr   {}
          | arith_expr MINUS atomic_expr  {}
          | arith_expr TIMES atomic_expr  {}
          | arith_expr DIVIDE atomic_expr {}
          | NOT atomic_expr               {}
          | MINUS atomic_expr             {}

atomic_expr: NUMBER
		       | STRING_LIT
           | TRUE
           | FALSE
           | LID
           | func_call
           | LPAREN expr RPAREN

for_stmt: FOR LPAREN variable_stmt expr SEMICOLON variable_stmt RPAREN LBRACE stmt_list RBRACE {} // todas as variáveis de controle de for vão ter o escopo local?

func_call: LID LPAREN arg_list_opt RPAREN {}

arg_list_opt: %empty | arg_list {}

arg_list: expr | arg_list COMMA expr {}

if_stmt: IF LPAREN expr RPAREN LBRACE stmt_list RBRACE
       | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE

while_stmt: WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE

%%

int main (void) {
	return yyparse ( );
}

int yyerror (const char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
