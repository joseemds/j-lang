#include "printers.h"
#include <stdio.h>

void pp_string(char *ident) { printf("%s", ident); }

void pp_bool(int value) {
  if (value == 1) {
    printf("true");
  } else {
    printf("false");
  }
}

void pp_int(int value) { printf("%i", value); }

void pp_float(float value) { printf("%f", value); }

void pp_type(ASTType *type) {
  switch (type->kind) {
  case TYPE_IDENT:
    pp_string(type->ident->name);
    break;
  case TYPE_PRIM:
    pp_string(type->prim->name);
    break;
  case TYPE_ARRAY:
    printf("[");
    pp_type(type->array->inner_type);
    printf("]");
    break;
  }
}

void pp_binary_op(int op) {
  switch (op) {
  default:
    printf("Unknown operation %d", op);
  case 276:
    printf(" = ");
    break;
  case 277:
    printf(" == ");
    break;
  case 278:
    printf(" <= ");
    break;
  case 279:
    printf(" < ");
    break;
  case 280:
    printf(" >= ");
    break;
  case 281:
    printf(" > ");
    break;
  case 282:
    printf(" != ");
    break;
  case 285:
    printf(" + ");
    break;
  case 286:
    printf(" - ");
    break;
  case 297:
    printf(" * ");
    break;
  case 298:
    printf(" / ");
    break;
  case 289:
    printf(" %% ");
    break;
  }
}

void pp_expr(ASTExpr *expr) {
  switch (expr->kind) {
  case EXPR_BINARY:
    pp_expr(expr->binary_op->left);
    pp_binary_op(expr->binary_op->op);
    pp_expr(expr->binary_op->right);

    break;
  case EXPR_FUNC_CALL:
    break;
  case EXPR_INT_LITERAL:
    pp_int(expr->int_lit->value);
    break;
  case EXPR_FLOAT_LITERAL:
    pp_float(expr->float_lit->value);
    break;
  case EXPR_ARRAY_LIT:
    break;
  case EXPR_ARRAY_ACCESS:
    break;
  case EXPR_ATTR_ACCESS:
    break;
  case EXPR_CONS:
    break;
  case EXPR_STRING_LITERAL:
    pp_string(expr->string_lit->value);
    break;
  case EXPR_BOOL_LITERAL:
    pp_bool(expr->bool_lit->value);
    break;
  case EXPR_IDENT:
    pp_string(expr->ident->name);
    break;
  case EXPR_RATIONAL_LITERAL:
    break;

  default:
    printf("Unknown expr kind");
  }
}

void pp_expr_list(ExprList *expr_list) {
  while (expr_list != NULL) {
    pp_expr(expr_list->expr);
    printf(" ");
    expr_list = expr_list->next;
  }
}

void pp_func_param(StmtFuncParams *param) {
  printf("( ");
  pp_expr_list(param->idents);
  printf(" : ");
  pp_type(param->type);
  printf(")");
}

void pp_func_params(StmtFuncParams *params) {
  printf("(");
  while (params != NULL) {
    pp_func_param(params);
    params = params->next;
  }
  printf(")");
}

void pp_stmt(ASTStmt *stmt) {
  switch (stmt->kind) {
  default:
    printf("Unknown stmt kind");

  case STMT_FUNC_DECL:
    printf("(func %s ", stmt->func_decl->name);
    pp_func_params(stmt->func_decl->params);
    printf(" ");
    pp_stmt_list(stmt->func_decl->body);

    printf(")");
    printf(": ");
    pp_type(stmt->func_decl->return_typ);

    break;

  case STMT_IF:
    printf("(if ");
    printf("(");
    pp_expr(stmt->if_stmt->condition);
    printf(") ");
    if (stmt->if_stmt->then) {
      printf("(");
      pp_stmt_list(stmt->if_stmt->then);
      printf(") ");
    }
    if (stmt->if_stmt->else_) {
      printf("(");
      pp_stmt_list(stmt->if_stmt->else_);
      printf(")");
    }
    printf(")");
    break;

  case STMT_RETURN:
    printf("return ");
    pp_expr(stmt->return_stmt->expr);
    break;

  case STMT_EXPR:
    pp_expr(stmt->expr->expr);
  }
}

void pp_stmt_list(StmtList *stmt_list) {
  pp_stmt(stmt_list->stmt);
  while (stmt_list->next != NULL) {
    stmt_list = stmt_list->next;
  }
}
