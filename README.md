## J-Lang

## Pre-requesitos

- Ter o lex instalado
- Ter o GCC ou outro compilador de C de sua preferência instalado

Para gerar o compilador da linguagem, basta executar:

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

./bin/j-lang < hello.j
./bin/j-lang < mergesort.j
./bin/j-lang < fib.j

Alternativamente, execute o comando `make test` na raiz do projeto para tentar compilar todos os arquivos no diretório `./examples/` e gerar os arquivos de saída com o resultado da checagem e transpilação no diretório `./out/`.

Se usada a segunda forma de gerar os arquivos, é possível tentar compilar os arquivos .c gerados usando os seguintes comandos:

- make compile-single FILE=nome_do_arquivo
- make compile 

Para gerar os executáveis de um arquivo específico, ou de todos os arquivos, respectivamente.
```
