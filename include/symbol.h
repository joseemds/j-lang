#pragma once
#include "ast.h"

typedef enum { SYMBOL_VAR, SYMBOL_FUNC, SYMBOL_TYPE, SYMBOL_PARAM } SymbolKind;
typedef struct Symbol {
  char *name;
  SymbolKind kind;
  ASTType *type;
} Symbol;

Symbol *mk_symbol(char *name, SymbolKind kind, ASTType *type);
