#pragma once

#include "ast.h"

void pp_stmt_list(StmtList *stmt_list);
void pp_stmt(ASTStmt *stmt);
void pp_stmt_variable();

void pp_expr(ASTExpr *expr);
void pp_expr_list(ExprList *list);
