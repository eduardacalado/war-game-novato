#include <stdio.h>
#include "war.h"        // Header do jogo WAR
#include "torre.h"      // Header do jogo Torre de Fuga
#include "tetris.h"     // Header do jogo Tetris Stack
#include "detective.h"  // Header do jogo Detective Quest

/*
 * Função principal do programa.
 * Exibe um menu inicial permitindo que o jogador escolha entre os jogos disponíveis.
 * Cada opção chama a função correspondente ao jogo selecionado.
 * O programa continua exibindo o menu até que o jogador escolha a opção "0" para sair.
 */
int main() {
    int opcao; // Armazena a escolha do usuário no menu

    do {
        // Exibição do menu principal
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

        // Estrutura de controle para chamar cada jogo
        switch (opcao) {
            case 1:
                jogarWar();          // Chama o jogo WAR
                break;
            case 2:
                jogarTorre();        // Chama o jogo Torre de Fuga
                break;
            case 3:
                jogarTetris();       // Chama o jogo Tetris Stack
                break;
            case 4:
                jogarDetective();    // Chama o jogo Detective Quest
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n"); // Caso o jogador digite um número inválido
        }
    } while (opcao != 0); // Continua até que o usuário escolha sair

    return 0; // Finaliza o programa
}
