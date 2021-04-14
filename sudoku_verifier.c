#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define QTD_LINHAS 9
#define QTD_COLUNAS 9
#define QTD_QUADRADOS 9
#define TAMANHO_QUADRADO 3

#define EXPECTED_SUDOKU_LENGH_DATABASE (9*9) + 1

int sudoku_m[QTD_LINHAS][QTD_COLUNAS];
// Implementar de modo que se possa retornar valor atraves das threads
int valid_sudoku = 1;

int read_database(){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int aux_number;

    fp = fopen("puzzle_solutions.txt", "r");

    // Ler apenas um valor
    read = getline(&line, &len, fp);
    if (read != EXPECTED_SUDOKU_LENGH_DATABASE){
        return 1;
    }

    printf("Sudoku a ser verificado:\n\n");
    for(int linha=0; linha<QTD_LINHAS; linha++){
        // Espacamento a cada quadrado
        if (linha%3 == 0 && linha!=0)
            printf("\n");
        for(int coluna=0; coluna<QTD_COLUNAS; coluna++){
            aux_number = line[coluna + (QTD_COLUNAS)*linha] - 48;
            if (aux_number <= 0 || aux_number > 9)
                return 1;
            sudoku_m[linha][coluna] = aux_number;
            // Espacamento a cada quadrado
            if (coluna%3 == 0 && coluna!=0)
                printf(" ");
            printf("%d ", sudoku_m[linha][coluna]);
        }
        printf("\n");
    }
    printf("\n");
/*
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
*/
    fclose(fp);
    if (line)
        free(line);
    return 0;
}

void* checa_linha_n(void *linha_n){
    int algarismos_por_linha[QTD_LINHAS];
    int l = (int) linha_n;
    int key;

    for(int i=0; i<QTD_LINHAS; i++){
        algarismos_por_linha[i] = 0;
    }

    for(int i=0; i<QTD_LINHAS; i++){
        key = sudoku_m[l][i];
        if (algarismos_por_linha[key] == 1){
            // existem algarismos repetidos por linha
            printf("Linha %d inválida\n", l);
            valid_sudoku = 0;
            return NULL;
        }else{
            algarismos_por_linha[key] = 1;
        }
    }
    return NULL; 
}

void* checa_coluna_n(void *col_n){
    int algarismos_por_coluna[QTD_COLUNAS];
    int c = (int) col_n;
    int key;

    for(int i=0; i<QTD_COLUNAS; i++){
        algarismos_por_coluna[i] = 0;
    }

    for(int i=0; i<QTD_COLUNAS; i++){
        key = (sudoku_m[i][c] % 10) - 1;
        if (algarismos_por_coluna[key] == 1){
            // existem algarismos repetidos por coluna
            printf("Coluna %d inválida\n", c);
            valid_sudoku = 0;
            return NULL;
        }else{
            algarismos_por_coluna[key] = 1;
        }
    }
    return NULL; 
}

void* checa_quadrado_n(void *quadrado_n){
    int quadrado_id = (int) quadrado_n;
    int key;
    int linha_inicial_quadrado = (int) (quadrado_id/3) * 3;
    int coluna_inicial_quadrado = (int) (quadrado_id%3) * 3;
    int algarismos_por_quadrado[QTD_QUADRADOS];

    for(int i=0; i<QTD_QUADRADOS; i++){
        algarismos_por_quadrado[i] = 0;
    }

    for(int linha=linha_inicial_quadrado; linha<(linha_inicial_quadrado + TAMANHO_QUADRADO); linha++){
        for(int coluna=coluna_inicial_quadrado; coluna<(coluna_inicial_quadrado + TAMANHO_QUADRADO); coluna++){
            key = (sudoku_m[linha][coluna] % 10) - 1;
            if (algarismos_por_quadrado[key] == 1){
                // existem algarismos repetidos por coluna
                printf("Quadrado %d invalido\n", quadrado_id);
                valid_sudoku = 0;
                return NULL;
            }else{
                algarismos_por_quadrado[key] = 1;
            }
        }
    }
    return NULL;
}

void* checagem_linhas(){
    pthread_t check_line[QTD_LINHAS];
    for (int i=0; i<1; i++){
        // Cria uma thread para cada linha a ser verificada
        pthread_create(&check_line[i], NULL, checa_linha_n, (void*) i);
    }
    for (int i=0; i<QTD_LINHAS; i++){
        pthread_join(check_line[i], NULL);
    }
    return NULL;
}

void* checagem_colunas(){
    pthread_t check_col[QTD_COLUNAS];
    for (int i=0; i<QTD_COLUNAS; i++){
        // Cria uma thread para cada coluna a ser verificada
        pthread_create(&check_col[i], NULL, checa_coluna_n, (void*) i);
    }
    for (int i=0; i<QTD_COLUNAS; i++){
        pthread_join(check_col[i], NULL);
    }
    return NULL;
}

void* checagem_quadrados(){
    pthread_t check_quad[QTD_QUADRADOS];
    for (int i=0; i<QTD_QUADRADOS; i++){
        // Cria uma thread para cada quadrado a ser verificada
        pthread_create(&check_quad[i], NULL, checa_quadrado_n, (void*) i);
    }
    for (int i=0; i<QTD_QUADRADOS; i++){
        pthread_join(check_quad[i], NULL);
    }
    return NULL;
}

int main(){
    pthread_t linhas;
    pthread_t colunas;
    pthread_t quadrados;

    if(read_database()){
        printf("Erro em ler sudoku, terminando o programa");
        return 0;
    }

    // thread que checa linhas
    pthread_create(&linhas, NULL, checagem_linhas, NULL);
    // thread que checa colunas
    pthread_create(&colunas, NULL, checagem_colunas, NULL);
    // thread que checa quadrados
    pthread_create(&quadrados, NULL, checagem_quadrados, NULL);

    pthread_join(linhas, NULL);
    pthread_join(colunas, NULL);
    pthread_join(quadrados, NULL);

    if (valid_sudoku){
        printf("\nSudoku correto.\n");
    }else{
        printf("\nSudoku incorreto.\n");
    }

    return 0;
}