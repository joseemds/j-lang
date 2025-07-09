#include "semantic.h"
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void return_error(char *reason, int line, int col) {
  fprintf(stderr, "Error em %d:%d: %s\n", line, col, reason);
}

void type_error(char *reason, int line, int col) {
  fprintf(stderr, "TypeError em %d:%d: %s\n", line, col, reason);
}

int check_redeclaration(ASTExpr *expr, SymbolTable *st) {
  if (symbol_table_lookup_current_scope(st, expr->ident->name) != NULL) {
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
  case EXPR_IDENT: {
    Symbol *symbol = symbol_table_lookup(st, expr->ident->name);
    if (!symbol) {
      return_error("Identifier not declared", expr->line, expr->col);
      expr->inferred_type = NULL;
    } else {
      expr->inferred_type = symbol->type;
    }
    break;
	}

  case EXPR_BINARY:
    check_expr(expr->binary_op->left, st);
    check_expr(expr->binary_op->right, st);
    ASTType *lhs_type = expr->binary_op->left->inferred_type;
    ASTType *rhs_type = expr->binary_op->right->inferred_type;

    if (!cmp_types(lhs_type, rhs_type)) {
      type_error("Tipo de operandos diferente", expr->line, expr->col);
    }
  }
}

void check_stmt(ASTStmt *stmt, SymbolTable *st) {
  switch (stmt->kind) {
  case STMT_VAR_DECL: {
    ExprList *idents = stmt->val_decl->idents;
    while (idents != NULL) {
			// TODO: nao ta funcionando
      if (symbol_table_lookup_current_scope(st, idents->expr->ident->name)) {
        return_error("Redeclaração de variável", idents->expr->line,
                     idents->expr->col);
      } else {
        Symbol *ident = malloc(sizeof(Symbol));
        ident->name = idents->expr->ident->name;
        ident->kind = SYMBOL_VAR;
        ident->type = stmt->val_decl->type;
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

    check_stmt_list(stmt->func_decl->body, st);

    symbol_table_exit_scope(st);
    break;

  case STMT_IF:
    check_expr(stmt->if_stmt->condition, st);
    ASTType *cond_type = stmt->if_stmt->condition->inferred_type;
    if (cond_type && (cond_type->kind != TYPE_PRIM ||
                      strcmp(cond_type->prim->name, "Bool") != 0)) {
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

  default:
    break;
  }
}

int check_program(StmtList *program) {
  SymbolTable *st = symbol_table_create();

  check_stmt_list(program, st);

  // symbol_table_free(st);
  return 0;
}
