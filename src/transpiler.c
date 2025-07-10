#include "transpiler.h"
#include "ast.h"
#include "printers.h"
#include "y.tab.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STACK_MAX_DEPTH 1000

void transpile_printf(ASTExpr *expr) {
  ASTType *type = expr->inferred_type;
  if (type == NULL) {
    fprintf(stderr, "Error: Cannot print untyped expression \n");
    return;
  }

  if (type->kind == TYPE_PRIM) {
    const char *typeName = type->prim->name;
    if (strcmp(typeName, "String") == 0) {
      printf("printf(\"%%s \", ");
      transpile_expr(expr,0 ,0);
      printf(");\n  ");
    } else if (strcmp(typeName, "Int") == 0) {
      printf("printf(\"%%d \", ");
      transpile_expr(expr, 0,0);
      printf(");\n  ");
    } else if (strcmp(typeName, "Char") == 0) {
      printf("printf(\"%%c \", ");
      transpile_expr(expr, 0,0);
      printf(");\n  ");
    } else if (strcmp(typeName, "Bool") == 0) {
      printf("printf(\"%%d \", ");
      transpile_expr(expr, 0, 0);
      printf(");\n  ");
    } else if (strcmp(typeName, "Float") == 0) {
      printf("printf(\"%%f \", ");
      transpile_expr(expr, 0,0);
      printf(");\n  ");
    } else if (strcmp(typeName, "Frac") == 0) {
      printf("print_rational(");
      transpile_expr(expr, 0, 0);
      printf("); printf(\" \");\n  ");
    } else {
      printf("/* Cannot print type %s */", typeName);
    }
  } else {
      printf("/* Apenas tipos primitos são aceitos*/");
  }
}

void transpile_frac_binary_op(int op, ASTExpr *left, ASTExpr *right) {
  switch (op) {
  case PLUS:
    printf("rational_add(");
    break;
  case MINUS:
    printf("rational_sub(");
    break;
  case TIMES:
    printf("rational_mul(");
    break;
  case DIVIDE:
    printf("rational_div(");
    break;

  case CMP:
  case NEQ:
  case GT:
  case GEQ:
  case LT:
  case LEQ:
    printf("rational_compare(");
    break;

  default:
    printf("/* unsupported_frac_op */(");
    break;
  }

  transpile_expr(left, 0, 0);
  printf(", ");
  transpile_expr(right, 0, 0);
  printf(")");

  // For comparison operators, add the final part of the C expression
  switch (op) {
  case CMP:
    printf(" == 0");
    break;
  case NEQ:
    printf(" != 0");
    break;
  case GT:
    printf(" > 0");
    break;
  case GEQ:
    printf(" >= 0");
    break;
  case LT:
    printf(" < 0");
    break;
  case LEQ:
    printf(" <= 0");
    break;
  default:
    // No suffix for arithmetic ops
    break;
  }
}

struct {
  int continue_label;
  int break_label;
} loop_label_stack[STACK_MAX_DEPTH];

int loop_label_top = -1;

void transpile_indent(int indentation_level) {
  for (int i = 0; i < indentation_level; i++)
    printf("  ");
}

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
  return -1;
}

int get_current_break_label() {
  if (loop_label_top > -1) {
    return loop_label_stack[loop_label_top].break_label;
  }
  fprintf(stderr, "Error: 'break' used outside of a loop.\n");
  return -1;
}

int label_count = 0;

void transpile_stmt_list(StmtList *stmt_list, int indentation_level) {
  while (stmt_list != NULL) {
    transpile_indent(indentation_level);
    transpile_stmt(stmt_list->stmt, indentation_level);
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
    } else if (strcmp(type->prim->name, "Frac") == 0) {
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

void transpile_binary_op(int op) {
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

void transpile_expr(ASTExpr *expr, int depth, int is_statement) {
  if (!expr)
    return;

  switch (expr->kind) {
  case EXPR_UNARY:
    transpile_unary_op(expr->unary_op->op);
    transpile_expr(expr->unary_op->operand, depth + 1, 0);
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
 if (expr->kind == EXPR_FUNC_CALL &&
        strcmp(expr->func_call->func_name, "print") == 0) {
ExprList *args = expr->func_call->params;
      while (args != NULL) {
        transpile_printf(args->expr);
        args = args->next;
      }
      printf("printf(\"\\n\");\n");

			}
			else{

    printf("%s(", expr->func_call->func_name);
    transpile_expr_list(expr->func_call->params);
    printf(")");

			}
    break;

  case EXPR_BINARY: {
			ASTType *operand_type = expr->binary_op->left->inferred_type;

    if (operand_type != NULL && operand_type->kind == TYPE_PRIM &&
        strcmp(operand_type->prim->name, "Frac") == 0) {
        
        transpile_frac_binary_op(expr->binary_op->op, expr->binary_op->left,
                                 expr->binary_op->right);

    } else {
        printf("(");
        transpile_expr(expr->binary_op->left, depth+1, 0);
        transpile_binary_op(expr->binary_op->op);
        transpile_expr(expr->binary_op->right, depth+1, 0);
        printf(")");
    }
    break;
	};

  case EXPR_ATTR_ACCESS:
    transpile_expr(expr->attr_access->base, depth + 1, 0);
    printf(".");
    printf("%s", expr->attr_access->attribute);
    break;

  case EXPR_ARRAY_LIT:
    // Expr de array solta não é suportado pelo C
    printf("(");
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
    transpile_expr(expr_list->expr, 0, 0);
    if (expr_list->next != NULL)
      printf(", ");
    expr_list = expr_list->next;
  }
}

void transpile_val_init(ExprList *idents_list, ExprList *inits_list) {
  while (idents_list != NULL) {
    transpile_expr(idents_list->expr, 0, 0);
    if (inits_list != NULL) {
      printf(" = ");
      transpile_expr(inits_list->expr, 0, 0);
      inits_list = inits_list->next;
    }
    if (idents_list->next != NULL)
      printf(", ");
    idents_list = idents_list->next;
  }
}

void transpile_struct_fields(StmtStructField *fields, int indentation_level) {
  while (fields != NULL) {
    transpile_indent(indentation_level);
    transpile_type(fields->type);
    printf(" ");
    transpile_val_init(fields->idents, NULL);
    printf(";\n");
    fields = fields->next;
  }
}

void transpile_type_decl(StmtTypeDecl *type_decl, int indentation_level) {
  switch (type_decl->decl_kind) {
  case TYPE_DECL_ALIAS:
    printf("typedef ");
    transpile_type(type_decl->alias);
    printf(" %s;\n", type_decl->name);
    break;
  case TYPE_DECL_STRUCT:
    printf("typedef struct %s %s;\n", type_decl->name, type_decl->name);
    printf("typedef struct %s {\n", type_decl->name);
    transpile_struct_fields(type_decl->struct_.fields, indentation_level + 1);
    printf("} %s;\n", type_decl->name);
    break;
  case TYPE_DECL_ENUM:
    printf("enum %s {", type_decl->name);
    pp_expr_list(type_decl->enum_.values);
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
        transpile_expr(ident->expr, 0, 0);
        printf("[]");
      } else {
        transpile_type(params->type);
        printf(" ");
        transpile_expr(ident->expr, 0, 0);
      }
      if (ident->next != NULL || params->next != NULL) {
        printf(", ");
      }

      ident = ident->next;
    }
    params = params->next;
  }
}

void transpile_func_decl(StmtFuncDecl *func_decl, int indentation_level) {
  transpile_type(func_decl->return_typ);
  printf(" ");
  printf("%s(", func_decl->name);
  transpile_func_params(func_decl->params);
  printf("){\n");
  transpile_stmt_list(func_decl->body, indentation_level + 1);
  printf("}\n\n");
}

void transpile_declaration(ASTType *type, ExprList *idents) {
  if (type->kind == TYPE_ARRAY) {
    transpile_type(type->array->inner_type);
    printf(" ");

    while (idents != NULL) {
      transpile_expr(idents->expr, 0, 0);
      printf("[]");

      if (idents->next != NULL)
        printf(", ");
      idents = idents->next;
    }
  } else {
    transpile_type(type);
    printf(" ");
    while (idents != NULL) {
      transpile_expr(idents->expr, 0, 0);
      if (idents->next != NULL)
        printf(", ");
      idents = idents->next;
    }
  }
  printf(";\n");
}

void transpile_stmt(ASTStmt *stmt, int indentation_level) {
  switch (stmt->kind) {
  case STMT_TYPE_DECL:
    transpile_type_decl(stmt->type_decl, indentation_level);
    break;
  case STMT_FUNC_DECL:
    transpile_func_decl(stmt->func_decl, indentation_level);
    break;
  case STMT_VAR_DECL:
    transpile_declaration(stmt->val_decl->type, stmt->val_decl->idents);
    break;
  case STMT_VAR_INIT:
    ASTType *type = stmt->val_init->type;
    ExprList *idents = stmt->val_init->idents;
    ExprList *exprs = stmt->val_init->exprs;

    if (type->kind == TYPE_ARRAY && idents && !idents->next && exprs &&
        !exprs->next && exprs->expr->kind == EXPR_ARRAY_LIT) {

      ASTType *inner_type = type->array->inner_type;
      const char *var_name = idents->expr->ident->name;

      ExprList *elements = exprs->expr->array_lit->elements;
      ExprList *counter = elements;
      int count = 0;

      while (counter != NULL) {
        count++;
        counter = counter->next;
      }
      printf("Vector* %s = vector_from_array((", var_name);
      transpile_type(inner_type);
      printf("[]){");
      transpile_expr_list(elements);
      printf("}, %d, sizeof(", count);
      transpile_type(inner_type);
      printf("));\n");
    } else {
      transpile_type(type);
      printf(" ");
      transpile_val_init(idents, exprs);
      printf(";\n");
    }
    break;

  case STMT_ASSIGN:
    transpile_expr(stmt->assign->ident, 0, 0);
    printf(" = ");
    transpile_expr(stmt->assign->expr, 0, 0);
    printf(";\n");
    break;

  case STMT_EXPR:
    transpile_expr(stmt->expr->expr, 0, 1);
    printf(";\n");
    break;

  case STMT_RETURN:
    printf("return ");
    transpile_expr(stmt->return_stmt->expr, 0, 0);
    printf(";\n");
    break;

  case STMT_WHILE:
    int start_while_label = label_count++;
    int end_while_label = label_count++;

    push_loop_labels(start_while_label, end_while_label);

    printf("label_%d: // continue\n", start_while_label);
    transpile_indent(indentation_level);
    printf("if (!(");
    transpile_expr(stmt->while_stmt->cond, 0, 0);
    printf(")) goto label_%d;\n", end_while_label);

    transpile_stmt_list(stmt->while_stmt->body, indentation_level + 1);

    transpile_indent(indentation_level);
    printf("goto label_%d;\n", start_while_label);

    transpile_indent(indentation_level);
    printf("label_%d: // break\n", end_while_label);
    pop_loop_labels();
    break;

  case STMT_IF:
    int if_then_label = label_count++;
    int if_end_label = label_count++;

    printf("if ((");
    transpile_expr(stmt->if_stmt->condition, 0, 0);
    printf(")) goto label_%d;\n", if_then_label);

    if (stmt->if_stmt->else_)
      transpile_stmt_list(stmt->if_stmt->else_, indentation_level + 1);
    transpile_indent(indentation_level);
    printf("goto label_%d;\n", if_end_label);

    transpile_indent(indentation_level);
    printf("label_%d:\n", if_then_label);
    if (stmt->if_stmt->then)
      transpile_stmt_list(stmt->if_stmt->then, indentation_level + 1);

    transpile_indent(indentation_level);
    printf("label_%d:\n", if_end_label);
    break;

  case STMT_FOR:
    int for_cond_label = label_count++;
    int for_inc_label = label_count++;
    int for_end_label = label_count++;

    push_loop_labels(for_inc_label, for_end_label);

    if (stmt->for_stmt->var)
      transpile_stmt(stmt->for_stmt->var, indentation_level + 1);

    transpile_indent(indentation_level + 1);
    printf("goto label_%d;\n", for_cond_label);

    transpile_indent(indentation_level + 1);
    printf("label_%d: // continue target\n", for_inc_label);
    if (stmt->for_stmt->inc) {
      transpile_indent(indentation_level + 1);
      transpile_stmt(stmt->for_stmt->inc, indentation_level + 1);
    }

    transpile_indent(indentation_level + 1);
    printf("label_%d: // condition check\n", for_cond_label);
    transpile_indent(indentation_level + 1);
    printf("if (!(");
    transpile_expr(stmt->for_stmt->cond, 0, 0);
    printf(")) goto label_%d;\n", for_end_label);

    transpile_stmt_list(stmt->for_stmt->body, indentation_level + 1);

    transpile_indent(indentation_level + 1);
    printf("goto label_%d;\n", for_inc_label);

    transpile_indent(indentation_level + 1);
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
  printf("// Código gerado automaticamente pela J-Lang\n");
  printf("#include <stdio.h>\n");
  printf("#include <stdlib.h>\n");
  printf("#include <string.h>\n");
  printf("#include \"rational.h\"\n");
  printf("#include \"vector.h\"\n\n");
}

void transpile(StmtList *program) {
  print_headers();
  transpile_stmt_list(program, 0);
}
