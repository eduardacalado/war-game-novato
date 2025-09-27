#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detective.h"

// ---------------- Criação da Mansão ----------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ---------------- BST de Pistas ----------------
PistaNode* criarNoPista(const char *pista) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista);
    novo->esq = novo->dir = NULL;
    return novo;
}

void inserirPista(PistaNode **raiz, const char *pista) {
    if (*raiz == NULL) {
        *raiz = criarNoPista(pista);
    } else if (strcmp(pista, (*raiz)->pista) < 0) {
        inserirPista(&((*raiz)->esq), pista);
    } else if (strcmp(pista, (*raiz)->pista) > 0) {
        inserirPista(&((*raiz)->dir), pista);
    }
    // se for igual, não insere de novo
}

void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esq);
        liberarPistas(raiz->dir);
        free(raiz);
    }
}

// ---------------- Exploração ----------------
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        // Coleta a pista, se houver
        if (strlen(atual->pista) > 0) {
            printf("💡 Você encontrou uma pista: \"%s\"\n", atual->pista);
            inserirPista(raizPistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada aqui.\n");
        }

        printf("Escolha seu caminho (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda!\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita!\n");
            }
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            return;
        } else {
            printf("Opção inválida! Use e, d ou s.\n");
        }
    }
}

// ---------------- Jogo Principal ----------------
void jogarDetective() {
    // Montando a mansão
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas");
    Sala *salaEstar = criarSala("Sala de Estar", "Um lenço de seda");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *jardim = criarSala("Jardim", "Um bilhete rasgado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro com páginas arrancadas");
    Sala *quarto = criarSala("Quarto Secreto", "Chave dourada");

    // Ligações
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    cozinha->direita = quarto;

    // Árvore BST de pistas
    PistaNode *raizPistas = NULL;

    printf("\n=== Detective Quest: Coleta de Pistas ===\n");
    explorarSalasComPistas(hall, &raizPistas);

    // Exibir pistas coletadas
    printf("\n📜 Pistas coletadas (em ordem alfabética):\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista encontrada.\n");
    } else {
        exibirPistas(raizPistas);
    }

    // Liberar memória
    liberarPistas(raizPistas);
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(biblioteca);
    free(quarto);
}
