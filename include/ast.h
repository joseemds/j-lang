#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rational.h"

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
	EXPR_IDENT
  // EXPR_RATIONAL_LITERAL ?
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
	};
} ASTExpr;

typedef enum StmtKind {
    STMT_TYPE_DECL,
		STMT_VARIABLE,
		STMT_FUNC_DECL,
		STMT_RETURN,
		STMT_FOR,
		STMT_WHILE,
		STMT_IF,
		STMT_BREAK,
		STMT_CONTINUE,
		STMT_EXPR,
} StmtKind;

typedef struct StmtList {
    ASTStmt* item;
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

// typedef struct StmtImport {
  // não vamos trabalhar com imports
// } StmtImport;

typedef struct StmtTypeDecl {
  // separar em casos dos construtores?
} StmtTypeDecl;

typedef struct StmtFuncParam {
	ExprIdent* ident; // lista 
	ASTType* type;
} StmtFuncParam;

typedef struct StmtFuncDecl {
  // guardar o tipo de retorno?
  StmtFuncParam* params;
  StmtList* body;
} StmtFuncDecl;

typedef struct StmtFuncCall {

} StmtFuncCall;

typedef struct StmtValDecl {
  // guardar os tipos?
  ExprIdent** idents; // array de ponteiros porque várias variáveis podem ser declaradas por linha? (relatado?)
  ASTExpr** exprs; // possíveis inicializações das variáveis
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
		StmtWhile while_stmt;
		StmtFor for_stmt;
		StmtReturn return_stmt;

		StmtTypeDecl type_decl;
		StmtFuncDecl func_decl;
		StmtValDecl val_decl;
		StmtFuncParam func_param;
		StmtStructField struct_field;

		StmtAssign assign;
		StmtFuncCall func_call;
	};
};


ASTStmt* mk_if_stmt(int line, int col, ASTExpr* cond, StmtList* then, StmtList* else_);
ASTStmt* mk_while_stmt(int line, int col);
ASTStmt* mk_for_stmt(int line, int col);
ASTStmt* mk_return_stmt(int line, int col);
ASTStmt* mk_type_decl_stmt(int line, int col);
ASTStmt* mk_func_decl_stmt(int line, int col);
ASTStmt* mk_func_param_stmt(int line, int col);
ASTStmt* mk_val_decl_stmt(int line, int col);
ASTStmt* mk_assign_stmt(int line, int col);

ASTExpr*  mk_binary_op(int line, int col, int op, ASTExpr* left, ASTExpr* right);
ASTExpr*  mk_ident(int line, int col, char* ident);
ASTExpr*  mk_int_lit(int line, int col, int value);
ASTExpr*  mk_float_lit(int line, int col, float value);
ASTExpr*  mk_string_lit(int line, int col, char* value);
ASTExpr*  mk_bool_lit(int line, int col, int value);

ASTType* mk_type_ident(int line, int col, char* type_name);
ASTType* mk_type_prim(int line, int col, char* type_name);
ASTType* mk_type_array(int line, int col, ASTType* type_);
