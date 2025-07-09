#include "transpiler.h"
#include "ast.h"
#include "printers.h"
#include "y.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STACK_MAX_DEPTH 1000

struct {
  int continue_label;
  int break_label;
} loop_label_stack[STACK_MAX_DEPTH];

int loop_label_top = -1;

void push_loop_labels(int continue_label, int break_label) {
  if (loop_label_top < STACK_MAX_DEPTH - 1) {
    loop_label_top++;
    loop_label_stack[loop_label_top].continue_label = continue_label;
    loop_label_stack[loop_label_top].break_label = break_label;
  } else {
    fprintf(stderr, "Error: Loop nesting depth exceeded.\n");
  }
}

void pop_loop_labels() {
  if (loop_label_top > -1) {
    loop_label_top--;
  }
}

int get_current_continue_label() {
  if (loop_label_top > -1) {
    return loop_label_stack[loop_label_top].continue_label;
  }
  fprintf(stderr, "Error: 'continue' used outside of a loop.\n");
  return -1; // Error case
}

int get_current_break_label() {
  if (loop_label_top > -1) {
    return loop_label_stack[loop_label_top].break_label;
  }
  fprintf(stderr, "Error: 'break' used outside of a loop.\n");
  return -1; // Error case
}

int label_count = 0;

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
    } else if (strcmp(type->prim->name,  "Frac") == 0) {
      printf("rational");
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

void transpile_frac_cons(ASTExpr *expr) {
  switch (expr->frac_cons->kind) {
    case FRAC_RATIONAL:
      printf("rational_create(%d, %d)", expr->frac_cons->rational_lit.top,
        expr->frac_cons->rational_lit.bot);
      break;
    case FRAC_EXPRS:
      printf("rational_create(");
      pp_expr(expr->frac_cons->exprs.top);
      printf(", ");
      pp_expr(expr->frac_cons->exprs.bot);
      printf(");");
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

  case EXPR_FRAC_CONS:
    transpile_frac_cons(expr);
    break;

  case EXPR_FUNC_CALL:
    printf("%s(", expr->func_call->func_name);
    transpile_expr_list(expr->func_call->params);
    printf(")"); // not putting semicolon when called as a single stmt
    // printf(");\n");
    break;

  case EXPR_BINARY:
    transpile_expr(expr->binary_op->left);
    trasnpile_binary_op(expr->binary_op->op);
    transpile_expr(expr->binary_op->right);
    break;

  case EXPR_ATTR_ACCESS:
    transpile_expr(expr->attr_access->base);
    printf(".");
    printf("%s", expr->attr_access->attribute);
    break;

  case EXPR_ARRAY_LIT:
    // Expr de array solta não é suportado pelo C
    printf(" (");
    // transpile_type(); // Pegar tipo do array
    printf("[]){");
    transpile_expr_list(expr->array_lit->elements);
    printf("}");
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
  while (idents_list != NULL) {
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
    // transpile_expr_list(fields->idents);
    // printf(" : ");
    transpile_type(fields->type);
    printf(" ");
    transpile_val_init(fields->idents, NULL);
    printf(";\n");
    fields = fields->next;
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
    // printf("struct %s {\n", type_decl->name); // "typedef struct name;"
    // above? to use without needing struct keyword
    printf("typedef struct %s {\n",
           type_decl->name); // "typedef struct name;" above? to use without
                             // needing struct keyword
    transpile_struct_fields(type_decl->struct_.fields);
    printf("} %s;\n", type_decl->name);
    break;
  case TYPE_DECL_ENUM:
    printf("enum %s {", type_decl->name);
    pp_expr_list(type_decl->enum_
                     .values); // error because in C the values aren't strings
    printf("};\n");

    break;
  }
}

void transpile_func_params(StmtFuncParams *params) {
  while (params != NULL) {
    ExprList *ident = params->idents;
    while (ident != NULL) {
      if (params->type->kind == TYPE_ARRAY) {
        transpile_type(params->type->array->inner_type);
        printf(" ");
        transpile_expr(ident->expr);
        printf("[]");
      } else {
        transpile_type(params->type);
        printf(" ");
        transpile_expr(ident->expr);
      }
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

void transpile_declaration(ASTType *type, ExprList *idents, ExprList *inits) {
    if (type->kind == TYPE_ARRAY) {
        transpile_type(type->array->inner_type);
        printf(" ");

        while (idents != NULL) {
            transpile_expr(idents->expr);
            printf("[]");

            if (inits != NULL) {
                printf(" = ");
                ASTExpr *init_expr = inits->expr;
                
                if (init_expr && init_expr->kind == EXPR_ARRAY_LIT) {
                    printf("{");
                    transpile_expr_list(init_expr->array_lit->elements);
                    printf("}");
                } else {
                    transpile_expr(init_expr);
                }
                inits = inits->next;
            }
            if (idents->next != NULL) printf(", ");
            idents = idents->next;
        }
    } 
    else {
        transpile_type(type);
        printf(" ");
        while (idents != NULL) {
            transpile_expr(idents->expr);
            if (inits != NULL) {
                printf(" = ");
                transpile_expr(inits->expr);
                inits = inits->next;
            }
            if (idents->next != NULL) printf(", ");
            idents = idents->next;
        }
    }
    printf(";\n");
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
    transpile_declaration(stmt->val_decl->type, stmt->val_decl->idents, NULL);
    break;
  case STMT_VAR_INIT:
    transpile_declaration(stmt->val_init->type, stmt->val_init->idents, stmt->val_init->exprs);
    break;
  case STMT_ASSIGN:
    transpile_expr(stmt->assign->ident);
    printf(" = ");
    transpile_expr(stmt->assign->expr);
    printf(";\n");
    break;

  case STMT_EXPR:
    transpile_expr(
        stmt->expr->expr); // not putting semicolon when called as a single stmt
    break;

  case STMT_RETURN:
    printf("return ");
    transpile_expr(stmt->return_stmt->expr);
    printf(";\n");
    break;

  case STMT_WHILE:
    int start_while_label = label_count++;
    int end_while_label = label_count++;

    push_loop_labels(start_while_label, end_while_label);

    printf("label_%d: // continue\n  ", start_while_label);
    printf("if (!(");
    transpile_expr(stmt->while_stmt->cond);
    printf(")) goto label_%d;\n", end_while_label);

    transpile_stmt_list(stmt->while_stmt->body);

    printf("  goto label_%d;\n", start_while_label);

    printf("label_%d: // break\n", end_while_label);
    pop_loop_labels();
    break;

  case STMT_IF:
    int if_then_label = label_count++;
    int if_end_label = label_count++;

    printf("if ((");
    transpile_expr(stmt->if_stmt->condition);
    printf(")) goto label_%d;\n", if_then_label);

    if (stmt->if_stmt->else_) {
      transpile_stmt_list(stmt->if_stmt->else_);
    }
    printf("goto label_%d;\n", if_end_label);

    printf("label_%d:\n", if_then_label);
    if (stmt->if_stmt->then) {
      transpile_stmt_list(stmt->if_stmt->then);
    }

    printf("label_%d:\n", if_end_label);
    break;

  case STMT_FOR:
    int for_cond_label = label_count++;
    int for_inc_label = label_count++;
    int for_end_label = label_count++;

    push_loop_labels(for_inc_label, for_end_label);

    if (stmt->for_stmt->var)
      transpile_stmt(stmt->for_stmt->var);

    printf("  goto label_%d;\n", for_cond_label);

    printf("label_%d: // continue target\n  ", for_inc_label);
    if (stmt->for_stmt->inc) {
      transpile_stmt(stmt->for_stmt->inc);
    }

    printf("label_%d: // condition check\n  ", for_cond_label);
    printf("if (!(");
    transpile_expr(stmt->for_stmt->cond);
    printf(")) goto label_%d;\n", for_end_label);

    transpile_stmt_list(stmt->for_stmt->body);

    printf("  goto label_%d;\n", for_inc_label);

    printf("label_%d: // break target\n", for_end_label);
    pop_loop_labels();
    break;

  case STMT_BREAK:
    printf("goto label_%d; // break\n", get_current_break_label());
    break;
  case STMT_CONTINUE:
    printf("goto label_%d; // continue\n", get_current_continue_label());
    break;

  default:
    printf("Unimplemented");
    break;
  }
}

void print_headers() {
  printf("#include <stdio.h>\n");
  printf("#include \"rational.h\"\n");
}

void transpile(StmtList *program) {
  print_headers();
  transpile_stmt_list(program);
}
