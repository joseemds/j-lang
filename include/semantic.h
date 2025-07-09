#pragma once

#include "ast.h"
#include "symbol_table.h"

void check_stmt_list(StmtList *list, SymbolTable *st);
void check_stmt(ASTStmt *stmt, SymbolTable *st);
void check_expr(ASTExpr *expr, SymbolTable *st);
int check_program(StmtList *program);
