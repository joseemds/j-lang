#include "transpiler.h"
#include "ast.h"
#include "printers.h"
#include <stdio.h>
#include <string.h>

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
            } else {
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


void transpile_expr(ASTExpr* expr) {
	switch (expr->kind) {
		case EXPR_IDENT:
			printf("%s", expr->ident->name);
			break;

		default:
			printf("Unimplemented");
			break;
	
	}
}

void transpile_expr_list(ExprList *expr_list) {
  while (expr_list != NULL) {
    transpile_expr(expr_list->expr);
    if (expr_list->next != NULL) {
      printf(", ");
    }
    expr_list = expr_list->next;
  }
}

void transpile_struct_fields(StmtStructField* fields){
	while (fields != NULL) {
		transpile_expr_list(fields->idents);
		printf(" : ");
		transpile_type(fields->type);
	
	}
}

void transpile_type_decl(StmtTypeDecl* type_decl){
	switch (type_decl->decl_kind) {
		case TYPE_DECL_ALIAS: 
			printf("typedef ");
			transpile_type(type_decl->alias);
			printf(" %s;\n", type_decl->name);
			break;
		case TYPE_DECL_STRUCT:
			printf("struct %s {\n", type_decl->name);
			printf("};\n");
			break;
		case TYPE_DECL_ENUM: 
			printf("enum %s {", type_decl->name);
			pp_expr_list(type_decl->enum_.values);
			printf("}");
			
			break;
	}
}


void transpile_stmt(ASTStmt* stmt){
	switch (stmt->kind) {
		case STMT_TYPE_DECL: transpile_type_decl(stmt->type_decl);
		default: printf("Unimplemented");
	
	}
}

