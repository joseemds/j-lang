#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ExprList *mk_expr_list(ASTExpr *expr) {
  ExprList *list = (ExprList *)malloc(sizeof(ExprList));
  list->expr = expr;
  list->next = NULL;

  return list;
}

void append_expr_list(ExprList *list, ASTExpr *expr) {
  if (expr == NULL) {
    return;
  }
  ExprList *next = mk_expr_list(expr);
  while (list->next != NULL) {
    list = list->next;
  }
  list->next = next;
}

StmtList *mk_stmt_list(ASTStmt *stmt) {
  StmtList *list = (StmtList *)malloc(sizeof(StmtList));
  list->stmt = stmt;
  list->next = NULL;

  return list;
}

void append_stmt_list(StmtList *list, ASTStmt *stmt) {
  if (stmt == NULL)
    return;
  StmtList *next = mk_stmt_list(stmt);
  while (list->next != NULL) {
    list = list->next;
  }
  list->next = next;
}

ASTExpr *mk_ident(int line, int col, char *name) {
  ExprIdent *ident = (ExprIdent *)malloc(sizeof(ExprIdent));
  ident->name = strdup(name);

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_IDENT;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->ident = ident;

  return expr_node;
}

ASTExpr *mk_func_call(int line, int col, char *func_name, ExprList *params) {
  ExprFuncCall *func_call = (ExprFuncCall *)malloc(sizeof(ExprIdent));
  func_call->func_name = strdup(func_name);
  func_call->params = params;

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_FUNC_CALL;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->func_call = func_call;

  return expr_node;
}

ASTExpr *mk_bool_lit(int line, int col, int value) {
  ExprBoolLiteral *bool_lit =
      (ExprBoolLiteral *)malloc(sizeof(ExprBoolLiteral));
  if (!bool_lit) {
    fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
    exit(EXIT_FAILURE);
  }

  bool_lit->value = value;

  ASTExpr *node = (ASTExpr *)malloc(sizeof(ASTExpr));
  if (!node) {
    fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
    exit(EXIT_FAILURE);
  }

  node->kind = EXPR_BOOL_LITERAL;
  node->line = line;
  node->col = col;
  node->bool_lit = bool_lit;

  return node;
}

ASTExpr *mk_int_lit(int line, int col, int value) {
  ExprIntLiteral *int_lit = (ExprIntLiteral *)malloc(sizeof(ExprIntLiteral));
  int_lit->value = value;
  ASTExpr *node = (ASTExpr *)malloc(sizeof(ASTExpr));
  if (!node) {
    fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
    exit(EXIT_FAILURE);
  }

  node->kind = EXPR_INT_LITERAL;
  node->line = line;
  node->col = col;
  node->int_lit = int_lit;

  return node;
}

ASTExpr *mk_float_lit(int line, int col, float value) {
  ExprFloatLiteral *float_lit =
      (ExprFloatLiteral *)malloc(sizeof(ExprFloatLiteral));
  float_lit->value = value;

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_FLOAT_LITERAL;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->float_lit = float_lit;

  return expr_node;
}

ASTExpr *mk_string_lit(int line, int col, char *value) {
  ExprStringLiteral *string_lit =
      (ExprStringLiteral *)malloc(sizeof(ExprStringLiteral));
  string_lit->value = strdup(value);

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_STRING_LITERAL;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->string_lit = string_lit;

  return expr_node;
}

ASTExpr *mk_rational_lit(int line, int col, rational value) {
  ExprRationalLiteral *rational_lit =
      (ExprRationalLiteral *)malloc(sizeof(ExprRationalLiteral));
  rational_lit->value = value;

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_RATIONAL_LITERAL;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->rational_lit = rational_lit;

  return expr_node;
}

ASTExpr *mk_char_lit(int line, int col, char *value) {
  ExprCharLiteral *char_lit = (ExprCharLiteral *)malloc(sizeof(ExprCharLiteral));
  char_lit->value = value;

  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  expr_node->kind = EXPR_CHAR_LITERAL;
  expr_node->line = line;
  expr_node->col = col;
  expr_node->char_lit = char_lit;

  return expr_node;
}

ASTExpr *mk_binary_op(int line, int col, int op, ASTExpr *left,
                      ASTExpr *right) {
  ASTExpr *expr_node = (ASTExpr *)malloc(sizeof(ASTExpr));
  ExprBinaryOp *binary_op = (ExprBinaryOp *)malloc(sizeof(ExprBinaryOp));

  binary_op->left = left;
  binary_op->right = right;
  binary_op->op = op;

  expr_node->col = col;
  expr_node->line = line;
  expr_node->binary_op = binary_op;

  return expr_node;
};

ASTType *mk_type_ident(int line, int col, char *type_name) {
  ASTType *typ = (ASTType *)malloc(sizeof(ASTType));
  TypeIdent *ident = (TypeIdent *)malloc(sizeof(TypeIdent));
  ident->name = type_name;

  typ->line = line;
  typ->col = col;
  typ->ident = ident;
  typ->kind = TYPE_IDENT;
  return typ;
}

ASTType *mk_type_prim(int line, int col, char *type_name) {
  ASTType *typ = (ASTType *)malloc(sizeof(ASTType));
  TypePrim *prim = (TypePrim *)malloc(sizeof(TypePrim));
  prim->name = type_name;

  typ->line = line;
  typ->col = col;
  typ->prim = prim;
  typ->kind = TYPE_PRIM;
  return typ;
}

ASTType *mk_type_array(int line, int col, ASTType *inner_type) {
  ASTType *typ = (ASTType *)malloc(sizeof(ASTType));
  TypeArray *array = (TypeArray *)malloc(sizeof(TypeArray));
  array->inner_type = inner_type;

  typ->line = line;
  typ->col = col;
  typ->kind = TYPE_ARRAY;
  typ->array = array;
  return typ;
}

ASTStmt *mk_while_stmt(int line, int col, ASTExpr *cond, StmtList *body) {
  StmtWhile *while_stmt = (StmtWhile *)malloc(sizeof(StmtWhile));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  while_stmt->cond = cond;
  while_stmt->body = body;

  stmt->kind = STMT_WHILE;
  stmt->line = line;
  stmt->col = col;
  stmt->while_stmt = while_stmt;

  return stmt;
}

ASTStmt *mk_for_stmt(int line, int col, ASTStmt *var, ASTExpr *cond,
                     ASTStmt *inc, StmtList *body) {
  StmtFor *for_stmt = (StmtFor *)malloc(sizeof(StmtFor));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  for_stmt->cond = cond;
  for_stmt->var = var;
  for_stmt->inc = inc;

  stmt->kind = STMT_FOR;
  stmt->line = line;
  stmt->col = col;
  stmt->for_stmt = for_stmt;

  return stmt;
}

ASTStmt *mk_if_stmt(int line, int col, ASTExpr *cond, StmtList *then,
                    StmtList *else_) {
  StmtIf *if_stmt = (StmtIf *)malloc(sizeof(StmtIf));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  if_stmt->condition = cond;
  if_stmt->then = then;
  if_stmt->else_ = else_;

  stmt->kind = STMT_IF;
  stmt->line = line;
  stmt->col = col;
  stmt->if_stmt = if_stmt;

  return stmt;
}

ASTStmt *mk_break_stmt(int line, int col) {
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));
  stmt->kind = STMT_BREAK;
  stmt->line = line;
  stmt->col = col;
  return stmt;
}

ASTStmt *mk_continue_stmt(int line, int col) {
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));
  stmt->kind = STMT_CONTINUE;
  stmt->line = line;
  stmt->col = col;
  return stmt;
}

// ASTStmt* mk_type_decl_stmt(int line, int col, ... ){}

ASTStmt *mk_val_decl_stmt(int line, int col, ExprList *idents, ASTType *typ) {
  StmtValDecl *val_decl_stmt = (StmtValDecl *)malloc(sizeof(StmtValDecl));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  val_decl_stmt->idents = idents;
  val_decl_stmt->type = typ;

  stmt->kind = STMT_VAR_DECL;
  stmt->line = line;
  stmt->col = col;
  stmt->val_decl = val_decl_stmt;

  return stmt;
}

ASTStmt *mk_val_init_stmt(int line, int col, ExprList *idents, ASTType *typ,
                          ExprList *values) {
  StmtValInit *val_init = (StmtValInit *)malloc(sizeof(StmtValDecl));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  val_init->idents = idents;
  val_init->type = typ;
  val_init->exprs = values;

  stmt->kind = STMT_VAR_INIT;
  stmt->line = line;
  stmt->col = col;
  stmt->val_init = val_init;

  return stmt;
}

ASTStmt *mk_return_stmt(int line, int col, ASTExpr *expr) {
  StmtReturn *return_stmt = (StmtReturn *)malloc(sizeof(StmtReturn));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  return_stmt->expr = expr;

  stmt->kind = STMT_RETURN;
  stmt->line = line;
  stmt->col = col;
  stmt->return_stmt = return_stmt;

  return stmt;
}

ASTStmt *mk_assign_stmt(int line, int col, ASTExpr *ident, ASTExpr *value) {
  StmtAssign *assign = (StmtAssign *)malloc(sizeof(StmtAssign));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  assign->ident = ident;
  assign->expr = value;
  stmt->kind = STMT_ASSIGN;
  stmt->line = line;
  stmt->col = col;
  stmt->assign = assign;

  return stmt;
}

ASTStmt *mk_expr_stmt(int line, int col, ASTExpr *expr) {
  StmtExpr *expr_stmt = (StmtExpr *)malloc(sizeof(StmtExpr));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  expr_stmt->expr = expr;
  expr_stmt->expr->kind = expr->kind;

  stmt->kind = STMT_EXPR;
  stmt->line = line;
  stmt->col = col;
  stmt->expr = expr_stmt;

  return stmt;
}

ASTStmt *mk_func_decl_stmt(int line, int col, char *name,
                           StmtFuncParams *params, ASTType *return_typ,
                           StmtList *body) {
  StmtFuncDecl *func_decl = (StmtFuncDecl *)malloc(sizeof(StmtFuncDecl));
  ASTStmt *stmt = (ASTStmt *)malloc(sizeof(ASTStmt));

  func_decl->name = name;
  func_decl->params = params;
  func_decl->body = body;
  func_decl->return_typ = return_typ;

  stmt->kind = STMT_FUNC_DECL;
  stmt->line = line;
  stmt->col = col;
  stmt->func_decl = func_decl;

  return stmt;
}

StmtFuncParams *mk_func_params(ExprList *idents, ASTType *type) {
  StmtFuncParams *params = (StmtFuncParams *)malloc(sizeof(StmtFuncParams));
  params->idents = idents;
  params->type = type;
  params->next = NULL;

  return params;
}

void append_func_params(StmtFuncParams *curr, StmtFuncParams *param) {
  if (param == NULL) {
    return;
  }
  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = param;
}
