## J-Lang

## Pre-requesitos

- Ter o lex instalado
- Ter o GCC ou outro compilador de C de sua prefência instalado

Para compilar a linguagem, basta executar:

```bash
make
```
E será gerado um binário na pasta `bin/` com o nome `j-lang`

Na pasta `examples/` estão disponíveis alguns exemplos de programas feitos na linguagem. Entre eles estão:

- hello.j
- mergesort.j
- fib.j

Para executar o parser da linguagem em um dos exemplos, basta executar: 
`./bin/j-lang < arquivo.j`, isto é

```
./bin/j-lang < hello.j
./bin/j-lang < mergesort.j
./bin/j-lang < fib.j
```
