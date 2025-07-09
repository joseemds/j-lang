%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>
#include <printers.h>
#include <transpiler.h>
#include "rational.h"
#include <semantic.h>

int yylex(void);
int yyerror(const char *s);
extern int yylineno;
extern char *yytext;
StmtList *root = NULL;
%}

%code requires {
	#include <ast.h>
}

%union {
	int    iValue;
  float  fValue;
  rational rValue;
	char *cValue;
	char *sValue;
	ASTStmt *stmtValue;
	ASTExpr *exprValue;
	ASTType *typeValue;
	StmtList *stmtList;
  ExprList *exprList;
  StmtFuncParams *funcParams;
	StmtStructField *structFields;
	StructFieldAssign *structAssign;
};

%token <sValue> UID LID STRING_LIT PRIM_TYPE
%token <cValue> CHAR_LIT
%token <iValue> NUMBER
%token <fValue> FLOAT
%token <rValue> FRAC_LIT
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM BREAK CONTINUE
%nonassoc EQUAL CMP LEQ LT GEQ GT NEQ TRUE FALSE
%token PLUS MINUS TIMES DIVIDE MOD AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT
%define parse.error verbose
%locations
%nonassoc FRAC_CONS

%left TIMES DIVIDE AND
%left PLUS MINUS OR


%type <structAssign> struct_assign struct_assign_opt
%type <exprValue> atomic_expr arith_expr expr func_call attr_access array_access array_notation struct_cons
%type <stmtValue> stmt type_decl func_decl variable_stmt for_stmt while_stmt if_stmt return_stmt assign val_decl
%type <stmtList> program stmt_list stmts
%type <typeValue> usable_type array_type
%type <exprList> idents expr_list expr_list_opt arg_list arg_list_opt enum_values
%type <funcParams> func_param func_params func_params_opt
%type <structFields> type_struct struct_fields

%%

program: stmt_list {root = $1;}

stmt_list: %empty {$$ = NULL;} | stmts {$$ = $1;}

stmts: stmt {$$ = mk_stmt_list($1);} 
     | stmts stmt {append_stmt_list($1, $2); $$ = $1;}

stmt: 
    type_decl  {$$ = $1;}
    | variable_stmt SEMICOLON {$$ = $1;}
    | func_decl  {$$ = $1;}
    | expr SEMICOLON {$$ = mk_expr_stmt(@1.first_line, @1.first_column, $1);}
    | return_stmt {$$ = $1;}
    | for_stmt { $$ = $1;}
    | while_stmt {$$ = $1;}
    | if_stmt {$$ = $1;}
    | BREAK SEMICOLON {$$ = mk_break_stmt(@1.first_line, @1.first_column);}
    | CONTINUE SEMICOLON {$$ = mk_continue_stmt(@1.first_line, @1.first_column);}

type_decl:
    TYPE UID[name] EQUAL usable_type[type] SEMICOLON
    {
        $$ = mk_type_alias(@1.first_line, @1.first_column, $name, $type);
    }
    | TYPE UID[name] EQUAL LBRACE type_struct[fields] RBRACE SEMICOLON
    {
        $$ = mk_type_struct(@1.first_line, @1.first_column, $name, $fields);
    }
    | TYPE UID[name] EQUAL ENUM LBRACE enum_values[vals] RBRACE SEMICOLON
    {
        $$ = mk_type_enum(@1.first_line, @1.first_column, $name, $vals);
    }

type_struct: %empty {$$ = NULL;}
           | struct_fields SEMICOLON type_struct {
							$1->next = $3;
							$$ = $1;
					 }

struct_fields: idents COLON usable_type {$$ = mk_struct_field($1, $3);}

enum_values: STRING_LIT {$$ = mk_expr_list(mk_string_lit(@1.first_line, @1.first_column, $1));}
           | enum_values COMMA STRING_LIT {
							append_expr_list($1, mk_string_lit(@3.first_line, @3.first_column, $3));
							$$ = $1;
					 }

array_type: LBRACKET array_type RBRACKET  {$$ = mk_type_array(@2.first_line, @2.first_column, $2);}
          | LBRACKET usable_type RBRACKET {$$ = $2;}

variable_stmt: assign {$$ = $1;}
             | val_decl {$$ = $1;}

assign: LID EQUAL expr {$$ = mk_assign_stmt(@1.first_line, @1.first_column, mk_ident(@1.first_line, @1.first_column, $1), $3);}
      | array_access EQUAL expr {$$ = mk_assign_stmt(@1.first_line, @1.first_column, $1, $3);}
      | attr_access EQUAL expr {$$ = mk_assign_stmt(@1.first_line, @1.first_column, $1, $3);}

array_access: atomic_expr LBRACKET arith_expr RBRACKET {mk_array_access_expr(@2.first_line, @2.first_column, $1, $3);}

val_decl: VAL idents COLON usable_type {$$ = mk_val_decl_stmt(@1.first_line, @1.first_column, $2, $4);} 
				| VAL idents[ids] COLON usable_type[typ] EQUAL expr_list[values] {$$ = mk_val_init_stmt(@1.first_line, @1.first_column, $ids, $typ, $values);}

idents: LID {$$ = mk_expr_list(mk_ident(@1.first_line, @1.first_column, $1));}
			| idents COMMA LID  {append_expr_list($1, mk_ident(@3.first_line, @3.first_column, $3)); $$ = $1; }

usable_type: UID {$$ = mk_type_ident(@1.first_line, @1.first_column, $1);}
           | PRIM_TYPE {$$ = mk_type_prim(@1.first_line, @1.first_column, $1);}
           | array_type {$$ = mk_type_array(@1.first_line, @1.first_column, $1);}


func_decl: FUNC LID[name] LPAREN func_params_opt[params] RPAREN COLON usable_type[return_typ] LBRACE stmt_list[body] RBRACE {
				 $$ = mk_func_decl_stmt(@1.first_line, @1.first_column, $name, $params, $return_typ, $body);
				 }

func_params_opt: %empty {$$ = NULL;}
							 | func_params {$$ = $1;}

func_params: func_param {$$ = $1;} 
					 | func_params COMMA func_param {append_func_params($1, $3); $$ = $1;}

func_param: idents COLON usable_type {
  $$ = mk_func_params($1, $3);
}

return_stmt: RETURN expr SEMICOLON {$$ = mk_return_stmt(@1.first_line, @1.first_column, $2);}

expr: arith_expr  {$$ = $1;}
    | expr MOD arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, MOD, $1, $3);}
    | expr OR arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, OR, $1, $3);}
    | expr AND arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, AND, $1, $3);}
		| expr LT arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, LT, $1, $3);}
		| expr LEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, LEQ, $1, $3);}
		| expr GT arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, GT, $1, $3);}
		| expr GEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, GEQ, $1, $3);}
    | expr CMP arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, CMP, $1, $3);}
    | expr NEQ arith_expr {$$ = mk_binary_op(@1.first_line, @1.first_column, NEQ, $1, $3);}

arith_expr: atomic_expr                      {$$ = $1;}
          | arith_expr PLUS atomic_expr      {$$ = mk_binary_op(@1.first_line, @1.first_column, PLUS, $1, $3);}
          | arith_expr MINUS atomic_expr     {$$ = mk_binary_op(@1.first_line, @1.first_column, MINUS, $1, $3);}
          | arith_expr TIMES atomic_expr     {$$ = mk_binary_op(@1.first_line, @1.first_column, TIMES, $1, $3);}
          | arith_expr DIVIDE atomic_expr    {$$ = mk_binary_op(@1.first_line, @1.first_column, DIVIDE, $1, $3);}
          | arith_expr FRAC_CONS atomic_expr {$$ = mk_frac_cons_exprs(@1.first_line, @1.first_column, $1, $3);} 
          | NOT atomic_expr                  {$$ = mk_unary_op(@1.first_line, @1.first_column, NOT, $2);}
          | MINUS atomic_expr                {$$ = mk_unary_op(@1.first_line, @1.first_column, MINUS, $2);}


atomic_expr: NUMBER {$$ = mk_int_lit(@1.first_line, @1.first_column, $1);}
           | FLOAT {$$ = mk_float_lit(@1.first_line, @1.first_column, $1);}
		       | STRING_LIT  {$$ = mk_string_lit(@1.first_line, @1.first_column, $1);}
           | TRUE  {$$ = mk_bool_lit(@1.first_line, @1.first_column, 1);}
           | FALSE {$$ = mk_bool_lit(@1.first_line, @1.first_column, 0);}
           | LID {$$ = mk_ident(@1.first_line, @1.first_column, $1);}
           | CHAR_LIT {$$ = mk_char_lit(@1.first_line, @1.first_column, $1);}
           | FRAC_LIT {$$ = mk_frac_cons_rational(@1.first_line, @1.first_column, $1);}
           | func_call {$$ = $1;}
           | LPAREN expr RPAREN {$$ = $2;}
           | array_access {$$ = $1;}
           | attr_access {$$ = $1;}
           | array_notation {$$ = $1;}
           | struct_cons {$$ = $1;}

attr_access: atomic_expr DOT LID {$$ = mk_attr_access_expr(@2.first_line, @2.first_column, $1, $3);}

array_notation: LBRACKET expr_list_opt RBRACKET {$$ = mk_array_lit_expr(@1.first_line, @1.first_column, $2);}

expr_list_opt: %empty {$$ = NULL;}
             | expr_list {$$ = $1;}

expr_list: expr {$$ = mk_expr_list($1);}
         | expr_list COMMA expr {append_expr_list($1, $3); $$ = $1;}

struct_cons: usable_type LBRACE struct_assign_opt RBRACE {$$ = mk_struct_cons_expr(@1.first_line, @1.first_column, $1, $3);}

struct_assign_opt: %empty {$$ = NULL;}
             | struct_assign {$$ = $1;}

struct_assign: LID COLON expr {$$ = mk_struct_field_assign($1, $3);} // lhs tem que ser expressao
             | struct_assign COMMA LID COLON expr {} // Isso ta certo?

for_stmt: FOR LPAREN variable_stmt[var] SEMICOLON expr[cond] SEMICOLON assign[inc] RPAREN LBRACE stmt_list[body] RBRACE {$$ = mk_for_stmt(@1.first_line, @1.first_column, $var, $cond, $inc, $body);}
        | FOR LPAREN LID[var] SEMICOLON expr[cond] SEMICOLON assign[inc] RPAREN LBRACE stmt_list[body] RBRACE { 
            ASTExpr *ident = mk_ident(@1.first_line, @1.first_column, $var);
            $$ = mk_for_stmt(@1.first_line, @1.first_column, mk_expr_stmt(@var.first_line, @var.first_column, ident), $cond, $inc, $body);
          }

func_call: LID LPAREN arg_list_opt RPAREN {
				   $$ = mk_func_call(@1.first_line, @1.first_column, $1, $3);
				 }

arg_list_opt: %empty {$$ = NULL;}
            | arg_list {$$ = $1;}

arg_list: expr  {$$ = mk_expr_list($1);}
        | arg_list COMMA expr {append_expr_list($1, $3); $$ = $1;}

if_stmt: IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE { $$ = mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, NULL); }
       | IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE ELSE LBRACE stmt_list[else_] RBRACE {$$ = mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, $else_); }

while_stmt: WHILE LPAREN expr[cond] RPAREN LBRACE stmt_list[body] RBRACE {$$ = mk_while_stmt(@1.first_line, @1.first_column, $cond, $body);}

%%

int main (void) {
	int status = yyparse ();
	// printf("Parsed with status: %d\n", status);
	if (root){
		transpile(root);
		printf("checagem %d\n", check_program(root));
 		// transpile_stmt_list(root);
 		transpile(root);
	}
	return status;
}

int yyerror (const char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
