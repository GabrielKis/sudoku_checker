# Verificação de um quebra-cabeça de sudoku

## Utilização

Este repositório contém um arquivo que verifica se um Sudoku está correto. Para executá-lo é necessário compilar e executar o arquivo `sudoku_verifier.c`.

Comando para compilação:
``` bash
gcc -pthread -Wall -o sudoku_check sudoku_verifier.c
```

Comando para execução:
``` bash
./sudoku_check
```

O conjunto de dados contendo os quebra-cabeça já resolvidos foram obtidos de: https://www.kaggle.com/bryanpark/sudoku

O script em python `separate_quizzes_solutions.py` realiza uma separação dos dados, já que apenas são desejadas as soluções.

## Vídeo

TODO: Colocar Link para vídeo

## Melhoria

Fazer com que seja possível conferir toda a base de dados de uma vez (1.000.000 de quebra-cabeças)
