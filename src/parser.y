%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>
#include <printers.h>

int yylex(void);
int yyerror(const char *s);
extern int yylineno;
extern char * yytext;
StmtList* root = NULL;
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
	ASTType* typeValue;
	StmtList* stmtList;
  ExprList* exprList;
  StmtFuncParams* funcParams;
};

%token <sValue> UID LID STRING_LIT PRIM_TYPE
%token <iValue> NUMBER
%token <fValue> FLOAT
%token TYPE FUNC VAL IF ELSE WHILE FOR RETURN IMPORT ENUM BREAK CONTINUE
%nonassoc EQUAL CMP LEQ LT GEQ GT NEQ TRUE FALSE
%token PLUS MINUS TIMES DIVIDE MOD AND OR NOT
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET COLON SEMICOLON COMMA DOT
%define parse.error verbose
%locations

%left TIMES DIVIDE AND
%left PLUS MINUS OR


%type <exprValue> atomic_expr arith_expr expr
%type <stmtValue> stmt type_decl func_decl variable_stmt for_stmt while_stmt if_stmt return_stmt
%type <stmtList> program stmt_list stmts
%type <typeValue> usable_type array_type
%type <exprList> idents
%type <funcParams> func_param func_params func_params_opt

%%

program: stmt_list {root = $1;}

stmt_list: %empty {$$ = NULL;} | stmts {$$ = $1;}

stmts: stmt {$$ = mk_stmt_list($1);} 
     | stmts stmt {append_stmt_list($1, $2); $$ = $1;}

stmt: 
    type_decl 
    | variable_stmt SEMICOLON
    | func_decl  {$$ = $1;}
    | expr SEMICOLON {$$ = mk_expr_stmt(@1.first_line, @1.first_column, $1);}
    | return_stmt 
    | for_stmt
    | while_stmt
    | if_stmt {$$ = $1;}
    | BREAK SEMICOLON {$$ = mk_break_stmt(@1.first_line, @1.first_column);}
    | CONTINUE SEMICOLON {$$ = mk_continue_stmt(@1.first_line, @1.first_column);}

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

array_type: LBRACKET array_type RBRACKET  {$$ = mk_type_array(@2.first_line, @2.first_column, $2);}
          | LBRACKET usable_type RBRACKET {$$ = $2;}

variable_stmt: assign
             | val_decl

assign: LID EQUAL expr {}
      | array_access EQUAL expr {}
      | attr_access EQUAL expr {}

array_access: atomic_expr LBRACKET arith_expr RBRACKET {} // possível erro com o uso de atomic_expr

val_decl: VAL idents COLON usable_type val_initialization_opt {}

idents: LID {$$ = mk_expr_list(mk_ident(@1.first_line, @1.first_column, $1));}
			| idents COMMA LID  {append_expr_list($1, mk_ident(@3.first_line, @3.first_column, $3)); $$ = $1; }

usable_type: UID {$$ = mk_type_ident(@1.first_line, @1.first_column, $1);}
           | PRIM_TYPE {$$ = mk_type_prim(@1.first_line, @1.first_column, $1);}
           | array_type {$$ = mk_type_array(@1.first_line, @1.first_column, $1);}

val_initialization_opt: %empty | val_initialization {}

val_initialization: EQUAL expr_list {}

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

if_stmt: IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE { $$ = mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, NULL); }
       | IF LPAREN expr[cond] RPAREN LBRACE stmt_list[then] RBRACE ELSE LBRACE stmt_list[else_] RBRACE {$$ = mk_if_stmt(@1.first_line, @1.first_column, $cond, $then, $else_); }

while_stmt: WHILE LPAREN expr RPAREN LBRACE stmt_list RBRACE {}

%%

int main (void) {
	int status = yyparse ();
	printf("Parsed with status: %d\n", status);
	// if(root){
	// 	pp_stmt_list(root);
	// }
	return status;
}

int yyerror (const char *msg) {
	fprintf (stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
	return 0;
}
