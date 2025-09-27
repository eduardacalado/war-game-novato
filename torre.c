#include "torre.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/*
 * mostrarInventario()
 * Exibe todos os componentes atualmente armazenados na mochila (inventário).
 * - Mostra em formato de tabela com Nome, Tipo, Quantidade e Prioridade.
 * - Se o inventário estiver vazio, mostra aviso ao jogador.
 */
void mostrarInventario(Componente comp[], int qtd) {
    if (qtd == 0) {
        printf("\n📦 Inventário vazio.\n");
        printf("Pressione Enter para continuar...");
        getchar(); getchar();
        return;
    }

    printf("\n--- INVENTÁRIO ATUAL (%d/%d) ---\n", qtd, MAX_COMP);
    printf("--------------------------------------------------------------------------------\n");
    printf("%-20s | %-20s | %-12s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        printf("%-20s | %-20s | %-12d | %-10d\n",
               comp[i].nome,
               comp[i].tipo,
               comp[i].quantidade,
               comp[i].prioridade);
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("Pressione Enter para continuar...");
    getchar(); getchar();
}

/*
 * adicionarComponente()
 * Permite ao jogador coletar um novo componente.
 * - Coleta Nome, Tipo, Quantidade e Prioridade.
 * - Faz validações para garantir dados corretos (ex.: qtd > 0, prioridade entre 1 e 5).
 * - Adiciona componente à mochila e exibe o inventário atualizado.
 */
void adicionarComponente(Componente comp[], int *qtd) {
    if (*qtd >= MAX_COMP) {
        printf("Inventário cheio!\n");
        return;
    }

    printf("\n--- Coletando novo componente ---\n");
    getchar(); // limpa buffer antes da leitura com fgets

    printf("Nome: ");
    fgets(comp[*qtd].nome, MAX_NOME, stdin);
    comp[*qtd].nome[strcspn(comp[*qtd].nome, "\n")] = 0;

    printf("Tipo (Estrutural, Eletronico, Energia): ");
    fgets(comp[*qtd].tipo, MAX_TIPO, stdin);
    comp[*qtd].tipo[strcspn(comp[*qtd].tipo, "\n")] = 0;

    int val;
    char buffer[50];

    // Entrada validada de quantidade
    do {
        printf("Quantidade: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &val) != 1 || val <= 0) {
            printf("Valor inválido! Digite um número inteiro maior que 0.\n");
            val = -1;
        }
    } while (val <= 0);
    comp[*qtd].quantidade = val;

    // Entrada validada de prioridade
    do {
        printf("Prioridade (1-5): ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &val) != 1 || val < 1 || val > 5) {
            printf("Valor inválido! Digite um número entre 1 e 5.\n");
            val = 0;
        }
    } while (val < 1 || val > 5);
    comp[*qtd].prioridade = val;

    (*qtd)++;
    mostrarInventario(comp, *qtd);
}

/*
 * descartarComponente()
 * Remove um item da mochila com base no índice escolhido pelo jogador.
 * - Mostra inventário antes da remoção.
 * - Reorganiza os elementos (shift à esquerda) para evitar "buracos".
 */
void descartarComponente(Componente comp[], int *qtd) {
    if (*qtd == 0) {
        printf("Inventário vazio!\n");
        return;
    }

    mostrarInventario(comp, *qtd);
    int idx;
    printf("Digite o índice do componente para descartar (1 a %d): ", *qtd);
    scanf("%d", &idx);

    if (idx < 1 || idx > *qtd) {
        printf("Índice inválido!\n");
        return;
    }

    for (int i = idx - 1; i < *qtd - 1; i++) {
        comp[i] = comp[i + 1];
    }
    (*qtd)--;
    printf("Componente descartado!\n");
}

/*
 * bubbleSortNome()
 * Ordena os componentes pelo campo "nome" usando o algoritmo Bubble Sort.
 * - Conta o número de comparações realizadas.
 */
void bubbleSortNome(Componente comp[], int qtd, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(comp[j].nome, comp[j + 1].nome) > 0) {
                Componente temp = comp[j];
                comp[j] = comp[j + 1];
                comp[j + 1] = temp;
            }
        }
    }
}

/*
 * insertionSortTipo()
 * Ordena os componentes pelo campo "tipo" usando o algoritmo Insertion Sort.
 */
void insertionSortTipo(Componente comp[], int qtd, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < qtd; i++) {
        Componente chave = comp[i];
        int j = i - 1;
        while (j >= 0 && strcmp(comp[j].tipo, chave.tipo) > 0) {
            (*comparacoes)++;
            comp[j + 1] = comp[j];
            j--;
        }
        comp[j + 1] = chave;
    }
}

/*
 * selectionSortPrioridade()
 * Ordena os componentes pela prioridade de montagem (decrescente).
 */
void selectionSortPrioridade(Componente comp[], int qtd, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < qtd; j++) {
            (*comparacoes)++;
            if (comp[j].prioridade > comp[maxIdx].prioridade) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            Componente temp = comp[i];
            comp[i] = comp[maxIdx];
            comp[maxIdx] = temp;
        }
    }
}

/*
 * buscaBinariaPorNome()
 * Busca um componente pelo nome usando algoritmo de busca binária.
 * - Requer que o inventário esteja ordenado por nome.
 * - Retorna o índice do componente ou -1 se não encontrado.
 */
int buscaBinariaPorNome(Componente comp[], int qtd, char chave[], int *comparacoes) {
    int inicio = 0, fim = qtd - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(comp[meio].nome, chave);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/*
 * jogarTorre()
 * Função principal da Torre de Fuga (nível mestre).
 * - Gerencia a mochila (inventário).
 * - Permite adicionar, descartar, listar e ordenar componentes.
 * - Inclui busca binária por nome.
 * - Menu interativo até o jogador ativar a torre (sair).
 */
void jogarTorre() {
    Componente mochila[MAX_COMP];
    int qtd = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag necessária para permitir busca binária

    do {
        printf("\n=====================================================\n");
        printf("    PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)");
        printf("\n=====================================================\n");
        printf("Itens da mochila: %d/%d\n", qtd, MAX_COMP);
        printf("Status de ordenação por Nome: %s\n",
               ordenadoPorNome ? "ORDENADO" : "NÃO ORDENADO");
        printf("\n1. Adicionar componente\n");
        printf("2. Descartar componente\n");
        printf("3. Listar componentes\n");
        printf("4. Organizar mochila\n");
        printf("5. Busca binária por componente-chave (nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (sair)\n");
        printf("-------------------------------------------------------\n");

        char buffer[10];
        printf("Escolha uma opção: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%d", &opcao) != 1) {
            printf("Opção inválida! Digite um número.\n");
            continue;
        }

        int comparacoes;
        char chave[MAX_NOME];
        clock_t inicio, fim;

        switch (opcao) {
            case 1:
                adicionarComponente(mochila, &qtd);
                ordenadoPorNome = 0;
                break;

            case 2:
                descartarComponente(mochila, &qtd);
                ordenadoPorNome = 0;
                mostrarInventario(mochila, qtd);
                break;

            case 3:
                mostrarInventario(mochila, qtd);
                break;

            case 4:
                printf("\n--- Estrategia de organização ---\n");
                printf("Como deseja ordenar os componentes?\n");
                printf("1. Por Nome\n");
                printf("2. Por Tipo\n");
                printf("3. Por Prioridade de Montagem\n");
                printf("0. Cancelar\n");
                printf("Escolha o critério: ");

                int escolha;
                scanf("%d", &escolha);

                if (escolha == 0) {
                    printf("Operação cancelada. Nenhuma ordenação realizada.\n");
                    printf("Pressione Enter para continuar...");
                    getchar(); getchar();
                    break;
                }

                inicio = clock();
                if (escolha == 1) {
                    bubbleSortNome(mochila, qtd, &comparacoes);
                    ordenadoPorNome = 1;
                }
                else if (escolha == 2) {
                    insertionSortTipo(mochila, qtd, &comparacoes);
                    ordenadoPorNome = 0;
                }
                else if (escolha == 3) {
                    selectionSortPrioridade(mochila, qtd, &comparacoes);
                    ordenadoPorNome = 0;
                }
                fim = clock();

                mostrarInventario(mochila, qtd);
                break;

            case 5:
                if (!ordenadoPorNome) {
                    printf("\nALERTA: A busca binária requer que a mochila esteja ordenada por NOME.");
                    printf("\nUse a opção 4 para organizar a mochila primeiro.\n");
                    printf("Pressione Enter para continuar...");
                    getchar(); getchar();
                    break;
                }

                getchar(); // limpa buffer antes de fgets
                printf("\n--- Busca binaria por componente-chave ---");
                printf("\nNome do componente a buscar: ");
                fgets(chave, MAX_NOME, stdin);
                chave[strcspn(chave, "\n")] = 0;

                inicio = clock();
                int idx = buscaBinariaPorNome(mochila, qtd, chave, &comparacoes);
                fim = clock();

                if (idx != -1)
                    printf("\n--- Componente-chave encontrado! ---"),
                    printf("\nNome: %s, Tipo: %s, Qtd: %d, Prio: %d\n",
                           mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade, mochila[idx].prioridade);
                else
                    printf("Componente NÃO encontrado!\n");

                printf("Pressione Enter para continuar...");
                getchar(); getchar();
                break;

            case 0:
                printf(">>> Torre ativada! Fuga iniciada! <<<\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}
