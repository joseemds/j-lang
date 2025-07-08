#include "transpiler.h"
#include "ast.h"
#include "printers.h"
#include "y.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void transpile_stmt_list(StmtList *stmt_list) {
  while (stmt_list != NULL) {
    printf("  ");
    transpile_stmt(stmt_list->stmt);
    stmt_list = stmt_list->next;
  }
}

void transpile_type(ASTType *type) {
  switch (type->kind) {
  case TYPE_PRIM:
    if (strcmp(type->prim->name, "String") == 0) {
      printf("char*");
    } else if (strcmp(type->prim->name, "Bool") == 0) {
      printf("int");
    } else {
      type->prim->name[0] = tolower(type->prim->name[0]);
      printf("%s", type->prim->name);
    }
    break;

  case TYPE_IDENT:
    printf("%s", type->ident->name);
    break;

  case TYPE_ARRAY:
    // TODO: define custom array
    printf("[");
    transpile_type(type->array->inner_type);
    printf("]");
    break;

  default:
    printf("/* unknown_type_kind */");
    break;
  }
}

void trasnpile_binary_op(int op) {
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

void transpile_unary_op(int op) {
  switch (op) {
  case NOT:
    printf("!");
    break;
  case MINUS:
    printf("-");
    break;
  }
}

void transpile_expr(ASTExpr *expr) {
  switch (expr->kind) {
  case EXPR_UNARY:
    transpile_unary_op(expr->unary_op->op);
    transpile_expr(expr->unary_op->operand);
    break;

  case EXPR_IDENT:
    printf("%s", expr->ident->name);
    break;

  case EXPR_CHAR_LITERAL:
    printf("%s", expr->char_lit->value);
    break;

  case EXPR_STRING_LITERAL:
    printf("%s", expr->string_lit->value);
    break;

  case EXPR_INT_LITERAL:
    printf("%d", expr->int_lit->value);
    break;

  case EXPR_BOOL_LITERAL:
    printf("%d", expr->bool_lit->value);
    break;

  case EXPR_FLOAT_LITERAL:
    printf("%f", expr->float_lit->value);
    break;

  case EXPR_RATIONAL_LITERAL:
    printf("rational_create(%d, %d);", expr->rational_lit->value.top,
           expr->rational_lit->value.bot);
    break;

  case EXPR_FUNC_CALL:
    printf("%s(", expr->func_call->func_name);
    transpile_expr_list(expr->func_call->params);
    printf(")");
    break;

  case EXPR_BINARY:
    transpile_expr(expr->binary_op->left);
    trasnpile_binary_op(expr->binary_op->op);
    transpile_expr(expr->binary_op->right);
    break;

  default:
    printf("Unimplemented");
    break;
  }
}

void transpile_expr_list(ExprList *expr_list) {
  while (expr_list != NULL) {
    transpile_expr(expr_list->expr);
    if (expr_list->next != NULL)
      printf(", ");
    expr_list = expr_list->next;
  }
}

void transpile_val_init(ExprList *idents_list, ExprList *inits_list) {
  while(idents_list != NULL) {
    transpile_expr(idents_list->expr);
    if (inits_list != NULL) {
      printf(" = ");
      transpile_expr(inits_list->expr);
      inits_list = inits_list->next;
    }
    if (idents_list->next != NULL)
      printf(", ");
    idents_list = idents_list->next;
  }
}

void transpile_struct_fields(StmtStructField *fields) {
  while (fields != NULL) {
    transpile_expr_list(fields->idents);
    printf(" : ");
    transpile_type(fields->type);
  }
}

void transpile_type_decl(StmtTypeDecl *type_decl) {
  switch (type_decl->decl_kind) {
  case TYPE_DECL_ALIAS:
    printf("typedef ");
    transpile_type(type_decl->alias);
    printf(" %s;\n", type_decl->name);
    break;
  case TYPE_DECL_STRUCT:
    printf("struct %s {\n", type_decl->name);
    printf("};\n");
    break;
  case TYPE_DECL_ENUM:
    printf("enum %s {", type_decl->name);
    pp_expr_list(type_decl->enum_.values);
    printf("}");

    break;
  }
}

void transpile_func_params(StmtFuncParams *params) {
  while (params != NULL) {
    ExprList *ident = params->idents;
    while (ident != NULL) {
      transpile_type(params->type);
      printf(" ");
      transpile_expr(ident->expr);
      if (ident->next != NULL || params->next != NULL) {
        printf(", ");
      }

      ident = ident->next;
    }
    params = params->next;
  }
}

void transpile_func_decl(StmtFuncDecl *func_decl) {
  transpile_type(func_decl->return_typ);
  printf(" ");
  printf("%s(", func_decl->name);
  transpile_func_params(func_decl->params);
  printf("){\n");
  transpile_stmt_list(func_decl->body);
  printf("\n}\n");
}

void transpile_stmt(ASTStmt *stmt) {
  switch (stmt->kind) {
  case STMT_TYPE_DECL:
    transpile_type_decl(stmt->type_decl);
    break;
  case STMT_FUNC_DECL:
    transpile_func_decl(stmt->func_decl);
    break;
  case STMT_VAR_DECL:
    transpile_type(stmt->val_decl->type);
    printf(" ");
    transpile_expr_list(stmt->val_decl->idents);
    printf(";\n");
    break;
  case STMT_VAR_INIT:
    transpile_type(stmt->val_init->type);
    printf(" ");
    // transpile_expr_list(stmt->val_init->idents);
    // printf(" = ");
    // transpile_expr_list(stmt->val_init->exprs);
    transpile_val_init(stmt->val_init->idents, stmt->val_init->exprs);
    printf(";\n");
    break;
  case STMT_ASSIGN:
    transpile_expr(stmt->assign->ident);
    printf(" = ");
    transpile_expr(stmt->assign->expr);
    printf(";\n");
    break;

  case STMT_EXPR:
    transpile_expr(stmt->expr->expr);
    break;

  default:
    printf("Unimplemented");
    break;
  }
}
