#include "ast.h"

void transpile_stmt_list(StmtList *stmt_list, int indentation_level);
void transpile_stmt(ASTStmt *stmt, int indentation_level);
void transpile_type(ASTType *type);
void transpile_expr(ASTExpr *expr, int depth, int is_statement);
void transpile_expr_list(ExprList *expr);

void transpile(StmtList *stmt_list);
