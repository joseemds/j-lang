#include "ast.h"

void transpile_stmt_list(StmtList* stmt_list);
void transpile_stmt(ASTStmt* stmt);
void transpile_type(ASTType* type);
void transpile_expr(ASTExpr* expr);
void transpile_expr_list(ExprList* expr);
