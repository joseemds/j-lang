#pragma once

#include "rational.h"

typedef struct IdentList IdentList;
typedef struct ExprList ExprList;
typedef struct ASTNode ASTNode;
typedef struct ASTStmt ASTStmt;
typedef struct ASTExpr ASTExpr;
typedef struct ASTType ASTType;

typedef enum ASTTypeKind { TYPE_IDENT, TYPE_PRIM, TYPE_ARRAY } ASTTypeKind;

typedef struct TypeIdent {
  char *name;
} TypeIdent;

typedef struct TypePrim {
  char *name;
} TypePrim;

typedef struct TypeArray {
  ASTType *inner_type;
} TypeArray;

typedef struct ExprList {
  ASTExpr *expr;
  ASTType *inferred_type; // Feito na analise semantica
  ExprList *next;
} ExprList;

typedef struct ASTType {
  int line;
  int col;
  ASTTypeKind kind;
  union {
    TypeIdent *ident;
    TypePrim *prim;
    TypeArray *array;
  };
} ASTType;

typedef enum ExprKind {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_FUNC_CALL,
  EXPR_INT_LITERAL,
  EXPR_FLOAT_LITERAL,
  EXPR_ARRAY_LIT,
  EXPR_ARRAY_ACCESS,
  EXPR_ATTR_ACCESS,
  EXPR_STRUCT_CONS,
  EXPR_STRING_LITERAL,
  EXPR_BOOL_LITERAL,
  EXPR_IDENT,
  // EXPR_RATIONAL_LITERAL,
  EXPR_FRAC_CONS,
  EXPR_CHAR_LITERAL
} ExprKind;

typedef struct ExprFuncCall {
  char *func_name;
  ExprList *params;
} ExprFuncCall;

typedef struct ExprBinaryOp {
  int op;
  ASTExpr *left;
  ASTExpr *right;
} ExprBinaryOp;

typedef struct ExprUnaryOp {
  int op;
  ASTExpr *operand;
} ExprUnaryOp;

typedef struct ExprIdent {
  char *name;
} ExprIdent;

typedef struct ExprIntLiteral {
  int value;
} ExprIntLiteral;

typedef struct ExprFloatLiteral {
  float value;
} ExprFloatLiteral;

typedef struct ExprStringLiteral {
  char *value;
} ExprStringLiteral;

typedef struct ExprBoolLiteral {
  int value;
} ExprBoolLiteral;

// typedef struct ExprRationalLiteral {
//   rational value;
// } ExprRationalLiteral;

typedef enum FracKind { FRAC_RATIONAL, FRAC_EXPRS } FracKind;

typedef struct ExprFracCons {
  FracKind kind;
  union {
    rational rational_lit;
    struct {
      ASTExpr *top;
      ASTExpr *bot;
    } exprs;
  };
} ExprFracCons;

typedef struct ExprCharLiteral {
  char *value;
} ExprCharLiteral;

typedef struct ExprArrayAccess {
  ASTExpr *base;
  ASTExpr *index;
} ExprArrayAccess;

typedef struct ExprAttrAccess {
  ASTExpr *base;
  char *attribute;
} ExprAttrAccess;

typedef struct ExprArrayLit {
  ExprList *elements;
} ExprArrayLit;

typedef struct StructFieldAssign StructFieldAssign;
typedef struct StructFieldAssign {
  char *name;
  ASTExpr *value;
  StructFieldAssign *next;
} StructFieldAssign;

typedef struct ExprStructCons {
  ASTType *type;
  StructFieldAssign *fields;
} ExprStructCons;

typedef struct IdentList {
  ExprIdent *ident;
  IdentList *next;
} IdentList;

typedef struct ASTExpr {
  ExprKind kind;
  ASTType *inferred_type; // Feito na analise semantica
  int col;
  int line;
  union {
    ExprBinaryOp *binary_op;
    ExprUnaryOp *unary_op;
    ExprIdent *ident;
    ExprIntLiteral *int_lit;
    ExprFloatLiteral *float_lit;
    ExprStringLiteral *string_lit;
    ExprBoolLiteral *bool_lit;
    ExprFracCons *frac_cons;
    ExprCharLiteral *char_lit;
    ExprFuncCall *func_call;
    ExprArrayLit *array_lit;
    ExprAttrAccess *attr_access;
    ExprArrayAccess *array_access;
    ExprStructCons *struct_cons;
  };
} ASTExpr;

typedef enum StmtKind {
  STMT_TYPE_DECL,
  STMT_VAR_DECL,
  STMT_VAR_INIT,
  STMT_FUNC_DECL,
  STMT_FUNC_PARAM,
  STMT_RETURN,
  STMT_FOR,
  STMT_WHILE,
  STMT_IF,
  STMT_BREAK,
  STMT_CONTINUE,
  STMT_EXPR,
  STMT_ASSIGN
} StmtKind;

typedef struct StmtList {
  ASTStmt *stmt;
  struct StmtList *next;
} StmtList;

typedef struct StmtWhile {
  ASTExpr *cond;
  StmtList *body;
} StmtWhile;

typedef struct StmtFor {
  ASTStmt *var;
  ASTExpr *cond;
  ASTStmt *inc;
  StmtList *body;
} StmtFor;

typedef struct StmtReturn {
  ASTExpr *expr;
} StmtReturn;

typedef struct StmtStructField StmtStructField;
typedef struct StmtStructField {
  ExprList *idents;
  ASTType *type;
  StmtStructField *next;
} StmtStructField;

typedef enum TypeDeclKind {
  TYPE_DECL_ALIAS,
  TYPE_DECL_STRUCT,
  TYPE_DECL_ENUM
} TypeDeclKind;

typedef struct StmtTypeDecl {
  char *name;
  TypeDeclKind decl_kind;
  union {
    ASTType *alias;
    struct {
      StmtStructField *fields;
    } struct_;
    struct {
      ExprList *values;
    } enum_;
  };
} StmtTypeDecl;

typedef struct StmtFuncParams StmtFuncParams;
typedef struct StmtFuncParams {
  ExprList *idents;
  ASTType *type;
  StmtFuncParams *next;
} StmtFuncParams;

typedef struct StmtFuncDecl {
  char *name;
  ASTType *return_typ;
  StmtFuncParams *params;
  StmtList *body;
} StmtFuncDecl;

typedef struct StmtFuncCall {

} StmtFuncCall;

typedef struct StmtValDecl {
  ASTType *type;
  ExprList *idents;
} StmtValDecl;

typedef struct StmtValInit {
  ASTType *type;
  ExprList *idents;
  ExprList *exprs;
} StmtValInit;

typedef struct StmtExpr {
  ASTExpr *expr;
} StmtExpr;

typedef struct StmtAssign {
  ASTExpr *ident;
  ASTExpr *expr;
} StmtAssign;

typedef struct StmtIf {
  ASTExpr *condition;
  StmtList *then;
  StmtList *else_;
} StmtIf;

struct ASTStmt {
  StmtKind kind;
  int line;
  int col;
  union {
    StmtIf *if_stmt;
    StmtWhile *while_stmt;
    StmtFor *for_stmt;
    StmtReturn *return_stmt;

    StmtTypeDecl *type_decl;
    StmtFuncDecl *func_decl;
    StmtValDecl *val_decl;
    StmtValInit *val_init;
    StmtStructField *struct_field;

    StmtAssign *assign;
    // StmtFuncCall *func_call; // depracted?
    StmtExpr *expr;
  };
};

ExprList *mk_expr_list(ASTExpr *Expr);
void append_expr_list(ExprList *list, ASTExpr *Expr);

StmtList *mk_stmt_list(ASTStmt *stmt);
void append_stmt_list(StmtList *list, ASTStmt *stmt);

StmtFuncParams *mk_func_params(ExprList *idents, ASTType *typ);
void append_func_params(StmtFuncParams *params, StmtFuncParams *next);

ASTStmt *mk_type_decl_stmt(int line, int col);
ASTStmt *mk_val_decl_stmt(int line, int col, ExprList *idents, ASTType *typ);
ASTStmt *mk_val_init_stmt(int line, int col, ExprList *idents, ASTType *typ,
                          ExprList *values);
ASTStmt *mk_func_decl_stmt(int line, int col, char *name,
                           StmtFuncParams *params, ASTType *return_typ,
                           StmtList *body);
ASTStmt *mk_assign_stmt(int line, int col, ASTExpr *ident, ASTExpr *value);
ASTStmt *mk_return_stmt(int line, int col, ASTExpr *expr);
ASTStmt *mk_for_stmt(int line, int col, ASTStmt *var, ASTExpr *cond,
                     ASTStmt *inc, StmtList *body);
ASTStmt *mk_while_stmt(int line, int col, ASTExpr *cond, StmtList *body);
ASTStmt *mk_if_stmt(int line, int col, ASTExpr *cond, StmtList *then,
                    StmtList *else_);
ASTStmt *mk_break_stmt(int line, int col);
ASTStmt *mk_continue_stmt(int line, int col);
ASTStmt *mk_expr_stmt(int line, int col, ASTExpr *expr);
// ASTStmt *mk_func_call_stmt(int line, int col, StmtFuncCall *func_call); //
// deprecated?

ASTExpr *mk_unary_op(int line, int col, int op, ASTExpr *operand);
ASTExpr *mk_binary_op(int line, int col, int op, ASTExpr *left, ASTExpr *right);
ASTExpr *mk_ident(int line, int col, char *ident);
ASTExpr *mk_int_lit(int line, int col, int value);
ASTExpr *mk_float_lit(int line, int col, float value);
ASTExpr *mk_string_lit(int line, int col, char *value);
ASTExpr *mk_bool_lit(int line, int col, int value);
ASTExpr *mk_frac_cons_rational(int line, int col, rational value);
ASTExpr *mk_frac_cons_exprs(int line, int col, ASTExpr *top, ASTExpr *bot);
ASTExpr *mk_char_lit(int line, int col, char *value);
ASTExpr *mk_func_call(int line, int col, char *funcName, ExprList *args);

ASTExpr *mk_array_access_expr(int line, int col, ASTExpr *base, ASTExpr *index);
ASTExpr *mk_attr_access_expr(int line, int col, ASTExpr *base, char *attribute);
ASTExpr *mk_array_lit_expr(int line, int col, ExprList *elements);
ASTExpr *mk_struct_cons_expr(int line, int col, ASTType *type,
                             StructFieldAssign *assignments);

StructFieldAssign *mk_struct_field_assign(char *name, ASTExpr *value);
void append_struct_field_assign(StructFieldAssign *fields,
                                StructFieldAssign *next);

ASTType *mk_type_ident(int line, int col, char *type_name);
ASTType *mk_type_prim(int line, int col, char *type_name);
ASTType *mk_type_array(int line, int col, ASTType *type_);
ASTStmt *mk_type_alias(int line, int col, char *name, ASTType *alias_for);
ASTStmt *mk_type_struct(int line, int col, char *name, StmtStructField *fields);
ASTStmt *mk_type_enum(int line, int col, char *name, ExprList *values);

StmtStructField *mk_struct_field(ExprList *idents, ASTType *type);
