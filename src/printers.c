#include "printers.h"
#include "y.tab.h"
#include <stdio.h>

void pp_expr_list_with_comma(ExprList *expr_list);
void pp_struct_fields(StmtStructField *fields);
void pp_struct_field_assigns(StructFieldAssign *assigns);

void pp_string(char *ident) {
  if (ident)
    printf("%s", ident);
}

void pp_bool(int value) { printf("%s", value ? "true" : "false"); }

void pp_int(int value) { printf("%d", value); }

void pp_float(float value) { printf("%f", value); }

void pp_type(ASTType *type) {
  if (!type)
    return;
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

void pp_unary_op(int op) {
  switch (op) {
  case NOT:
    printf("!");
    break;
  case MINUS:
    printf("-");
    break;
  default:
    printf("unknown_unary_op");
    break;
  }
}

void pp_binary_op(int op) {
  switch (op) {
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
  case OR:
    printf(" || ");
    break;
  case AND:
    printf(" && ");
    break;
  default:
    printf(" unknown_binary_op ");
    break;
  }
}

void pp_frac_cons(ASTExpr *expr) {
  switch (expr->frac_cons->kind) {
  case FRAC_RATIONAL:
    printf("%d//%d", expr->frac_cons->rational_lit.top,
           expr->frac_cons->rational_lit.bot);
    break;
  case FRAC_EXPRS:
    pp_expr(expr->frac_cons->exprs.top);
    printf(" // ");
    pp_expr(expr->frac_cons->exprs.top);
  }
}

void pp_expr(ASTExpr *expr) {
  if (!expr)
    return;
  switch (expr->kind) {
  case EXPR_BINARY:
    printf("(");
    pp_expr(expr->binary_op->left);
    pp_binary_op(expr->binary_op->op);
    pp_expr(expr->binary_op->right);
    printf(")");
    break;
  case EXPR_UNARY:
    pp_unary_op(expr->unary_op->op);
    pp_expr(expr->unary_op->operand);
    break;
  case EXPR_FUNC_CALL:
    pp_string(expr->func_call->func_name);
    printf("(");
    pp_expr_list_with_comma(expr->func_call->params);
    printf(")");
    break;
  case EXPR_INT_LITERAL:
    pp_int(expr->int_lit->value);
    break;
  case EXPR_FLOAT_LITERAL:
    pp_float(expr->float_lit->value);
    break;
  case EXPR_STRING_LITERAL:
    printf("%s", expr->string_lit->value);
    break;
  case EXPR_BOOL_LITERAL:
    pp_bool(expr->bool_lit->value);
    break;
  case EXPR_CHAR_LITERAL:
    printf("%s", expr->char_lit->value);
    break;
  case EXPR_IDENT:
    pp_string(expr->ident->name);
    break;

  case EXPR_ARRAY_LIT:
    printf("[");
    pp_expr_list_with_comma(expr->array_lit->elements);
    printf("]");
    break;
  case EXPR_ARRAY_ACCESS:
    pp_expr(expr->array_access->base);
    printf("[");
    pp_expr(expr->array_access->index);
    printf("]");
    break;
  case EXPR_ATTR_ACCESS:
    pp_expr(expr->attr_access->base);
    printf(".");
    pp_string(expr->attr_access->attribute);
    break;

  case EXPR_STRUCT_CONS:
    pp_type(expr->struct_cons->type);
    printf("{");
    pp_struct_field_assigns(expr->struct_cons->fields);
    printf("}");
    break;

  case EXPR_FRAC_CONS:
    pp_frac_cons(expr);
    break;

  default:
    printf("Unknown_Expr_Kind");
    break;
  }
}

void pp_expr_list(ExprList *expr_list) {
  while (expr_list != NULL) {
    pp_expr(expr_list->expr);
    if (expr_list->next) {
      printf(", ");
    }
    expr_list = expr_list->next;
  }
}

void pp_func_params(StmtFuncParams *params) {
  printf("(");
  while (params != NULL) {
    pp_expr_list(params->idents);
    printf(": ");
    pp_type(params->type);
    if (params->next) {
      printf(", ");
    }
    params = params->next;
  }
  printf(")");
}

void pp_stmt(ASTStmt *stmt) {
  if (!stmt)
    return;
  switch (stmt->kind) {
  case STMT_TYPE_DECL:
    printf("(type_decl %s ", stmt->type_decl->name);
    switch (stmt->type_decl->decl_kind) {
    case TYPE_DECL_ALIAS:
      printf("(alias ");
      pp_type(stmt->type_decl->alias);
      printf(")");
      break;
    case TYPE_DECL_STRUCT:
      printf("(struct \n");
      pp_struct_fields(stmt->type_decl->struct_.fields);
      printf("  )");
      break;
    case TYPE_DECL_ENUM:
      printf("(enum ");
      pp_expr_list(stmt->type_decl->enum_.values);
      printf(")");
      break;
    }
    printf(")\n");
    break;

  case STMT_VAR_DECL:
    printf("(val (");
    pp_expr_list(stmt->val_decl->idents);
    printf(") : ");
    pp_type(stmt->val_decl->type);
    printf(")\n");
    break;

  case STMT_VAR_INIT:
    printf("(val_init (");
    pp_expr_list(stmt->val_init->idents);
    printf(") : ");
    pp_type(stmt->val_init->type);
    printf(" = (");
    pp_expr_list(stmt->val_init->exprs);
    printf("))\n");
    break;

  case STMT_ASSIGN:
    printf("(assign ");
    pp_expr(stmt->assign->ident);
    printf(" = ");
    pp_expr(stmt->assign->expr);
    printf(")\n");
    break;

  case STMT_IF:
    printf("(if ");
    pp_expr(stmt->if_stmt->condition);
    printf("\n  (then ");
    pp_stmt_list(stmt->if_stmt->then);
    printf("  )");
    if (stmt->if_stmt->else_) {
      printf("\n  (else ");
      pp_stmt_list(stmt->if_stmt->else_);
      printf("  )");
    }
    printf("\n)\n");
    break;

  case STMT_FOR:
    printf("(for \n");
    printf("  (init ");
    pp_stmt(stmt->for_stmt->var);
    printf(")\n");
    printf("  (cond ");
    pp_expr(stmt->for_stmt->cond);
    printf(")\n");
    printf("  (inc ");
    pp_stmt(stmt->for_stmt->inc);
    printf(")\n");
    printf("  (body ");
    pp_stmt_list(stmt->for_stmt->body);
    printf(")\n");
    printf(")\n");
    break;

  case STMT_WHILE:
    printf("(while ");
    pp_expr(stmt->while_stmt->cond);
    printf(" \n");
    pp_stmt_list(stmt->while_stmt->body);
    printf(")\n");
    break;

  case STMT_FUNC_DECL:
    printf("(func_decl %s ", stmt->func_decl->name);
    pp_func_params(stmt->func_decl->params);
    printf(" -> ");
    pp_type(stmt->func_decl->return_typ);
    printf("\n");
    pp_stmt_list(stmt->func_decl->body);
    printf(")\n");
    break;

  case STMT_RETURN:
    printf("(return ");
    pp_expr(stmt->return_stmt->expr);
    printf(")\n");
    break;
  case STMT_BREAK:
    printf("(break)\n");
    break;
  case STMT_CONTINUE:
    printf("(continue)\n");
    break;
  case STMT_EXPR:
    printf("(expr_stmt ");
    pp_expr(stmt->expr->expr);
    printf(")\n");
    break;
  default:
    printf("(Unknown_Stmt_Kind)\n");
    break;
  }
}

void pp_stmt_list(StmtList *stmt_list) {
  if (!stmt_list)
    printf(" (empty_block) ");
  while (stmt_list != NULL) {
    printf("  ");
    pp_stmt(stmt_list->stmt);
    stmt_list = stmt_list->next;
  }
}

void pp_struct_fields(StmtStructField *fields) {
  while (fields) {
    printf("    (field (");
    pp_expr_list(fields->idents);
    printf(") : ");
    pp_type(fields->type);
    printf(")\n");
    fields = fields->next;
  }
}

void pp_struct_field_assigns(StructFieldAssign *assigns) {
  while (assigns != NULL) {
    pp_string(assigns->name);
    printf(": ");
    pp_expr(assigns->value);

    if (assigns->next != NULL) {
      printf(", ");
    }
    assigns = assigns->next;
  }
}

void pp_expr_list_with_comma(ExprList *expr_list) {
  while (expr_list != NULL) {
    pp_expr(expr_list->expr);
    if (expr_list->next != NULL) {
      printf(", ");
    }
    expr_list = expr_list->next;
  }
}
