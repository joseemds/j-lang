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
  float  fValue;
	char   cValue;
	char * sValue;
	};

%token <sValue> UID LID STRING_LIT PRIM_TYPE
%token <iValue> NUMBER
%token <fValue> FLOAT
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM BREAK CONTINUE
%nonassoc EQUAL CMP LEQ LT GEQ GT NEQ TRUE FALSE
%token PLUS MINUS TIMES DIVIDE MOD AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT
%define parse.error detailed

%left TIMES DIVIDE AND
%left PLUS MINUS OR

%%

program: stmt_list {}

stmt_list: %empty | stmts

stmts: stmt | stmts stmt {}

stmt: import 
    | type_decl 
    | variable_stmt SEMICOLON
    | func_decl 
    | expr SEMICOLON 
    | return_stmt 
    | for_stmt
    | while_stmt
    | if_stmt
    | BREAK SEMICOLON
    | CONTINUE SEMICOLON

import: IMPORT LID SEMICOLON {}

type_decl: TYPE UID EQUAL type_constr {}

type_constr: usable_type SEMICOLON
					 | LBRACE type_struct RBRACE {}
           | type_enum SEMICOLON

type_struct: %empty 
           | struct_fields SEMICOLON type_struct // fazer recursão esquerda

struct_fields: idents COLON usable_type val_initialization_opt

type_enum: ENUM LBRACE enum_values RBRACE

enum_values: STRING_LIT
           | enum_values COMMA STRING_LIT 

array_type: LBRACKET array_type RBRACKET
          | LBRACKET usable_type RBRACKET

variable_stmt: assign
             | val_decl

assign: LID EQUAL expr {}
      | array_access EQUAL expr {}
      | attr_access EQUAL expr {}

array_access: atomic_expr LBRACKET arith_expr RBRACKET {} // possível erro com o uso de atomic_expr

val_decl: VAL idents COLON usable_type val_initialization_opt {}

idents: LID | idents COMMA LID  {}

usable_type: UID
           | PRIM_TYPE
           | array_type

val_initialization_opt: %empty | val_initialization {}

val_initialization: EQUAL expr_list {}

func_decl: FUNC LID LPAREN func_params_opt RPAREN COLON usable_type LBRACE stmt_list RBRACE {}

func_params_opt: %empty | func_params

func_params: func_param | func_params COMMA func_param

func_param: idents COLON usable_type

return_stmt: RETURN expr SEMICOLON {}

expr: arith_expr 
    | expr OR arith_expr
    | expr AND arith_expr
		| expr LT arith_expr
		| expr LEQ arith_expr
		| expr GT arith_expr
		| expr GEQ arith_expr
    | expr CMP arith_expr
    | expr NEQ arith_expr

arith_expr: atomic_expr
          | arith_expr PLUS atomic_expr   {}
          | arith_expr MINUS atomic_expr  {}
          | arith_expr TIMES atomic_expr  {}
          | arith_expr DIVIDE atomic_expr {}
          | NOT atomic_expr               {}
          | MINUS atomic_expr             {}

atomic_expr: NUMBER
           | FLOAT
		       | STRING_LIT
           | TRUE
           | FALSE
           | LID
           | func_call
           | LPAREN expr RPAREN
           | array_access
           | attr_access
           | array_notation
           | struct_cons

attr_access: atomic_expr DOT LID {} // possível erro com o uso de atomic_expr

array_notation: LBRACKET expr_list_opt RBRACKET {}

expr_list_opt: %empty
             | expr_list {}

expr_list: expr {}
         | expr_list COMMA expr {}

struct_cons: usable_type LBRACE struct_assign_opt RBRACE

struct_assign_opt: %empty
             | struct_assign

struct_assign: LID COLON expr
             | struct_assign COMMA LID COLON expr {}

for_stmt: FOR LPAREN variable_stmt SEMICOLON expr SEMICOLON assign RPAREN LBRACE stmt_list RBRACE {}
        | FOR LPAREN LID SEMICOLON expr SEMICOLON assign RPAREN LBRACE stmt_list RBRACE {}

func_call: LID LPAREN arg_list_opt RPAREN {}

arg_list_opt: %empty
            | arg_list {}

arg_list: expr 
        | arg_list COMMA expr {}

if_stmt: IF LPAREN expr RPAREN LBRACE stmt_list RBRACE {}
       | IF LPAREN expr RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE {}

while_stmt: WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {}

%%

int main (void) {
	return yyparse ( );
}

int yyerror (const char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
