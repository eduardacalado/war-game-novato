#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "war.h"

// Definições de constantes para limites de dados
#define MAX_NOME 30        // Máximo de caracteres para o nome do território
#define MAX_COR 10         // Máximo de caracteres para a cor do exército
#define MAX_TERRITORIOS 5  // Número fixo de territórios no jogo

/*
 * Estrutura que representa um território no jogo.
 * - nome: identificador único do território
 * - cor: exército ao qual pertence
 * - tropas: quantidade de tropas no território
 */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/*
 * Funções auxiliares internas ao módulo WAR.
 * Declaradas como "static" pois só são usadas neste arquivo.
 */
static Territorio* cadastrarTerritorios();
static void exibirTerritorios(const Territorio* territorios, int quantidade);
static void atacar(Territorio* atacante, Territorio* defensor);
static void liberarMemoria(Territorio* mapa);

/*
 * Função principal do jogo WAR.
 * - Inicializa o mapa de territórios via cadastro do jogador.
 * - Exibe o estado inicial.
 * - Permite repetidas fases de ataque até o jogador optar por sair.
 */
void jogarWar() {
    srand(time(NULL)); // Inicializa a semente para números aleatórios (dados)

    printf("========================================\n");
    printf("   WAR ESTRUTURADO - CADASTRO INICIAL \n");
    printf("========================================\n");

    // Criação e cadastro dos territórios
    Territorio* territorios = cadastrarTerritorios();

    // Exibição do estado inicial do mapa
    printf("\n========================================\n");
    printf("     MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("========================================\n");
    exibirTerritorios(territorios, MAX_TERRITORIOS);

    // Fase de ataque: jogadores escolhem atacante e defensor
    printf("\n\n--- FASE DE ATAQUE ---");

    int idxAtacante, idxDefensor;

    while (1) {
        // Escolha do território atacante
        printf("\nEscolha o território atacante (1 a %d, ou 0 para sair): ", MAX_TERRITORIOS);
        scanf("%d", &idxAtacante);

        if (idxAtacante == 0) { // Condição de saída
            printf("\nEncerrando o jogo...\n");
            break;
        }

        // Escolha do território defensor
        printf("Escolha o território defensor (1 a %d): ", MAX_TERRITORIOS);
        scanf("%d", &idxDefensor);

        // Ajuste para índices baseados em 0
        idxAtacante -= 1;
        idxDefensor -= 1;

        // Validação: índices válidos e cores diferentes (não pode atacar aliado)
        if (idxAtacante >= 0 && idxAtacante < MAX_TERRITORIOS &&
            idxDefensor >= 0 && idxDefensor < MAX_TERRITORIOS &&
            strcmp(territorios[idxAtacante].cor, territorios[idxDefensor].cor) != 0) {

            // Executa o ataque
            atacar(&territorios[idxAtacante], &territorios[idxDefensor]);

            // Exibe mapa atualizado
            printf("\n--- Estado atualizado do mapa ---\n");
            exibirTerritorios(territorios, MAX_TERRITORIOS);
        } else {
            printf("\nAtaque inválido: verifique os índices e as cores.\n");
        }

        // Pausa entre turnos
        printf("\n\nPressione Enter para continuar para o próximo turno...");
        getchar();
        getchar();
    }

    // Liberação da memória alocada para o mapa
    liberarMemoria(territorios);
}

/*
 * cadastrarTerritorios()
 * Aloca dinamicamente memória para os territórios
 * e coleta as informações de cada um via entrada do usuário.
 */
static Territorio* cadastrarTerritorios() {
    Territorio* territorios = (Territorio*) calloc(MAX_TERRITORIOS, sizeof(Territorio));
    if (!territorios) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);

        printf("Nome do território: ");
        scanf(" %29[^\n]", territorios[i].nome);

        printf("Cor do exército (ex: Azul, Verde): ");
        scanf(" %9[^\n]", territorios[i].cor);

        // Limpeza de buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Entrada validada do número de tropas
        int valido = 0;
        while (!valido) {
            char buffer[50];
            printf("Número de tropas: ");
            if (fgets(buffer, sizeof(buffer), stdin)) {
                if (sscanf(buffer, "%d", &territorios[i].tropas) == 1 && territorios[i].tropas > 0) {
                    valido = 1;
                } else {
                    printf("Entrada inválida! Digite um número inteiro maior que 0.\n");
                }
            }
        }
    }

    return territorios;
}

/*
 * exibirTerritorios()
 * Mostra no console todos os territórios com nome, cor e número de tropas.
 */
static void exibirTerritorios(const Territorio* territorios, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n%d. %s (Exército %s, Tropas: %d)",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

/*
 * atacar()
 * Realiza a lógica de batalha entre dois territórios:
 * - Cada lado lança um dado (1 a 6).
 * - O maior resultado vence.
 * - Perdedor perde uma tropa e vencedor ganha uma.
 * - Se tropas do defensor chegam a 0, território é conquistado.
 */
static void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- RESULTADO DA BATALHA ---");

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nO atacante %s rolou 1 dado e tirou: %d", atacante->nome, dadoAtacante);
    printf("\nO defensor %s rolou 1 dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas -= 1;
        atacante->tropas += 1;

        // Caso o território defensor seja conquistado
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n",
                   defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1; // Mantém ao menos 1 tropa no território conquistado
        }
    } else {
        printf("VITÓRIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
        atacante->tropas -= 1;
        defensor->tropas += 1;

        // Evita tropas negativas
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/*
 * liberarMemoria()
 * Libera a memória alocada dinamicamente para os territórios.
 */
static void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
