## J-Lang

## Pre-requesitos

- Ter o lex instalado
- Ter o GCC ou outro compilador de C de sua prefência instalado

Para compilar o lexer, basta executar os seguintes comandos:

```bash
lex j-lang.l
gcc lex.yy.c -o j-lang-lexer
```

Nessa pasta, você pode encontrar a implementação dos seguintes programas:

- hello.j
- mergesort.j
- fib.j

Para ver o resultado do lexer nesses arquivos basta executar, `./j-lang-lexer < arquivo.j`, isto é

```
./j-lang-lexer < hello.j
./j-lang-lexer < mergesort.j
./j-lang-lexer < fib.j
```
