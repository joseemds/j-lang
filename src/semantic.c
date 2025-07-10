#include "semantic.h"
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void populate_symbol_table(SymbolTable *st) {

  Symbol *print_fn =
      mk_symbol("print", SYMBOL_FUNC, mk_type_prim(-1, -1, "Void"));

  Symbol *input_fn =
      mk_symbol("input", SYMBOL_FUNC, mk_type_prim(-1, -1, "String"));

  Symbol *int_of_string_fn =
      mk_symbol("int_of_string", SYMBOL_FUNC, mk_type_prim(-1, -1, "Int"));

  Symbol *bool_of_string_fn =
      mk_symbol("int_of_string", SYMBOL_FUNC, mk_type_prim(-1, -1, "Bool"));

  Symbol *float_of_string_fn =
      mk_symbol("int_of_string", SYMBOL_FUNC, mk_type_prim(-1, -1, "Float"));

  Symbol *int_to_frac_fn =
      mk_symbol("int_to_frac", SYMBOL_FUNC, mk_type_prim(-1, -1, "Frac"));

  Symbol* frac_to_decimal = mk_symbol("frac_to_decimal", SYMBOL_FUNC, mk_type_prim(-1, -1, "Float"));
      
	Symbol* int_to_frac = mk_symbol("int_to_frac", SYMBOL_FUNC, mk_type_prim(-1, -1, "Frac"));
	Symbol* int_to_float = mk_symbol("int_to_float", SYMBOL_FUNC, mk_type_prim(-1, -1, "Float"));

  symbol_table_insert(st, print_fn);
  symbol_table_insert(st, input_fn);
  symbol_table_insert(st, int_of_string_fn);
  symbol_table_insert(st, bool_of_string_fn);
  symbol_table_insert(st, float_of_string_fn);
  symbol_table_insert(st, frac_to_decimal);
  symbol_table_insert(st, int_to_frac);
  symbol_table_insert(st, int_to_float);
}

void return_error(char *reason, int line, int col) {
  fprintf(stderr, "Error em %d:%d: %s\n", line, col, reason);
  exit(1);
}

void type_error(char *reason, int line, int col) {
  fprintf(stderr, "TypeError em %d:%d: %s\n", line, col, reason);
  exit(1);
}

int check_redeclaration(ASTExpr *expr, SymbolTable *st) {
  if (symbol_table_lookup_current_scope(st, expr->ident->name) != NULL) {
    // char err_msg[128];
    // sprintf(err_msg, "Redeclaração da variavel %s no mesmo escopo",
    // expr->ident->name); return_error(err_msg, expr->line, expr->col);
    return_error("Redeclaração de variavel", expr->line, expr->col);
  }

  return 0;
}

int cmp_types(ASTType *type1, ASTType *type2) {
  if (!type1 || !type2 || type1->kind != type2->kind) {
    return 0;
  }
  switch (type1->kind) {
  case TYPE_PRIM:
    return strcmp(type1->prim->name, type2->prim->name) == 0;
  case TYPE_IDENT:
    return strcmp(type1->ident->name, type2->ident->name) == 0;
  case TYPE_ARRAY:
    return cmp_types(type1->array->inner_type, type2->array->inner_type);
  }
  return 0;
}

void check_stmt_list(StmtList *list, SymbolTable *st) {
  while (list != NULL) {
    check_stmt(list->stmt, st);
    list = list->next;
  }
}

void check_expr(ASTExpr *expr, SymbolTable *st) {
  if (!expr) {
    return_error("Expressão nula", 0, 0);
    exit(EXIT_FAILURE);
  }
  switch (expr->kind) {
  case EXPR_INT_LITERAL:
    expr->inferred_type = mk_type_prim(expr->line, expr->col, "Int");
    break;

  case EXPR_FLOAT_LITERAL:
    expr->inferred_type = mk_type_prim(expr->line, expr->col, "Float");
    break;

  case EXPR_BOOL_LITERAL:
    expr->inferred_type = mk_type_prim(expr->line, expr->col, "Bool");
    break;

  case EXPR_STRING_LITERAL:
    expr->inferred_type = mk_type_prim(expr->line, expr->col, "String");
    break;

  case EXPR_CHAR_LITERAL:
    expr->inferred_type = mk_type_prim(expr->line, expr->col, "Char");
    break;

  case EXPR_ARRAY_LIT: {
    ExprList *elements = expr->array_lit->elements;
    if (elements == NULL) {
      // Array vazio, pegar da declaração?
      expr->inferred_type = NULL;
      break;
    }

    check_expr(elements->expr, st);

    ASTType *first_element_type = elements->expr->inferred_type;

    ExprList *current_element = elements->next;

    while (current_element != NULL) {

      check_expr(current_element->expr, st);

      if (current_element->expr->inferred_type == NULL) {

        expr->inferred_type = NULL;

        break;
      }

      if (!cmp_types(first_element_type,
                     current_element->expr->inferred_type)) {

        type_error(

            "Todos os elementos em um array literal devem ter o mesmo tipo",

            current_element->expr->line, current_element->expr->col);

        expr->inferred_type = NULL;

        return;
      }

      current_element = current_element->next;
    }

    if (expr->inferred_type != NULL || elements == NULL) {

      expr->inferred_type =

          mk_type_array(expr->line, expr->col, first_element_type);
    }

    break;
  }
  case EXPR_IDENT: {
    Symbol *symbol = symbol_table_lookup(st, expr->ident->name);
    if (!symbol) {
      // char err_msg[128];
      // sprintf(err_msg, "Identificador %s não declarado.", expr->ident->name);
      // return_error(err_msg, expr->line, expr->col);
      return_error("Identificador não declarado", expr->line, expr->col);
      expr->inferred_type = NULL;
    } else {
      expr->inferred_type = symbol->type;
    }
    break;
  }

  case EXPR_FUNC_CALL: {

    Symbol *func = symbol_table_lookup(st, expr->func_call->func_name);
    if (!func) {
      // char err_msg[128];
      // sprintf(err_msg, "Função %s não declarada.",
      // expr->func_call->func_name); return_error(err_msg, expr->line,
      // expr->col);
      return_error("Função não declarada.", expr->line, expr->col);
    } else if (func->kind != SYMBOL_FUNC) {
      return_error("Identificador não é função", expr->line, expr->col);
    }
		ExprList *args = expr->func_call->params;
    while (args != NULL) {
        check_expr(args->expr, st);
        args = args->next;
    }

    // TODO: Verificar parametros esperados vs passados
    // Tipo retorno da função
    expr->inferred_type = func->type;

    break;
  }

  case EXPR_BINARY:
    check_expr(expr->binary_op->left, st);
    check_expr(expr->binary_op->right, st);
    ASTType *lhs_type = expr->binary_op->left->inferred_type;
    ASTType *rhs_type = expr->binary_op->right->inferred_type;

    if (!cmp_types(lhs_type, rhs_type)) {
      // char err_msg[128];
      // sprintf(err_msg, "Tipos de operandos diferentes: %s e %s",
      //         lhs_type ? lhs_type->prim->name : "null",
      //         rhs_type ? rhs_type->prim->name : "null");
      // type_error(err_msg, expr->line, expr->col);
      type_error("Tipos de operandos diferente", expr->line, expr->col);
    }

    if (expr->binary_op->op >= 278 &&
        expr->binary_op->op <= 284) // operadores relacionais
      expr->inferred_type = mk_type_prim(expr->line, expr->col, "Bool");
    else
      expr->inferred_type = lhs_type; // ou rhs_type, ambos são iguais
    break;
  }
}

void check_stmt(ASTStmt *stmt, SymbolTable *st) {
  switch (stmt->kind) {
  case STMT_VAR_DECL: {
    ExprList *idents = stmt->val_decl->idents;
    while (idents != NULL) {
      if (!check_redeclaration(idents->expr, st)) {
        Symbol *ident = malloc(sizeof(Symbol));
        ident->name = idents->expr->ident->name;
        ident->kind = SYMBOL_VAR;
        ident->type = stmt->val_decl->type;
				// idents->expr->inferred_type = ident->type;
        symbol_table_insert(st, ident);
      }

      idents = idents->next;
    }
    break;
  }
  case STMT_VAR_INIT: {
    check_expr(stmt->val_init->exprs->expr, st);
    ASTType *rhs_type = stmt->val_init->exprs->expr->inferred_type;

    if (!rhs_type)
      break;

    if (stmt->val_init->type && !cmp_types(stmt->val_init->type, rhs_type)) {
      return_error("Tipo da expressao não condiz com tipo declarado",
                   stmt->line, stmt->col);
      break;
    }

    ExprList *current_ident = stmt->val_init->idents;
    while (current_ident) {
      const char *name = current_ident->expr->ident->name;

      if (symbol_table_lookup_current_scope(st, name)) {
        return_error("Redeclaração de variavel no mesmo escopo",
                     current_ident->expr->line, current_ident->expr->col);
      } else {
        // if (!check_redeclaration(name, st)) { // não funcionou
        Symbol *new_symbol = malloc(sizeof(Symbol));
        new_symbol->name = strdup(name);
        new_symbol->kind = SYMBOL_VAR;
        new_symbol->type = rhs_type;
        symbol_table_insert(st, new_symbol);
      }
      current_ident = current_ident->next;
    }
    break;
  }

  case STMT_ASSIGN:
    check_expr(stmt->assign->ident, st);
    check_expr(stmt->assign->expr, st);

    ASTType *lhs_type = stmt->assign->ident->inferred_type;
    ASTType *rhs_type = stmt->assign->expr->inferred_type;

    if (lhs_type && rhs_type && !cmp_types(lhs_type, rhs_type)) {
      return_error("Type error: tipos diferentes entre operandos", stmt->line,
                   stmt->col);
    }
    break;

  case STMT_FUNC_DECL:
    if (symbol_table_lookup_current_scope(st, stmt->func_decl->name)) {
      return_error("Function re-declared in the same scope", stmt->line,
                   stmt->col);
      break;
    }

    Symbol *func_symbol = malloc(sizeof(Symbol));
    func_symbol->name = strdup(stmt->func_decl->name);
    func_symbol->kind = SYMBOL_FUNC;
    func_symbol->type = stmt->func_decl->return_typ;
    symbol_table_insert(st, func_symbol);

    symbol_table_enter_scope(st);

    StmtFuncParams *params = stmt->func_decl->params;

    while (params != NULL) {
      ExprList *ident_node = params->idents;

      while (ident_node != NULL) {
        const char *param_name = ident_node->expr->ident->name;

        if (symbol_table_lookup_current_scope(st, param_name)) {
          char err_msg[256];
          sprintf(err_msg, "Duplicate parameter name '%s'", param_name);
          return_error(err_msg, ident_node->expr->line, ident_node->expr->col);
        }

        Symbol *param_symbol = malloc(sizeof(Symbol));
        param_symbol->name = strdup(param_name);
        param_symbol->kind = SYMBOL_VAR;
        param_symbol->type = params->type;

        symbol_table_insert(st, param_symbol);

        ident_node = ident_node->next;
      }
      params = params->next;
    }

    check_stmt_list(stmt->func_decl->body, st);

    symbol_table_exit_scope(st);
    break;

  case STMT_IF:
    check_expr(stmt->if_stmt->condition, st);
    ASTType *cond_type = stmt->if_stmt->condition->inferred_type;
    if (cond_type && (cond_type->kind != TYPE_PRIM ||
                      strcmp(cond_type->prim->name, "Bool") != 0)) {
      // printf("Cond type: %s\n", cond_type->prim->name);
      return_error("Type Error: Condição deve ser booleana",
                   stmt->if_stmt->condition->line,
                   stmt->if_stmt->condition->col);
    }
    check_stmt_list(stmt->if_stmt->then, st);
    check_stmt_list(stmt->if_stmt->else_, st);
    break;

  case STMT_EXPR:
    check_expr(stmt->expr->expr, st);
    break;

  case STMT_WHILE:
    check_expr(stmt->while_stmt->cond, st);
    break;
    // Checar se é Bool
    // if(stmt->while_stmt->cond->inferred_type);

  case STMT_FOR:
    check_stmt(stmt->for_stmt->var, st);
    // Checar se é Bool
    check_expr(stmt->for_stmt->cond, st);
    check_stmt(stmt->for_stmt->inc, st);

    ASTType *var_type = stmt->for_stmt->var->val_decl
                            ? stmt->for_stmt->var->val_init->type
                            : stmt->for_stmt->var->assign->expr->inferred_type;
    ;
    ASTType *cond_type_ = stmt->for_stmt->cond->inferred_type;
    ASTType *inc_type = stmt->for_stmt->inc->assign->ident->inferred_type;

    if (var_type && cond_type_ && inc_type &&
        (!strcmp(cond_type_->prim->name, "Bool") == 0)) {
      return_error(
          "Type Error: tipos diferentes entre variavel, condicao e incremento",
          stmt->line, stmt->col);
    }

    check_stmt_list(stmt->for_stmt->body, st);
    break;

  default:
    break;
  }
}

int check_program(StmtList *program) {
  SymbolTable *st = symbol_table_create();

  populate_symbol_table(st);

  check_stmt_list(program, st);

  // symbol_table_free(st);
  return 0;
}
