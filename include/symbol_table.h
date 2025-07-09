#pragma once
#include "hashtbl.h"
#include "symbol.h"

#define SCOPE_STACK_MAX_DEPTH 100

typedef struct SymbolTable {
  HashTable *scope_stack[SCOPE_STACK_MAX_DEPTH];
  int scope_level;
} SymbolTable;

SymbolTable *symbol_table_create();

void symbol_table_enter_scope(SymbolTable *st);

void symbol_table_exit_scope(SymbolTable *st);

int symbol_table_insert(SymbolTable *st, Symbol *symbol);

Symbol *symbol_table_lookup(SymbolTable *st, const char *name);

Symbol *symbol_table_lookup_current_scope(SymbolTable *st, const char *name);
