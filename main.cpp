#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10
#define MAX_TERRITORIOS 5

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

Territorio* cadastrarTerritorios();
void exibirTerritorios(const Territorio* territorios, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main(void) {
    srand(time(NULL));

    printf("========================================\n");
    printf("   WAR ESTRUTURADO - CADASTRO INICIAL \n");
    printf("========================================\n");

    Territorio* territorios = cadastrarTerritorios();

    printf("\n========================================\n");
    printf("     MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("========================================\n");
    exibirTerritorios(territorios, MAX_TERRITORIOS);

    printf("\n\n--- FASE DE ATAQUE ---");

    int idxAtacante, idxDefensor;

    while (1) {
        printf("\nEscolha o território atacante (1 a %d, ou 0 para sair): ", MAX_TERRITORIOS);
        scanf("%d", &idxAtacante);

        if (idxAtacante == 0) {
            printf("\nEncerrando o jogo...\n");
            break;
        }

        printf("Escolha o território defensor (1 a %d): ", MAX_TERRITORIOS);
        scanf("%d", &idxDefensor);

        idxAtacante -= 1;
        idxDefensor -= 1;

        if (idxAtacante >= 0 && idxAtacante < MAX_TERRITORIOS &&
            idxDefensor >= 0 && idxDefensor < MAX_TERRITORIOS &&
            strcmp(territorios[idxAtacante].cor, territorios[idxDefensor].cor) != 0) {

            atacar(&territorios[idxAtacante], &territorios[idxDefensor]);
            printf("\n--- Estado atualizado do mapa ---\n");
            exibirTerritorios(territorios, MAX_TERRITORIOS);
            } else {
                printf("\nAtaque inválido: verifique os índices e as cores.\n");
            }

        printf("\n\nPressione Enter para continuar para o próximo turno...");
        getchar();
        getchar();
    }

    liberarMemoria(territorios);
    return 0;
}


Territorio* cadastrarTerritorios() {
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
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

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

void exibirTerritorios(const Territorio* territorios, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n%d. %s (Exército %s, Tropas: %d)", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- RESULTADO DA BATALHA ---");

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nO atacante %s rolou 1 dado e tirou: %d", atacante->nome, dadoAtacante);
    printf("\nO defensor %s rolou 1 dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->tropas -= 1;
        atacante->tropas += 1;
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! O território %S foi dominado pelo Exército %s!.\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1;
        }
    } else {
        printf("VITÓRIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
        atacante->tropas -= 1;
        defensor->tropas += 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
