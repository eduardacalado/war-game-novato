#ifndef DETECTIVE_H
#define DETECTIVE_H

// --- limites ---
#define MAX_NOME_SALA 64
#define MAX_TEXTO_PISTA 128
#define HASH_SIZE 101   // tamanho da tabela hash (primo razoável)

// --- Estruturas ---
// Sala (nó da árvore que representa mapa)
typedef struct Sala {
    char nome[MAX_NOME_SALA];
    char pista[MAX_TEXTO_PISTA]; // texto da pista (pode estar vazio)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da BST para pistas coletadas
typedef struct PistaNode {
    char pista[MAX_TEXTO_PISTA];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Entrada da tabela hash (cadeia para colisões)
typedef struct HashEntry {
    char pista[MAX_TEXTO_PISTA];   // chave: texto da pista
    char suspeito[64];             // valor: nome do suspeito associado
    struct HashEntry *prox;
} HashEntry;

// Tabela hash (vetor de ponteiros para HashEntry)
typedef struct {
    HashEntry *vet[HASH_SIZE];
} HashTable;

// Protótipos principais
Sala* criarSala(const char *nome, const char *pista);
void liberarMapa(Sala *root);

void inserirPistaBST(PistaNode **raiz, const char *pista);
void exibirPistasInOrder(PistaNode *raiz);
void liberarPistasBST(PistaNode *raiz);

void initHash(HashTable *h);
void inserirNaHash(HashTable *h, const char *pista, const char *suspeito);
const char* encontrarSuspeito(HashTable *h, const char *pista);
int contarPistasParaSuspeito(HashTable *h, PistaNode *raiz, const char *suspeito);

void explorarSalas(Sala *inicio, PistaNode **raizPistas, HashTable *h);
void jogarDetective(); // função pública que monta tudo e roda o jogo

#endif // DETECTIVE_H
