#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rational.h"

typedef struct IdentList IdentList;
typedef struct ExprList ExprList;
typedef struct ASTNode ASTNode;
typedef struct ASTStmt ASTStmt;
typedef struct ASTExpr ASTExpr;
typedef struct ASTType ASTType;

typedef enum ASTTypeKind {
	TYPE_IDENT,
	TYPE_PRIM,
	TYPE_ARRAY
} ASTTypeKind;

typedef struct TypeIdent {char* name;} TypeIdent;
typedef struct TypePrim {char* name;} TypePrim;
typedef struct TypeArray {ASTType* inner_type;} TypeArray;


typedef struct ExprList {
	ASTExpr* expr;
	ExprList* next;
} ExprList;


typedef struct ASTType {
	int line;
	int col;
	ASTTypeKind kind;
	union {
		TypeIdent* ident;
		TypePrim* prim;
		TypeArray* array;
	};
} ASTType;

typedef enum ExprKind {
	EXPR_BINARY,
	EXPR_FUNC_CALL,
	EXPR_INT_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_ARRAY_LIT,
	EXPR_ARRAY_ACCESS,
	EXPR_ATTR_ACCESS,
	EXPR_CONS,
	EXPR_STRING_LITERAL,
	EXPR_BOOL_LITERAL,
	EXPR_IDENT,
  EXPR_RATIONAL_LITERAL
} ExprKind;

typedef struct ExprBinaryOp {
	int op;
	ASTExpr* left;
	ASTExpr* right;
} ExprBinaryOp;

typedef struct ExprIdent { char* name; } ExprIdent;
typedef struct ExprIntLiteral { int value; } ExprIntLiteral;
typedef struct ExprFloatLiteral { float value; } ExprFloatLiteral;
typedef struct ExprStringLiteral { char* value; } ExprStringLiteral;
typedef struct ExprBoolLiteral { int value; } ExprBoolLiteral;
typedef struct ExprRationalLiteral { rational value; } ExprRationalLiteral;

typedef struct IdentList {
	ExprIdent* ident;
	IdentList* next;
} IdentList;


typedef struct ASTExpr {
	ExprKind kind;
	int col;
	int line;
	union {
		ExprBinaryOp* binary_op;
		ExprIdent* ident;
		ExprIntLiteral* int_lit;
		ExprFloatLiteral* float_lit;
		ExprStringLiteral* string_lit;
		ExprBoolLiteral* bool_lit;
    ExprRationalLiteral* rational_lit;
	};
} ASTExpr;

typedef enum StmtKind {
    STMT_TYPE_DECL,
		STMT_VARIABLE,
		STMT_FUNC_DECL,
		STMT_FUNC_PARAM,
		STMT_RETURN,
		STMT_FOR,
		STMT_WHILE,
		STMT_IF,
		STMT_BREAK,
		STMT_CONTINUE,
		STMT_EXPR,
} StmtKind;

typedef struct StmtList {
    ASTStmt* stmt;
    struct StmtList* next;
} StmtList;

typedef struct StmtWhile {
	ASTExpr* cond;
	StmtList* body;
} StmtWhile;

typedef struct StmtFor {
  ASTExpr* var;
  ASTExpr* cond;
  ASTExpr* inc;
  StmtList* body;
} StmtFor;

typedef struct StmtReturn {
	ASTExpr* expr;
} StmtReturn;

typedef struct StmtTypeDecl {
  // separar em casos dos construtores?
} StmtTypeDecl;

typedef struct StmtFuncParams StmtFuncParams;
typedef struct StmtFuncParams {
	ExprList* idents; 
	ASTType* type;
	StmtFuncParams* next;
} StmtFuncParams;

typedef struct StmtFuncDecl {
  StmtFuncParams* params;
  StmtList* body;
} StmtFuncDecl;

typedef struct StmtFuncCall {

} StmtFuncCall;

typedef struct StmtValDecl {
  // guardar os tipos?
  ExprIdent** idents; // array de ponteiros porque várias variáveis podem ser declaradas por linha? (relatado?)
  ASTExpr** exprs; // possíveis inicializações das variáveis | mudar pra lista ligada por eficiência de memória
} StmtValDecl;

typedef struct StmtExpr {
  ASTExpr* expr; //?
} StmtExpr;

typedef struct StmtAssign {
  ExprIdent* ident;
  ASTExpr* expr;
} StmtAssign;

typedef struct StmtStructField {
  
} StmtStructField;


typedef struct StmtIf {
    ASTExpr* condition;
    StmtList* then;
    StmtList* else_;
} StmtIf;

struct ASTStmt {
	StmtKind kind;
	int line;
	int col;
	union {
		StmtIf* if_stmt;
		StmtWhile* while_stmt;
		StmtFor* for_stmt;
		StmtReturn* return_stmt;

		StmtTypeDecl* type_decl;
		StmtFuncDecl* func_decl;
		StmtValDecl* val_decl;
		StmtStructField* struct_field;

		StmtAssign* assign;
		StmtFuncCall* func_call;
	};
};


ExprList* mk_expr_list(ASTExpr* Expr);
void append_expr_list(ExprList* list, ASTExpr* Expr);

StmtList* mk_stmt_list(ASTStmt* stmt);
void append_stmt_list(StmtList* list, ASTStmt* stmt);

StmtFuncParams* mk_func_params(ExprList* idents, ASTType* typ);
void append_func_params(StmtFuncParams* params, StmtFuncParams* next);

ASTStmt* mk_type_decl_stmt(int line, int col);
ASTStmt* mk_val_decl_stmt(int line, int col, ExprIdent** idents, ASTExpr** exprs);
ASTStmt* mk_func_decl_stmt(int line, int col);
ASTStmt* mk_assign_stmt(int line, int col);
ASTStmt* mk_return_stmt(int line, int col, ASTExpr* expr);
ASTStmt* mk_for_stmt(int line, int col);
ASTStmt* mk_while_stmt(int line, int col);
ASTStmt* mk_if_stmt(int line, int col, ASTExpr* cond, StmtList* then, StmtList* else_);
ASTStmt* mk_break_stmt(int line, int col);
ASTStmt* mk_continue_stmt(int line, int col);
ASTStmt* mk_expr_stmt(int line, int col, ASTExpr* expr);

ASTExpr*  mk_binary_op(int line, int col, int op, ASTExpr* left, ASTExpr* right);
ASTExpr*  mk_ident(int line, int col, char* ident);
ASTExpr*  mk_int_lit(int line, int col, int value);
ASTExpr*  mk_float_lit(int line, int col, float value);
ASTExpr*  mk_string_lit(int line, int col, char* value);
ASTExpr*  mk_bool_lit(int line, int col, int value);

ASTType* mk_type_ident(int line, int col, char* type_name);
ASTType* mk_type_prim(int line, int col, char* type_name);
ASTType* mk_type_array(int line, int col, ASTType* type_);
