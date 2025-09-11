#include <stdio.h>
#include <string.h>

#define MAX_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

void cadastrarTerritorios(Territorio territorios[], int quantidade);
void exibirTerritorios(const Territorio territorios[], int quantidade);

int main(void) {
    Territorio territorios[MAX_TERRITORIOS];

    printf("========================================\n");
    printf("Vamos cadastrar os %d territórios iniciais do nosso mundo.\n\n", MAX_TERRITORIOS);

    cadastrarTerritorios(territorios, MAX_TERRITORIOS);

    printf("\nCadastro inicial concluído com sucesso!\n");
    printf("\n=======================================\n");
    printf("\n    MAPA DO MUNDO - ESTADO, ATUAL\n");
    printf("\n=======================================\n");
    exibirTerritorios(territorios, MAX_TERRITORIOS);

    return 0;
}

void cadastrarTerritorios(Territorio territorios[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Cadastrando Território %d ---", i + 1);

        printf("\nNome do território: ");
        scanf(" %29[^\n]", territorios[i].nome);

        printf("Cor do exército (ex: Azul, Verde): ");
        scanf(" %9s", territorios[i].cor);

        while (1) {
            printf("Número de tropas: ");
            if (scanf("%d", &territorios[i].tropas) == 1 && territorios[i].tropas >= 0) {
                break;
            } else {
                printf("Entrada inválida. Por favor, digite um número inteiro não negativo.\n");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) { }
            }
        }

    }
}

void exibirTerritorios(const Territorio territorios[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf(" - Nome: %s\n", territorios[i].nome);
        printf(" - Cor: %s\n", territorios[i].cor);
        printf(" - Tropas: %d\n", territorios[i].tropas);
    }
}
