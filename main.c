#include <stdio.h>
#include "war.h"
#include "torre.h"
#include "tetris.h"
#include "detective.h"

int main() {
    int opcao;

    do {
        printf("\n========================\n");
        printf("    MENU PRINCIPAL");
        printf("\n========================\n");
        printf("1. Jogar WAR\n");
        printf("2. Torre de Fuga\n");
        printf("3. Tetris Stack\n");
        printf("4. Detective Quest\n");
        printf("0. Sair\n");
        printf("Escolha o jogo: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarWar();
                break;
            case 2:
                jogarTorre();
                break;
            case 3:
                jogarTetris();
                break;
            case 4:
                jogarDetective();
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
