#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tetris.h"

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

// Gera uma nova peça
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Insere peça no final da fila (enqueue)
void enqueue(Fila *f, Peca p) {
    if (f->qtd == MAX_FILA) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->fim = (f->fim + 1) % MAX_FILA;
    f->pecas[f->fim] = p;
    f->qtd++;
}

// Remove peça da frente (dequeue)
Peca dequeue(Fila *f) {
    if (f->qtd == 0) {
        printf("Fila vazia! Não há peça para jogar.\n");
        return (Peca){' ', -1};
    }
    Peca p = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return p;
}

// Exibe estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de peças: ");
    if (f->qtd == 0) {
        printf("[vazia]");
    } else {
        int i, idx = f->inicio;
        for (i = 0; i < f->qtd; i++) {
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
            idx = (idx + 1) % MAX_FILA;
        }
    }
    printf("\n");
}

// Função principal do jogo Tetris Stack
void jogarTetris() {
    Fila fila;
    inicializarFila(&fila);
    int idCounter = 0;

    // inicializa a fila com 5 peças
    srand(time(NULL));
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idCounter++));
    }

    int opcao;
    do {
        exibirFila(&fila);
        printf("\n--- Menu Tetris Stack ---\n");
        printf("1. Jogar peça (dequeue)\n");
        printf("2. Inserir nova peça (enqueue)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1)
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                break;
            }
            case 2:
                enqueue(&fila, gerarPeca(idCounter++));
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}
