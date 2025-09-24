#ifndef TETRIS_H
#define TETRIS_H

#define MAX_FILA 5

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[MAX_FILA];
    int inicio, fim, qtd;
} Fila;

void jogarTetris();
void inicializarFila(Fila *f);
Peca gerarPeca(int id);
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
void exibirFila(Fila *f);

#endif
