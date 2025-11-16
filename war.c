#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
  WAR Estruturado - Aventureiro (ataque entre territórios)
  - Usa alocação dinâmica (calloc)
  - Usa ponteiros para manipular territórios
  - Função atacar(atacante, defensor) com rand() simulando dados 1..6
  - Ao vencer, atacante transfere cor e metade das tropas para o defensor
  - Ao perder, atacante perde 1 tropa
  - Tudo comentado e simples para nível iniciante/aventureiro
*/

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Protótipos */
Territorio* alocarMapa(int n);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
void limparBuffer(void);

/* ========== Função main ========== */
int main(void) {
    int n;
    srand((unsigned)time(NULL)); /* semente para rand */

    printf("=== WAR Estruturado - Simulador de Ataque (Aventureiro) ===\n\n");
    printf("Informe a quantidade de territorios a cadastrar (min 2): ");
    if (scanf("%d", &n) != 1 || n < 2) {
        printf("Entrada invalida. Encerrando.\n");
        return 1;
    }
    limparBuffer();

    /* Aloca mapa dinamicamente */
    Territorio* mapa = alocarMapa(n);
    if (!mapa) {
        printf("Erro de alocacao de memoria.\n");
        return 1;
    }

    /* Cadastro inicial */
    cadastrarTerritorios(mapa, n);

    /* Loop principal para atacar / exibir / sair */
    int opcao;
    do {
        printf("\n--- Menu ---\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            liberarMemoria(mapa);
            return 1;
        }
        limparBuffer();

        if (opcao == 1) {
            exibirMapa(mapa, n);
        } else if (opcao == 2) {
            exibirMapa(mapa, n);
            int ia, id;
            printf("Indice do territorio atacante (1..%d): ", n);
            if (scanf("%d", &ia) != 1) { limparBuffer(); printf("Entrada invalida.\n"); continue; }
            limparBuffer();
            printf("Indice do territorio defensor (1..%d): ", n);
            if (scanf("%d", &id) != 1) { limparBuffer(); printf("Entrada invalida.\n"); continue; }
            limparBuffer();

            /* converte para índice 0-based e valida */
            ia--; id--;
            if (ia < 0 || ia >= n || id < 0 || id >= n) {
                printf("Indices invalidos.\n");
                continue;
            }
            if (ia == id) {
                printf("Nao eh possivel atacar o mesmo territorio.\n");
                continue;
            }

            /* valida cor (nao atacar território da propria cor) */
            if (strcmp(mapa[ia].cor, mapa[id].cor) == 0) {
                printf("Nao eh permitido atacar um territorio da propria cor.\n");
                continue;
            }

            /* atacante precisa ter pelo menos 2 tropas para atacar (simples regra) */
            if (mapa[ia].tropas < 2) {
                printf("Atacante '%s' nao tem tropas suficientes para atacar (precisa ter >= 2).\n", mapa[ia].nome);
                continue;
            }

            printf("\nSimulando ataque: %s (cor %s, tropas %d)  ->  %s (cor %s, tropas %d)\n",
                   mapa[ia].nome, mapa[ia].cor, mapa[ia].tropas,
                   mapa[id].nome, mapa[id].cor, mapa[id].tropas);

            /* chama atacar com ponteiros */
            atacar(&mapa[ia], &mapa[id]);

            /* mostra mapa atualizado */
            printf("\nMapa apos o ataque:\n");
            exibirMapa(mapa, n);
        } else if (opcao == 0) {
            printf("Encerrando o jogo. Liberando memoria...\n");
        } else {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    /* libera memória e finaliza */
    liberarMemoria(mapa);
    return 0;
}

/* ========== Implementacao das funções ========== */

/* alocarMapa:
   Usa calloc para alocar n Territorio e inicializar a zero.
*/
Territorio* alocarMapa(int n) {
    Territorio* mapa = (Territorio*) calloc((size_t)n, sizeof(Territorio));
    return mapa;
}

/* cadastrarTerritorios:
   Lê do usuario nome, cor e tropas para cada territorio.
   Usa scanf com limites para evitar overflow.
*/
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nCadastro do territorio %d de %d\n", i + 1, n);
        printf("  Nome: ");
        scanf(" %29[^\n]", mapa[i].nome);
        limparBuffer();
        printf("  Cor do exercito: ");
        scanf(" %9[^\n]", mapa[i].cor);
        limparBuffer();
        printf("  Quantidade de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            printf("  Entrada invalida. Digite um numero inteiro: ");
            limparBuffer();
        }
        limparBuffer();
    }
}

/* exibirMapa:
   Mostra de forma limpa os territorios. Recebe const para indicar leitura apenas.
*/
void exibirMapa(const Territorio* mapa, int n) {
    printf("\n--- Mapa (%d territorios) ---\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* atacar:
   Simula um duelo com um dado 1..6 para cada lado.
   - Se atacante vence (dado maior), ele conquista: defensor recebe a cor do atacante
     e passa para o defensor metade (inteira) das tropas do atacante.
     O atacante perde esse número de tropas transferidas.
   - Se atacante perde (ou empata), atacante perde 1 tropa.
   Observação: usa ponteiros para modificar os territórios diretamente.
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("  Resultado dados -> Atacante: %d  |  Defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        /* atacante vence */
        int transferencia = atacante->tropas / 2;
        if (transferencia < 1) transferencia = 1; /* garante pelo menos 1 sendo transferido */
        /* atualiza defensor */
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor)-1);
        defensor->cor[sizeof(defensor->cor)-1] = '\0';
        defensor->tropas = transferencia;
        /* atualiza atacante */
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("  VITORIA! %s conquistou %s e transferiu %d tropas.\n", atacante->nome, defensor->nome, transferencia);
    } else {
        /* atacante perde (inclui empate) */
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("  ATAQUE FALHOU. %s perdeu 1 tropa.\n", atacante->nome);
    }
}

/* liberarMemoria:
   Libera o ponteiro alocado com calloc/malloc.
*/
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

/* limparBuffer:
   Consome o resto da linha no stdin para evitar leituras quebradas por scanf.
*/
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
