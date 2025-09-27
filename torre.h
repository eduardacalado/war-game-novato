#ifndef TORRE_H
#define TORRE_H

/**
 * @file torre.h
 * @brief Definições e protótipos para o jogo "Torre de Fuga".
 *
 * Este módulo implementa a mecânica de inventário e organização de componentes,
 * onde o jogador coleta, descarta, organiza e busca itens para ativar a torre
 * e escapar da ilha.
 *
 * Principais funcionalidades:
 * - Manipulação de componentes (adicionar, descartar e listar).
 * - Ordenação de inventário por Nome, Tipo ou Prioridade usando diferentes algoritmos.
 * - Busca binária por componentes (quando a mochila estiver ordenada por Nome).
 */

/** @brief Limite máximo de componentes no inventário */
#define MAX_COMP 10
/** @brief Tamanho máximo do nome de um componente */
#define MAX_NOME 30
/** @brief Tamanho máximo do tipo de um componente */
#define MAX_TIPO 20

/**
 * @struct Componente
 * @brief Estrutura que representa um item do inventário.
 *
 * Cada componente possui:
 * - Nome identificador
 * - Tipo (ex: Estrutural, Eletrônico, Energia)
 * - Quantidade em unidades
 * - Prioridade de montagem (1 a 5)
 */
typedef struct {
    char nome[MAX_NOME];   /**< Nome do componente */
    char tipo[MAX_TIPO];   /**< Tipo do componente */
    int quantidade;        /**< Quantidade disponível */
    int prioridade;        /**< Prioridade de montagem (1 a 5) */
} Componente;

/**
 * @brief Função principal do jogo Torre de Fuga.
 *
 * Controla o fluxo do jogo, exibindo o menu principal,
 * permitindo gerenciar os componentes e ativar a torre.
 */
void jogarTorre();

/**
 * @brief Adiciona um componente ao inventário.
 *
 * @param comp Vetor de componentes (inventário).
 * @param qtd Ponteiro para a quantidade atual de componentes.
 */
void adicionarComponente(Componente comp[], int *qtd);

/**
 * @brief Descarta (remove) um componente do inventário.
 *
 * @param comp Vetor de componentes (inventário).
 * @param qtd Ponteiro para a quantidade atual de componentes.
 */
void descartarComponente(Componente comp[], int *qtd);

/**
 * @brief Lista todos os componentes do inventário.
 *
 * @param comp Vetor de componentes.
 * @param qtd Quantidade atual de componentes.
 */
void listarComponentes(Componente comp[], int qtd);

/**
 * @brief Ordena os componentes por nome usando Bubble Sort.
 *
 * @param comp Vetor de componentes.
 * @param qtd Quantidade de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações realizadas.
 */
void bubbleSortNome(Componente comp[], int qtd, int *comparacoes);

/**
 * @brief Ordena os componentes por tipo usando Insertion Sort.
 *
 * @param comp Vetor de componentes.
 * @param qtd Quantidade de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações realizadas.
 */
void insertionSortTipo(Componente comp[], int qtd, int *comparacoes);

/**
 * @brief Ordena os componentes por prioridade usando Selection Sort.
 *
 * @param comp Vetor de componentes.
 * @param qtd Quantidade de componentes.
 * @param comparacoes Ponteiro para armazenar o número de comparações realizadas.
 */
void selectionSortPrioridade(Componente comp[], int qtd, int *comparacoes);

/**
 * @brief Busca binária de um componente pelo nome.
 *
 * Requer que o inventário esteja previamente ordenado por nome.
 *
 * @param comp Vetor de componentes.
 * @param qtd Quantidade de componentes.
 * @param chave Nome do componente a ser buscado.
 * @param comparacoes Ponteiro para armazenar o número de comparações realizadas.
 * @return Índice do componente encontrado, ou -1 se não encontrado.
 */
int buscaBinariaPorNome(Componente comp[], int qtd, char chave[], int *comparacoes);

#endif
