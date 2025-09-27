#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "detective.h"

/* ----------------- Utilitários ----------------- */

/* cria dinamicamente uma sala com nome e pista (pista pode ser NULL ou "") */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) { perror("malloc sala"); exit(EXIT_FAILURE); }
    strncpy(s->nome, nome, MAX_NOME_SALA-1); s->nome[MAX_NOME_SALA-1] = '\0';
    if (pista) strncpy(s->pista, pista, MAX_TEXTO_PISTA-1);
    else s->pista[0] = '\0';
    s->pista[MAX_TEXTO_PISTA-1] = '\0';
    s->esquerda = s->direita = NULL;
    return s;
}

/* libera o mapa (árvore de salas) em pós-ordem */
void liberarMapa(Sala *root) {
    if (!root) return;
    liberarMapa(root->esquerda);
    liberarMapa(root->direita);
    free(root);
}

/* ----------------- BST para pistas ----------------- */

PistaNode* criarNoPista(const char *pista) {
    PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
    if (!n) { perror("malloc pista"); exit(EXIT_FAILURE); }
    strncpy(n->pista, pista, MAX_TEXTO_PISTA-1); n->pista[MAX_TEXTO_PISTA-1] = '\0';
    n->esq = n->dir = NULL;
    return n;
}

/* insere sem duplicar: compara strings lexicograficamente */
void inserirPistaBST(PistaNode **raiz, const char *pista) {
    if (!(*raiz)) {
        *raiz = criarNoPista(pista);
        return;
    }
    int cmp = strcmp(pista, (*raiz)->pista);
    if (cmp < 0) inserirPistaBST(&((*raiz)->esq), pista);
    else if (cmp > 0) inserirPistaBST(&((*raiz)->dir), pista);
    else {
        // já existe; não inserir duplicata
    }
}

/* exibe as pistas em ordem alfabética (inorder) */
void exibirPistasInOrder(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistasInOrder(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistasInOrder(raiz->dir);
}

/* libera BST */
void liberarPistasBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistasBST(raiz->esq);
    liberarPistasBST(raiz->dir);
    free(raiz);
}

/* ----------------- Hash table (cadeia) ----------------- */

static unsigned long hash_string(const char *s) {
    // djb2
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*s++)) hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

void initHash(HashTable *h) {
    for (int i = 0; i < HASH_SIZE; ++i) h->vet[i] = NULL;
}

/* insere associação pista -> suspeito (não duplica a mesma pista) */
void inserirNaHash(HashTable *h, const char *pista, const char *suspeito) {
    unsigned long idx = hash_string(pista);
    HashEntry *e = h->vet[idx];
    while (e) {
        if (strcmp(e->pista, pista) == 0) {
            // já existe: atualiza suspeito (ou mantém)
            strncpy(e->suspeito, suspeito, sizeof(e->suspeito)-1);
            e->suspeito[sizeof(e->suspeito)-1] = '\0';
            return;
        }
        e = e->prox;
    }
    // criar novo entry e inserir no início
    HashEntry *novo = (HashEntry*) malloc(sizeof(HashEntry));
    if (!novo) { perror("malloc hashentry"); exit(EXIT_FAILURE); }
    strncpy(novo->pista, pista, MAX_TEXTO_PISTA-1); novo->pista[MAX_TEXTO_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, sizeof(novo->suspeito)-1); novo->suspeito[sizeof(novo->suspeito)-1] = '\0';
    novo->prox = h->vet[idx];
    h->vet[idx] = novo;
}

/* procura o suspeito associado a uma pista; retorna NULL se não encontrada */
const char* encontrarSuspeito(HashTable *h, const char *pista) {
    unsigned long idx = hash_string(pista);
    HashEntry *e = h->vet[idx];
    while (e) {
        if (strcmp(e->pista, pista) == 0) return e->suspeito;
        e = e->prox;
    }
    return NULL;
}

/* conta quantas pistas na BST apontam para o suspeito fornecido */
int contarPistasParaSuspeito(HashTable *h, PistaNode *raiz, const char *suspeito) {
    if (!raiz) return 0;
    int cnt = 0;
    const char *s = encontrarSuspeito(h, raiz->pista);
    if (s && strcmp(s, suspeito) == 0) cnt = 1;
    return cnt + contarPistasParaSuspeito(h, raiz->esq, suspeito) + contarPistasParaSuspeito(h, raiz->dir, suspeito);
}

/* libera a tabela hash */
void liberarHash(HashTable *h) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashEntry *e = h->vet[i];
        while (e) {
            HashEntry *next = e->prox;
            free(e);
            e = next;
        }
        h->vet[i] = NULL;
    }
}

/* ----------------- Exploração interativa (única função) -----------------
   - visita sala atual
   - exibe pista (se houver), insere na BST
   - pergunta direção: 'e' esquerda, 'd' direita, 's' sair
   - o jogo não impede revisitar salas (mas BST evita duplicar pistas)
--------------------------------------------------------------- */
void explorarSalas(Sala *inicio, PistaNode **raizPistas, HashTable *h) {
    Sala *atual = inicio;
    char buffer[16];

    while (atual != NULL) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("💡 Pista encontrada: \"%s\"\n", atual->pista);
            inserirPistaBST(raizPistas, atual->pista);
            const char *s = encontrarSuspeito(h, atual->pista);
            if (s) {
                printf("  (essa pista está associada ao suspeito: %s)\n", s);
            }
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("Escolha caminho: (e = esquerda, d = direita, s = sair): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        // trim newline
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0) { // se usuário apenas apertou Enter, pergunta de novo
            continue;
        }
        char escolha = tolower((unsigned char)buffer[0]);
        if (escolha == 'e') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há caminho à esquerda.\n");
        } else if (escolha == 'd') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há caminho à direita.\n");
        } else if (escolha == 's') {
            printf("Encerrando exploração.\n");
            return;
        } else {
            printf("Opção inválida. Use e, d ou s.\n");
        }
    }
}

/* ----------------- Função que monta o mapa, preenche hash e executa o jogo --------------- */

void jogarDetective() {
    // 1) montar mapa (fixo)
    Sala *hall = criarSala("Hall de Entrada", "Pegadas molhadas");
    Sala *salaEstar = criarSala("Sala de Estar", "Lenço de seda com monograma");
    Sala *cozinha = criarSala("Cozinha", "Marca de gordura no tampo");
    Sala *jardim = criarSala("Jardim", "Folha com tinta vermelha");
    Sala *biblioteca = criarSala("Biblioteca", "Página arrancada com anotações");
    Sala *quarto = criarSala("Quarto Secreto", "Chave dourada");
    Sala *escritorio = criarSala("Escritório", "Carta ameaçadora");
    Sala *porao = criarSala("Porão", "Pegadas de botas grandes");

    // ligações (árvore binária)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    cozinha->esquerda = escritorio;
    cozinha->direita = quarto;

    biblioteca->esquerda = porao; // apenas exemplo de estrutura

    // 2) preparar tabela hash com associação pista -> suspeito
    HashTable tabela;
    initHash(&tabela);

    // associar pistas a suspeitos (pré-definido)
    inserirNaHash(&tabela, "Pegadas molhadas", "Srta. Rosa");
    inserirNaHash(&tabela, "Lenço de seda com monograma", "Sr. Verde");
    inserirNaHash(&tabela, "Marca de gordura no tampo", "Cozinheiro");
    inserirNaHash(&tabela, "Folha com tinta vermelha", "Sr. Verde");
    inserirNaHash(&tabela, "Página arrancada com anotações", "Prof. Azul");
    inserirNaHash(&tabela, "Chave dourada", "Srta. Rosa");
    inserirNaHash(&tabela, "Carta ameaçadora", "Prof. Azul");
    inserirNaHash(&tabela, "Pegadas de botas grandes", "Sr. Preto");

    // 3) BST vazia para pistas coletadas
    PistaNode *raizPistas = NULL;

    printf("\n=== Detective Quest: investigação e acusação ===\n");
    printf("Você começa no Hall de Entrada.\n");

    // limpar buffer antes de usar fgets no explorarSalas
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    explorarSalas(hall, &raizPistas, &tabela);

    // 4) ao final, exibir pistas coletadas e pedir acusação
    printf("\n\n==== Pistas coletadas (ordenadas) ====\n");
    if (!raizPistas) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistasInOrder(raizPistas);
    }

    // solicitar acusação
    char acusacao[64];
    printf("\nQuem você acusa? Digite nome do suspeito (ex: Srta. Rosa): ");
    if (!fgets(acusacao, sizeof(acusacao), stdin)) acusacao[0] = '\0';
    acusacao[strcspn(acusacao, "\n")] = '\0';

    // se acusado vazio -> cancela
    if (strlen(acusacao) == 0) {
        printf("Nenhuma acusação feita.\n");
    } else {
        // contar pistas que apontam para o suspeito acusado
        int cont = contarPistasParaSuspeito(&tabela, raizPistas, acusacao);
        printf("\nPistas encontradas que apontam para %s: %d\n", acusacao, cont);
        if (cont >= 2) {
            printf("VEREDICTO: Acusação SUSTENTADA. O culpado provável é %s.\n", acusacao);
        } else {
            printf("VEREDICTO: Acusação FRACA. Não há pistas suficientes contra %s.\n", acusacao);
        }
    }

    // liberar tudo
    liberarPistasBST(raizPistas);
    liberarHash(&tabela);
    liberarMapa(hall); // libera árvore inteira (libera nós alocados)
}
