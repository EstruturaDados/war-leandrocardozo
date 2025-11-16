#include <stdio.h>
#include <string.h>

/* Define a quantidade de territórios que serão cadastrados */
#define QTD_TERRITORIOS 5

/* Definição da struct Territorio conforme solicitado:
   - nome: campo char[30]
   - cor: campo char[10]
   - tropas: campo int
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main(void) {
    Territorio territorios[QTD_TERRITORIOS]; /* vetor para armazenar 5 Territorio */
    int i;

    printf("=== Cadastro de Territorios (Jogo War - Nivel Iniciante) ===\n\n");

    /* Laço para cadastrar os 5 territórios */
    for (i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d de %d\n", i + 1, QTD_TERRITORIOS);

        /* Leitura do nome: permite espaços até 29 caracteres */
        printf("Informe o nome do territorio: ");
        scanf(" %29[^\n]", territorios[i].nome); /* ' %[^\n]' lê até newline (aceita espaços) */

        /* Leitura da cor do exército: permite até 9 caracteres (ex.: 'Azul') */
        printf("Informe a cor do exercito: ");
        scanf(" %9[^\n]", territorios[i].cor);

        /* Leitura do número de tropas (inteiro) */
        printf("Informe a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        /* Linha em branco para separar entradas e melhorar usabilidade */
        printf("\n");
    }

    /* Exibição dos dados cadastrados */
    printf("=== Territorios Cadastrados ===\n");
    for (i = 0; i < QTD_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome : %s\n", territorios[i].nome);
        printf("  Cor  : %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("\n");
    }

    return 0;
}
