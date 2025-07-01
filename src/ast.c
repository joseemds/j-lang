#include "ast.h"

ASTExpr* mk_ident(int line, int col, char* name) {
    ExprIdent* ident = (ExprIdent*)malloc(sizeof(ExprIdent));
    ident->name = strdup(name);

    ASTExpr* expr_node = (ASTExpr*)malloc(sizeof(ASTExpr));
    expr_node->kind = EXPR_IDENT;
    expr_node->line = line;
    expr_node->col = col;
    expr_node->ident = ident;

    return expr_node;
}

ASTExpr* mk_bool_lit(int line, int col, int value) {
    ExprBoolLiteral* bool_lit = (ExprBoolLiteral*)malloc(sizeof(ExprBoolLiteral));
    if (!bool_lit) {
        fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
        exit(EXIT_FAILURE);
    }

		bool_lit->value = value;

    ASTExpr* node = (ASTExpr*) malloc(sizeof(ASTExpr));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
        exit(EXIT_FAILURE);
    }

    node->kind = EXPR_BOOL_LITERAL;
    node->line = line;
    node->col = col;
    node->bool_lit = bool_lit;

    return node;
}

ASTExpr* mk_int_lit(int line, int col, int value) {
		ExprIntLiteral* int_lit = (ExprIntLiteral*)malloc(sizeof(ExprIntLiteral));
		int_lit->value = value;
    ASTExpr* node = (ASTExpr*) malloc(sizeof(ASTExpr));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for ASTExpr node\n");
        exit(EXIT_FAILURE);
    }

    node->kind = EXPR_INT_LITERAL;
    node->line = line;
    node->col = col;
    node->int_lit = int_lit;

    return node;
}

ASTExpr* mk_float_lit(int line, int col, float value) {
    ExprFloatLiteral* float_lit = (ExprFloatLiteral*)malloc(sizeof(ExprFloatLiteral));
    float_lit->value = value;

    ASTExpr* expr_node = (ASTExpr*)malloc(sizeof(ASTExpr));
    expr_node->kind = EXPR_FLOAT_LITERAL;
    expr_node->line = line;
    expr_node->col = col;
    expr_node->float_lit = float_lit;

    return expr_node;
}

ASTExpr* mk_string_lit(int line, int col, char* value) {
    ExprStringLiteral* string_lit = (ExprStringLiteral*)malloc(sizeof(ExprStringLiteral));
    string_lit->value = strdup(value);

    ASTExpr* expr_node = (ASTExpr*)malloc(sizeof(ASTExpr));
    expr_node->kind = EXPR_STRING_LITERAL;
    expr_node->line = line;
    expr_node->col = col;
    expr_node->string_lit = string_lit;

    return expr_node;
}

ASTExpr* mk_binary_op(int line, int col, int op, ASTExpr* left, ASTExpr* right){
    ASTExpr* expr_node = (ASTExpr*)malloc(sizeof(ASTExpr));
		ExprBinaryOp* binary_op = (ExprBinaryOp*)malloc(sizeof(ExprBinaryOp));

		binary_op->left = left;
		binary_op->right = right;
		binary_op->op = op;

		expr_node->col = col;
		expr_node->line = line;
		expr_node->binary_op=binary_op;

		return expr_node;

};

ASTStmt* mk_if_stmt(int line, int col, ASTExpr* cond, StmtList* then, StmtList* else_){
	StmtIf* if_stmt = (StmtIf*)malloc(sizeof(StmtIf));
	ASTStmt* stmt = (ASTStmt*)malloc(sizeof(ASTStmt));

	stmt->kind=STMT_IF;
	stmt->col=col;
	stmt->line=line;
	stmt->if_stmt = if_stmt;

	return stmt;

}

ASTType* mk_type_ident(int line, int col, char* type_name) {
  ASTType* typ = (ASTType*)malloc(sizeof(ASTType));
  TypeIdent* ident = (TypeIdent*)malloc(sizeof(TypeIdent));
  ident->name = type_name;
  
  typ->line = line;
  typ->col = col;
  typ->ident = ident;
  typ->kind = TYPE_IDENT;
  return typ;
}


ASTType* mk_type_prim(int line, int col, char* type_name) {
  ASTType* typ = (ASTType*)malloc(sizeof(ASTType));
  TypePrim* prim = (TypePrim*)malloc(sizeof(TypePrim));
  prim->name = type_name;
  
  typ->line = line;
  typ->col = col;
  typ->prim = prim;
  typ->kind = TYPE_PRIM;
  return typ;
}

ASTType* mk_type_array(int line, int col, ASTType* inner_type){
  ASTType* typ = (ASTType*)malloc(sizeof(ASTType));
  TypeArray* array = (TypeArray*)malloc(sizeof(TypeArray));
  array->inner_type = inner_type;

  typ->line = line;
  typ->col = col;
  typ->kind = TYPE_ARRAY;
  typ->array = array;
  return typ;
}
