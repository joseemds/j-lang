%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
int yyerror(const char *s);
extern int yylineno;
extern char * yytext;
%}

%code requires {
	#include <ast.h>
}

%union {
	int    iValue;
  float  fValue;
	char   cValue;
	char * sValue;
	ASTStmt* stmtValue;
	ASTExpr* exprValue;
	StmtList* stmtList;
};

%token <sValue> UID LID STRING_LIT PRIM_TYPE
%token <iValue> NUMBER
%token <fValue> FLOAT
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM BREAK CONTINUE
%nonassoc EQUAL CMP LEQ LT GEQ GT NEQ TRUE FALSE
%token PLUS MINUS TIMES DIVIDE MOD AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT
%define parse.error detailed
%locations

%left TIMES DIVIDE AND
%left PLUS MINUS OR


%type <exprValue> atomic_expr arith_expr expr
%type <stmtList> program stmt_list stmts


%%

program: stmt_list {$$ = $1;}

stmt_list: %empty {$$ = NULL;} | stmts {$$ = $1;} // revert this 

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

expr: arith_expr  {$$ = $1;}
    | expr OR arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, OR, $1, $3);}
    | expr AND arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, AND, $1, $3);}
		| expr LT arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, LT, $1, $3);}
		| expr LEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, LEQ, $1, $3);}
		| expr GT arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, GT, $1, $3);}
		| expr GEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, GEQ, $1, $3);}
    | expr CMP arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, CMP, $1, $3);}
    | expr NEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, NEQ, $1, $3);}

arith_expr: atomic_expr                   {$$ = $1;}
          | arith_expr PLUS atomic_expr   {$$ = mk_binary_op(@1.first_line, @1.first_column, PLUS, $1, $3);}
          | arith_expr MINUS atomic_expr  {$$ = mk_binary_op(@1.first_line, @1.first_column, MINUS, $1, $3);}
          | arith_expr TIMES atomic_expr  {$$ = mk_binary_op(@1.first_line, @1.first_column, TIMES, $1, $3);}
          | arith_expr DIVIDE atomic_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, DIVIDE, $1, $3);}
          | NOT atomic_expr               {}
          | MINUS atomic_expr             {}

atomic_expr: NUMBER {$$ = mk_int_lit(@1.first_line, @1.first_column, $1);}
           | FLOAT {$$ = mk_float_lit(@1.first_line, @1.first_column, $1);}
		       | STRING_LIT  {$$ = mk_string_lit(@1.first_line, @1.first_column, $1);}
           | TRUE  {$$ = mk_bool_lit(@1.first_line, @1.first_column, 1);}
           | FALSE {$$ = mk_bool_lit(@1.first_line, @1.first_column, 0);}
           | LID {$$ = mk_ident(@1.first_line, @1.first_column, $1);}
           | func_call {}
           | LPAREN expr RPAREN {$$ = $2;}
           | array_access {}
           | attr_access {}
           | array_notation {}
           | struct_cons {}

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

if_stmt: IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE { mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, NULL); }
       | IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE ELSE LBRACE stmt_list[else_] RBRACE {mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, $else_); }

while_stmt: WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {}

%%

int main (void) {
	return yyparse ( );
}

int yyerror (const char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
