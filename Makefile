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
	@mkdir -p ./out
	@for file in $(EXAMPLE_FILES); do \
		filename=$$(basename $$file .j); \
		$(TARGET) < $$file > ./out/$$filename.c 2>&1 && echo "    $$file OK" || (echo "    $$file FALHOU"; exit 1); \
	done
	@echo "--- Todos os testes passaram com sucesso! ---"

$(TARGET): $(LEX_GEN_C) $(YACC_GEN_C) $(C_SOURCES)
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
	rm -rf $(TARGET) $(LEX_GEN_C) $(YACC_GEN_C) $(YACC_GEN_H) y.output y.vcg y.gv $(BINDIR)

.PHONY: clean-all
clean-all: clean
	@echo "==> Cleaning up all generated files including output..."
	rm -rf ./out ./bin/compiled
	@echo "==> Done."

.PHONY: compile
compile:
	@echo "--- Compilando arquivos C gerados ---"
	@mkdir -p ./bin/compiled
	@for file in ./out/*.c; do \
		if [ -f "$$file" ]; then \
			basename=$$(basename $$file .c); \
			echo "Compilando $$basename..."; \
			$(CC) $(CFLAGS) $$file $(SRCDIR)/rational.c $(SRCDIR)/stdj.c $(SRCDIR)/jmath.c -o ./bin/compiled/$$basename || echo "Erro ao compilar $$basename"; \
		fi \
	done
	@echo "--- Compilação concluída! Os executáveis estão em ./bin/compiled/ ---"

.PHONY: compile-single
compile-single:
	@if [ -z "$(FILE)" ]; then \
		echo "Uso: make compile-single FILE=nome_do_arquivo"; \
		echo "Exemplo: make compile-single FILE=prob_1"; \
		exit 1; \
	fi
	@echo "--- Compilando $(FILE).c ---"
	@mkdir -p ./bin/compiled
	@if [ -f "./out/$(FILE).c" ]; then \
		$(CC) $(CFLAGS) ./out/$(FILE).c $(SRCDIR)/rational.c $(SRCDIR)/stdj.c $(SRCDIR)/jmath.c -o ./bin/compiled/$(FILE) && \
		echo "Compilação concluída! Executável: ./bin/compiled/$(FILE)"; \
	else \
		echo "Erro: Arquivo ./out/$(FILE).c não encontrado"; \
		exit 1; \
	fi

.PHONY: help
help:
	@echo "Makefile do J-Lang - Comandos disponíveis:"
	@echo ""
	@echo "  make              - Compila o transpilador J-Lang"
	@echo "  make test         - Executa testes nos arquivos de exemplo"
	@echo "  make compile      - Compila todos os arquivos C gerados"
	@echo "  make compile-single FILE=nome - Compila um arquivo específico"
	@echo "  make clean        - Limpa arquivos gerados do transpilador"
	@echo "  make clean-all    - Limpa todos os arquivos gerados"
	@echo "  make help         - Mostra esta mensagem de ajuda"
	@echo ""
	@echo "Exemplo de uso:"
	@echo "  make compile-single FILE=prob_1"
	@echo "  ./bin/compiled/prob_1"
	@echo ""
