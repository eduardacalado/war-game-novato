#ifndef DETECTIVE_H
#define DETECTIVE_H

// Estrutura de sala (mapa da mansão)
typedef struct Sala {
    char nome[50];
    char pista[100];          // pista encontrada na sala (pode ser vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST para armazenar pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// ---- Funções principais ----
Sala* criarSala(const char *nome, const char *pista);
void explorarSalasComPistas(Sala *inicio, PistaNode **raizPistas);
void inserirPista(PistaNode **raiz, const char *pista);
void exibirPistas(PistaNode *raiz);
void liberarPistas(PistaNode *raiz);
void jogarDetective();

#endif
