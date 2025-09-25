#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------- Constantes ----------------
#define MAX_FILA 5    // tamanho fixo da fila
#define MAX_PILHA 3   // capacidade da pilha de reserva

// ---------------- Estruturas ----------------
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[MAX_FILA];
    int inicio, fim, qtd;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// ---------------- Funções auxiliares ----------------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->qtd = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Gera uma nova peça aleatória
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
        printf("Erro: Fila cheia (não deveria acontecer, pois mantemos sempre cheia).\n");
        return;
    }
    f->fim = (f->fim + 1) % MAX_FILA;
    f->pecas[f->fim] = p;
    f->qtd++;
}

// Remove peça da frente da fila (dequeue)
Peca dequeue(Fila *f) {
    if (f->qtd == 0) {
        printf("Fila vazia!\n");
        return (Peca){' ', -1};
    }
    Peca p = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->qtd--;
    return p;
}

// Empilha uma peça (push)
void push(Pilha *p, Peca nova) {
    if (p->topo == MAX_PILHA - 1) {
        printf("Pilha de reserva cheia! Não é possível reservar.\n");
        return;
    }
    p->pecas[++(p->topo)] = nova;
}

// Desempilha uma peça (pop)
Peca pop(Pilha *p) {
    if (p->topo == -1) {
        printf("Pilha de reserva vazia!\n");
        return (Peca){' ', -1};
    }
    return p->pecas[(p->topo)--];
}

// Exibir estado atual (fila + pilha)
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n================== ESTADO ATUAL ==================\n");
    printf("Fila de peças: ");
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

    printf("Pilha de reserva (Topo -> Base): ");
    if (p->topo == -1) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n==================================================\n");
}

// ---------------- Função principal ----------------
void jogarTetris() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idCounter = 0;
    srand(time(NULL));

    // Preenche a fila inicialmente com MAX_FILA peças
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idCounter++));
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);

        printf("\n--- Menu Tetris Stack ---\n");
        printf("1. Jogar peça (da fila)\n");
        printf("2. Reservar peça (fila -> pilha)\n");
        printf("3. Usar peça reservada (da pilha)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(idCounter++));
                }
                break;
            }
            case 2: { // Reservar peça
                if (fila.qtd > 0 && pilha.topo < MAX_PILHA - 1) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(idCounter++));
                } else {
                    printf("Não foi possível reservar.\n");
                }
                break;
            }
            case 3: { // Usar peça reservada
                Peca usada = pop(&pilha);
                if (usada.id != -1) {
                    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 0:
                printf("Saindo do Tetris Stack...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}
