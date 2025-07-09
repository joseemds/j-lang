#include "symbol_table.h"
#include <string.h>

SymbolTable *symbol_table_create() {
  SymbolTable *st = (SymbolTable *)malloc(sizeof(SymbolTable));
  if (!st)
    return NULL;

  st->scope_level = -1;
  memset(st->scope_stack, 0, sizeof(st->scope_stack));
  // for (int i = 0; i < SCOPE_MAX_DEPTH; i++) st->scope_stack[i] = NULL;

  symbol_table_enter_scope(st);
  return st;
}

void symbol_table_enter_scope(SymbolTable *st) {
  if (st->scope_level >= SCOPE_MAX_DEPTH - 1)
    return;

  st->scope_level++;
  if (!st->scope_stack[st->scope_level])
    st->scope_stack[st->scope_level] = hashtbl_init();
}

void symbol_table_exit_scope(SymbolTable *st) {
  if (st->scope_level < 0)
    return;

  if (st->scope_stack[st->scope_level]) {
    hashtbl_free(st->scope_stack[st->scope_level]);
    // free(st->scope_stack[st->scope_level]);
    st->scope_stack[st->scope_level] = NULL;
  }
  st->scope_level--;
}

int symbol_table_insert(SymbolTable *st, Symbol *symbol) {
  if (st->scope_level < 0 || !st->scope_stack[st->scope_level]) {
    return 0;
  }

  if (symbol_table_lookup_current_scope(st, symbol->name) != NULL) {
    return 0;
  }

  hashtbl_put(st->scope_stack[st->scope_level], symbol->name, symbol);
  return 1;
}

Symbol *symbol_table_lookup(SymbolTable *st, const char *name) {
  for (int i = st->scope_level; i >= 0; i--) {
    Symbol *symbol = hashtbl_get(st->scope_stack[i], name);
    if (symbol != NULL) {
      return symbol;
    }
  }
  return NULL;
}

Symbol *symbol_table_lookup_current_scope(SymbolTable *st, const char *name) {
  if (st->scope_level < 0 || !st->scope_stack[st->scope_level])
    return NULL;

  return hashtbl_get(st->scope_stack[st->scope_level], name);
}
