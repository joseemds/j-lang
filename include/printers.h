#pragma once

#include "ast.h"
#include "y.tab.h"

void pp_stmt_list(StmtList *stmt_list);
void pp_stmt(ASTStmt *stmt);
void pp_stmt_variable();
