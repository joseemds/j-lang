CFLAGS = -g -Wall -Iinclude

SRCDIR := src
INCDIR := include
BINDIR := bin
EXAMPLES_DIR := examples

C_SOURCES   := $(wildcard $(SRCDIR)/*.c)
LEX_SOURCE := $(SRCDIR)/scanner.l
YACC_SOURCE:= $(SRCDIR)/parser.y
EXAMPLE_FILES = $(wildcard $(EXAMPLES_DIR)/*.j)


LEX_GEN_C := scanner.yy.c
YACC_GEN_C := y.tab.c
YACC_GEN_H := $(INCDIR)/y.tab.h

EXECUTABLE  := j-lang
TARGET      := $(BINDIR)/$(EXECUTABLE)


.PHONY: all
all: $(TARGET)


.PHONY: test
test: $(TARGET)
	@echo "--- Rodando teste da linguagem nos arquivos de exemplo ---"
	@for file in $(EXAMPLE_FILES); do \
		$(TARGET) < $$file > /dev/null 2>&1 && echo "    $$file OK" || (echo "    $$file FALHOU"; exit 1); \
	done
	@echo "--- Todos os testes passaram com sucesso! ---"

$(TARGET): $(LEX_GEN_C) $(YACC_GEN_C)
	@echo "==> Compiling and Linking: $(CC) $^ -o $(TARGET)"
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
	@echo "==> Build complete: $(TARGET)"

$(YACC_GEN_C): $(YACC_SOURCE)
	@echo "==> Creating directory: $(INCDIR)"
	@mkdir -p $(INCDIR)
	@echo "==> Running Bison..."
	bison -d --defines=$(YACC_GEN_H) -v -g -o $(YACC_GEN_C) $(YACC_SOURCE)

$(LEX_GEN_C): $(LEX_SOURCE) $(YACC_GEN_H)
	@echo "==> Running Flex: flex $(LEX_SOURCE)"
	flex -o $(LEX_GEN_C) $(LEX_SOURCE)

$(YACC_GEN_H): $(YACC_GEN_C)

.PHONY: clean
clean:
	@echo "==> Cleaning up generated files..."
	rm -f $(TARGET) $(LEX_GEN_C) $(YACC_GEN_C) $(YACC_GEN_H) y.output y.vcg y.gv $(BINDIR)
	@echo "==> Done."
