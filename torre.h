#ifndef TORRE_H
#define TORRE_H

#define MAX_COMP 10
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade;
} Componente;

void jogarTorre();

void adicionarComponente(Componente comp[], int *qtd);
void descartarComponente(Componente comp[], int *qtd);
void listarComponentes(Componente comp[], int qtd);

void bubbleSortNome(Componente comp[], int qtd, int *comparacoes);
void insertionSortTipo(Componente comp[], int qtd, int *comparacoes);
void selectionSortPrioridade(Componente comp[], int qtd, int *comparacoes);

int buscaBinariaPorNome(Componente comp[], int qtd, char chave[], int *comparacoes);

#endif
