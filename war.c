/*
  WAR Estruturado - Desafio Mestre
  - Integra iniciante + aventureiro + mestre (missões estratégicas)
  - Usa malloc/calloc, ponteiros, modularização
  - Funções principais: alocarMapa, cadastrarTerritorios, exibirMapa,
    atacar, atribuirMissao, verificarMissao, liberarMemoria
  - Missões simples implementadas e avaliadas após cada ataque
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* ------------------- Constantes ------------------- */
#define MAX_NOME 30
#define MAX_COR 10
#define MIN_TERRITORIOS 2
#define MISSIONS_COUNT 5

/* ------------------- Structs ------------------- */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* ------------------- Protótipos ------------------- */

/* Alocação / liberação */
Territorio* alocarMapa(int n);
void liberarMapa(Territorio* mapa);

/* Cadastro / exibição */
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);

/* Ataque */
void atacar(Territorio* atacante, Territorio* defensor);

/* Missões */
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int n, const char* corJogador);
void exibirMissao(const char* missao);

/* Utilitárias */
void limparBuffer(void);
void str_tolower_copy(char* dest, const char* src, size_t dest_size);

/* ------------------- Implementação ------------------- */

int main(void) {
    srand((unsigned)time(NULL));

    printf("=== WAR Estruturado - Desafio Mestre (Missões Estratégicas) ===\n\n");

    int n;
    printf("Informe a quantidade de territorios a cadastrar (min %d): ", MIN_TERRITORIOS);
    if (scanf("%d", &n) != 1 || n < MIN_TERRITORIOS) {
        printf("Entrada invalida. Encerrando.\n");
        return 1;
    }
    limparBuffer();

    /* Aloca mapa dinamicamente usando malloc (requisito do enunciado) */
    Territorio* mapa = alocarMapa(n);
    if (!mapa) {
        printf("Falha na alocacao de memoria.\n");
        return 1;
    }

    /* Cadastro dos territorios */
    cadastrarTerritorios(mapa, n);

    /* Leitura das cores dos dois jogadores (usamos essas cores para
       determinar quem "possuí" cada território) */
    char corJogador1[MAX_COR], corJogador2[MAX_COR];
    printf("\nDigite a cor do jogador 1 (ex: Vermelho): ");
    scanf(" %9[^\n]", corJogador1);
    limparBuffer();
    printf("Digite a cor do jogador 2 (ex: Azul): ");
    scanf(" %9[^\n]", corJogador2);
    limparBuffer();

    /* Vetor de missões possíveis (strings constantes) */
    char* missoes[MISSIONS_COUNT] = {
        "Conquistar 3 territorios",
        "Conquistar 5 territorios",
        "Eliminar todas as tropas da cor Vermelho",
        "Eliminar todas as tropas da cor Azul",
        "Possuir pelo menos 10 tropas no total"
    };

    /* Atribuir uma missão aleatória para cada jogador (armazenada dinamicamente) */
    char* missaoJog1 = NULL;
    char* missaoJog2 = NULL;
    atribuirMissao(&missaoJog1, missoes, MISSIONS_COUNT);
    atribuirMissao(&missaoJog2, missoes, MISSIONS_COUNT);

    printf("\nMissao atribuida ao Jogador 1 (%s):\n", corJogador1);
    exibirMissao(missaoJog1);
    printf("\nMissao atribuida ao Jogador 2 (%s):\n", corJogador2);
    exibirMissao(missaoJog2);

    /* Menu principal: permite atacar, exibir, checar missões e sair.
       Após cada ataque, verificamos se alguma missão foi cumprida. */
    int opcao;
    int jogoAtivo = 1;
    do {
        printf("\n--- Menu ---\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataque\n");
        printf("3 - Verificar missoes agora\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
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

            ia--; id--;
            if (ia < 0 || ia >= n || id < 0 || id >= n) {
                printf("Indices invalidos.\n");
                continue;
            }
            if (ia == id) {
                printf("Nao eh possivel atacar o mesmo territorio.\n");
                continue;
            }

            /* valida cor: não atacar território da própria cor */
            if (strcmp(mapa[ia].cor, mapa[id].cor) == 0) {
                printf("Nao eh permitido atacar um territorio da propria cor.\n");
                continue;
            }
            /* atacante precisa ter pelo menos 2 tropas */
            if (mapa[ia].tropas < 2) {
                printf("Atacante '%s' nao tem tropas suficientes (>=2) para atacar.\n", mapa[ia].nome);
                continue;
            }

            printf("\nSimulando ataque: %s (%s, %d tropas)  ->  %s (%s, %d tropas)\n",
                   mapa[ia].nome, mapa[ia].cor, mapa[ia].tropas,
                   mapa[id].nome, mapa[id].cor, mapa[id].tropas);

            atacar(&mapa[ia], &mapa[id]);

            printf("\nMapa apos o ataque:\n");
            exibirMapa(mapa, n);

            /* Verificar missões silenciosamente após ataque */
            if (verificarMissao(missaoJog1, mapa, n, corJogador1)) {
                printf("\n=== Jogador 1 (%s) CUMPRIU A MISSÃO ===\n", corJogador1);
                exibirMissao(missaoJog1);
                jogoAtivo = 0;
                break;
            }
            if (verificarMissao(missaoJog2, mapa, n, corJogador2)) {
                printf("\n=== Jogador 2 (%s) CUMPRIU A MISSÃO ===\n", corJogador2);
                exibirMissao(missaoJog2);
                jogoAtivo = 0;
                break;
            }

        } else if (opcao == 3) {
            /* Verificação manual de missões */
            int v1 = verificarMissao(missaoJog1, mapa, n, corJogador1);
            int v2 = verificarMissao(missaoJog2, mapa, n, corJogador2);
            printf("\nVerificacao de missoes:\n");
            printf(" Jogador 1 (%s) -> %s\n", corJogador1, v1 ? "Cumprida" : "Nao cumprida");
            printf(" Jogador 2 (%s) -> %s\n", corJogador2, v2 ? "Cumprida" : "Nao cumprida");
            if (v1 && !v2) { jogoAtivo = 0; printf("\nJogador 1 venceu!\n"); break; }
            if (v2 && !v1) { jogoAtivo = 0; printf("\nJogador 2 venceu!\n"); break; }
            if (v1 && v2) { jogoAtivo = 0; printf("\nAmbos cumpriram (empate)!\n"); break; }
        } else if (opcao == 0) {
            printf("Encerrando o jogo...\n");
            break;
        } else {
            printf("Opcao invalida.\n");
        }

    } while (jogoAtivo);

    /* Limpeza: libera mapa e missões alocadas dinamicamente */
    if (missaoJog1) free(missaoJog1);
    if (missaoJog2) free(missaoJog2);
    liberarMapa(mapa);

    printf("\nMemoria liberada. Ate a proxima.\n");
    return 0;
}

/* ------------------- Funcoes ------------------- */

/* alocarMapa:
   Aloca dinamicamente n Territorio usando malloc e retorna ponteiro.
*/
Territorio* alocarMapa(int n) {
    Territorio* mapa = (Territorio*) malloc((size_t)n * sizeof(Territorio));
    if (mapa) {
        /* inicializa campos simples (apenas por segurança) */
        for (int i = 0; i < n; i++) {
            mapa[i].nome[0] = '\0';
            mapa[i].cor[0] = '\0';
            mapa[i].tropas = 0;
        }
    }
    return mapa;
}

/* liberarMapa:
   Libera a memoria alocada para o mapa.
*/
void liberarMapa(Territorio* mapa) {
    free(mapa);
}

/* cadastrarTerritorios:
   Preenche n territorios a partir da entrada do usuario.
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
   Exibe todos os territorios de forma formatada (apenas leitura, recebe const).
*/
void exibirMapa(const Territorio* mapa, int n) {
    printf("\n--- Mapa (%d territorios) ---\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* atacar:
   Simula um duelo com um dado 1..6 para cada lado e atualiza
   os territórios (modificando via ponteiro).
   Regras simples:
   - Se dado atacante > dado defensor: atacante vence
       -> defensor recebe a cor do atacante e metade das tropas do atacante
       -> atacante perde as tropas transferidas
   - Caso contrário (defensor vence ou empate): atacante perde 1 tropa
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("  Resultado dados -> Atacante: %d  |  Defensor: %d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        int transferencia = atacante->tropas / 2;
        if (transferencia < 1) transferencia = 1;
        /* copia a cor do atacante para o defensor */
        strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
        defensor->cor[MAX_COR - 1] = '\0';
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("  VITORIA! %s conquistou %s e transferiu %d tropas.\n",
               atacante->nome, defensor->nome, transferencia);
    } else {
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("  ATAQUE FALHOU. %s perdeu 1 tropa.\n", atacante->nome);
    }
}

/* atribuirMissao:
   Sorteia uma missão do vetor missoes[] e aloca dinamicamente espaço para
   armazenar a cópia da string destino (usa malloc).
   Parâmetro destino: ponteiro para char*; a função modifica o ponteiro (passagem por referência).
*/
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    if (totalMissoes <= 0) {
        *destino = NULL;
        return;
    }
    int idx = rand() % totalMissoes;
    const char* escolhido = missoes[idx];
    size_t len = strlen(escolhido);
    *destino = (char*) malloc(len + 1);
    if (*destino) {
        strcpy(*destino, escolhido);
    }
}

/* exibirMissao:
   Exibe a missão (passagem por valor, apenas leitura).
*/
void exibirMissao(const char* missao) {
    if (!missao) {
        printf("  (Nenhuma missao atribuida)\n");
        return;
    }
    printf("  Missao: %s\n", missao);
}

/* verificarMissao:
   Implementa a lógica de verificação para as missões definidas no vetor.
   - Recebe a missão (string), o mapa de territorios e a cor do jogador.
   - Retorna 1 se a missão foi cumprida, 0 caso contrário.
   Lógica implementada (simples, conforme enunciado):
     * "Conquistar 3 territorios" -> conta territorios com cor == corJogador >= 3
     * "Conquistar 5 territorios" -> >= 5
     * "Eliminar todas as tropas da cor Vermelho" -> verifica que não existe territorio com cor "Vermelho" e tropas > 0
     * "Eliminar todas as tropas da cor Azul" -> idem para "Azul"
     * "Possuir pelo menos 10 tropas no total" -> soma tropas de territorios com cor == corJogador >= 10
*/
int verificarMissao(const char* missao, const Territorio* mapa, int n, const char* corJogador) {
    if (!missao || !mapa || !corJogador) return 0;

    /* normaliza strings para comparacao simples (minúsculas) */
    char missao_low[128];
    char cor_low[MAX_COR];
    str_tolower_copy(missao_low, missao, sizeof(missao_low));
    str_tolower_copy(cor_low, corJogador, sizeof(cor_low));

    if (strstr(missao_low, "conquistar 3")) {
        int cont = 0;
        for (int i = 0; i < n; i++) {
            char temp[MAX_COR];
            str_tolower_copy(temp, mapa[i].cor, sizeof(temp));
            if (strcmp(temp, cor_low) == 0) cont++;
        }
        return cont >= 3;
    } else if (strstr(missao_low, "conquistar 5")) {
        int cont = 0;
        for (int i = 0; i < n; i++) {
            char temp[MAX_COR];
            str_tolower_copy(temp, mapa[i].cor, sizeof(temp));
            if (strcmp(temp, cor_low) == 0) cont++;
        }
        return cont >= 5;
    } else if (strstr(missao_low, "eliminar todas as tropas da cor vermelho")) {
        for (int i = 0; i < n; i++) {
            char temp[MAX_COR];
            str_tolower_copy(temp, mapa[i].cor, sizeof(temp));
            if (strcmp(temp, "vermelho") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    } else if (strstr(missao_low, "eliminar todas as tropas da cor azul")) {
        for (int i = 0; i < n; i++) {
            char temp[MAX_COR];
            str_tolower_copy(temp, mapa[i].cor, sizeof(temp));
            if (strcmp(temp, "azul") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    } else if (strstr(missao_low, "possuir pelo menos 10 tropas")) {
        int soma = 0;
        for (int i = 0; i < n; i++) {
            char temp[MAX_COR];
            str_tolower_copy(temp, mapa[i].cor, sizeof(temp));
            if (strcmp(temp, cor_low) == 0) soma += mapa[i].tropas;
        }
        return soma >= 10;
    }

    /* Missão desconhecida por padrão não cumprida */
    return 0;
}

/* limparBuffer:
   Consome o restante da linha no stdin para evitar leituras quebradas por scanf.
*/
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* str_tolower_copy:
   Copia src para dest convertendo para minúsculas, garantindo terminador.
*/
void str_tolower_copy(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return;
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = (char) tolower((unsigned char) src[i]);
    }
    dest[i] = '\0';
}
