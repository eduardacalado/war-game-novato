#ifndef DETECTIVE_H
#define DETECTIVE_H

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;  // Caminho à esquerda
    struct Sala *direita;   // Caminho à direita
} Sala;

// Cria uma sala dinamicamente
Sala* criarSala(const char *nome);

// Explora a mansão a partir do Hall
void explorarSalas(Sala *atual);

// Função principal do jogo
void jogarDetective();

#endif
