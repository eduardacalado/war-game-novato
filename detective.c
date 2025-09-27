#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detective.h"

// Cria uma nova sala
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Explora as salas interativamente
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        // Se não houver mais caminhos
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este é um cômodo sem saídas. A exploração terminou!\n");
            return;
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

// Função principal do jogo Detective Quest
void jogarDetective() {
    // Montando manualmente a mansão (árvore binária)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *quarto = criarSala("Quarto Secreto");

    // Conectando as salas
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    cozinha->direita = quarto;

    // Início da exploração
    printf("\n=== Detective Quest ===\n");
    printf("Você começa no Hall de Entrada.\n");
    explorarSalas(hall);

    // Liberação da memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(jardim);
    free(biblioteca);
    free(quarto);
}
