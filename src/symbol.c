#include "symbol.h"
#include <stdlib.h>

Symbol *mk_symbol(char *name, SymbolKind kind, ASTType *type) {
  Symbol *s = malloc(sizeof(Symbol));
  s->name = name;
  s->kind = kind;
  s->type = type;

  return s;
}
