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
  case EQUAL:
    printf(" = ");
    break;
  case CMP:
    printf(" == ");
    break;
  case LEQ:
    printf(" <= ");
    break;
  case LT:
    printf(" < ");
    break;
  case GEQ:
    printf(" >= ");
    break;
  case GT:
    printf(" > ");
    break;
  case NEQ:
    printf(" != ");
    break;
  case PLUS:
    printf(" + ");
    break;
  case MINUS:
    printf(" - ");
    break;
  case TIMES:
    printf(" * ");
    break;
  case DIVIDE:
    printf(" / ");
    break;
  case MOD:
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
    printf("(");
    pp_string(expr->func_call->func_name);
    printf(" ");
    pp_expr_list(expr->func_call->params);
    printf(")");
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
    printf(" ");
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
  printf("\n");
}

void pp_stmt(ASTStmt *stmt) {
  switch (stmt->kind) {
  default:
    printf("Unknown stmt kind %d", stmt->kind);

  case STMT_FUNC_DECL:
    printf("(func %s ", stmt->func_decl->name);
    pp_func_params(stmt->func_decl->params);
    printf(" ");
    pp_stmt_list(stmt->func_decl->body);

    printf(")");
    printf(": ");
    pp_type(stmt->func_decl->return_typ);
    printf("\n");
    break;

	case STMT_WHILE:
		printf("(while (");
		pp_expr(stmt->while_stmt->cond);
		printf(") ");
		printf("(");
		pp_stmt_list(stmt->while_stmt->body);
		printf(")");
		printf("\n");
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

  case STMT_VAR_DECL:
    printf("(val ");
    printf("(");
    pp_expr_list(stmt->val_decl->idents);
    printf(":: ");
    pp_type(stmt->val_decl->type);
    printf("))\n");
    break;

  case STMT_VAR_INIT:
    printf("(val ");
    printf("(");
    pp_expr_list(stmt->val_init->idents);
    printf(" ");
    pp_expr_list(stmt->val_init->exprs);
    printf(" ");
    printf(":: ");
    pp_type(stmt->val_decl->type);
    printf("))\n");
    break;

  case STMT_ASSIGN:
    printf("(assign ");
    pp_expr(stmt->assign->ident);
    printf("(");
    printf(" ");
    pp_expr(stmt->assign->expr);
    printf("))\n");
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
  while (stmt_list != NULL) {
    pp_stmt(stmt_list->stmt);
    stmt_list = stmt_list->next;
  }
}
