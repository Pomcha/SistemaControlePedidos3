#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#include <ncurses.h>
#include "globals.h" 
#include "validadores.h"
#include "produto.h"

// ===========================================
// FUNÇÕES AUXILIARES
// ===========================================

// Função Auxiliar para limpar o buffer de entrada após scanf (Essencial para I/O)
static void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Analisar Produto
int analisarProduto(Produto listaProdutos[], int totalProdutos, int idBuscado) {
    for (int i = 0; i < totalProdutos; i++) {
        if (listaProdutos[i].id == idBuscado) {
            return 1; // Encontrado
        }
    }
    return 0; // Não encontrado
}

// ===========================================
// CADASTRO
// ===========================================

// Cadastrar Produto
void cadastrarProduto(Produto listaProdutos[], int *totalProdutos) {
    Produto novo;
    int id, y = 3;
    char temp_preco[20], temp_estoque[20]; // Variáveis temporárias para I/O

    clear();
    mvprintw(1, 4, "==== CADASTRO DE PRODUTO ====");

    if (*totalProdutos >= MAX) {
        mvprintw(y, 4, "ERRO: Limite de produtos atingido.");
        getch();
        return;
    }

    echo(); // Habilita eco para ver o que digita
    curs_set(1); // Exibe o cursor

    // Leitura do ID
    mvprintw(y, 4, "ID: "); 
    scanw("%d", &id);
    
    if (analisarProduto(listaProdutos, *totalProdutos, id)) {
        noecho();
        mvprintw(y + 1, 4, "ERRO: ID do produto ja existe!");
        getch(); return;
    }
    novo.id = id;
    
    // Leitura da Descrição
    mvprintw(++y, 4, "Descrição: ");
    getstr(novo.descricao); // Lê string com ncurses
    
    // Leitura do Preço
    mvprintw(++y, 4, "Preço: "); 
    getstr(temp_preco);
    novo.preco = atof(temp_preco); // Converte string para float
    
    // Leitura do Estoque
    mvprintw(++y, 4, "Estoque: ");
    getstr(temp_estoque);
    novo.estoque = atoi(temp_estoque); // Converte string para int

    noecho(); // Desabilita eco
    curs_set(0); // Oculta cursor

    // Salvar e finalizar
    listaProdutos[*totalProdutos] = novo;
    (*totalProdutos)++;

    mvprintw(++y, 4, "Produto cadastrado com sucesso!");
    mvprintw(++y, 4, "Pressione qualquer tecla para voltar.");
    getch();
}

// ===========================================
// CONSULTA, REMOÇÃO E LISTAGEM
// ===========================================

// Consultar Produto
void consultarProduto(Produto lista[], int tamanho) {
    int id;
    system("clear");
    printf("==== CONSULTAR PRODUTO ====\n\n");

    printf("Digite o ID do produto a consultar: ");
    fflush(stdout);
    if (scanf("%d", &id) != 1) {
        printf("\nERRO: ID inválido.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    for (int i = 0; i < tamanho; i++) {
        if (lista[i].id == id) {
            printf("\n--- Produto Encontrado ---\n");
            printf("ID: %d\n", lista[i].id);
            printf("Descrição: %s\n", lista[i].descricao);
            printf("Preço: %.2f\n", lista[i].preco);
            printf("Estoque: %d\n", lista[i].estoque);
            printf("Pressione ENTER para voltar...");
            fflush(stdout);
            limparBuffer();
            return;
        }
    }
    printf("\nERRO: Produto com ID %d não encontrado.\n", id);
    printf("Pressione ENTER para voltar...");
    fflush(stdout);
    limparBuffer();
}

// Verificar se produto está em pedido (Manter)
int produtoEmPedido(int produtoId, itemPedido itens[], int qtdItens) {
    // ... (Mantido o mesmo)
    for (int i = 0; i < qtdItens; i++) {
        if (itens[i].produtoId == produtoId) {
            return 1;
        }
    }
    return 0;
}

// Remover Produto
void removerProduto(Produto lista[], int *tamanho, itemPedido itens[], int qtdItens) {
    int id;
    system("clear");
    printf("==== REMOVER PRODUTO ====\n\n");

    printf("Digite o ID do produto a remover: ");
    fflush(stdout);
    if (scanf("%d", &id) != 1) {
        printf("\nERRO: ID inválido.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    int pos = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (lista[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nERRO: Produto não encontrado.\n");
        printf("Pressione ENTER para voltar..."); fflush(stdout); limparBuffer();
        return;
    }

    if (produtoEmPedido(id, itens, qtdItens)) {
        printf("\nERRO: Produto não pode ser removido pois existe em pedidos.\n");
        printf("Pressione ENTER para voltar..."); fflush(stdout); limparBuffer();
        return;
    }

    char confirm;
    printf("Confirmar remoção do produto %d? (S/N): ", id);
    fflush(stdout);
    // Usar " %c" garante que espaços e newlines no buffer sejam ignorados.
    if (scanf(" %c", &confirm) != 1) {
         printf("\nERRO: Entrada inválida.\n");
         limparBuffer();
         return;
    }
    limparBuffer();

    if (confirm == 'S' || confirm == 's') {
        for (int i = pos; i < *tamanho - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*tamanho)--;
        printf("\nProduto removido com sucesso.\n");
    } else {
        printf("\nOperação cancelada.\n");
    }
    printf("Pressione ENTER para voltar..."); fflush(stdout); limparBuffer();
}

// Listar Produtos
void listarProdutos(Produto lista[], int tamanho) {
    system("clear");
    printf("==== LISTAGEM GERAL DE PRODUTOS ====\n");
    
    if (tamanho == 0) {
        printf("\nNenhum produto cadastrado.\n");
        printf("Pressione ENTER para voltar..."); fflush(stdout); limparBuffer();
        return;
    }

    printf("\n---- LISTA DE PRODUTOS ----\n");
    // Header
    printf("%-5s %-40s %-10s %s\n", "ID", "Descrição", "Preço", "Estoque");
    printf("----- ---------------------------------------- ---------- -------\n");
    
    // Dados
    for (int i = 0; i < tamanho; i++) {
        printf("%-5d %-40s %-10.2f %d\n", 
            lista[i].id, 
            lista[i].descricao, 
            lista[i].preco, 
            lista[i].estoque
        );
    }
    printf("\n---------------------------\n");
    printf("Pressione ENTER para voltar...");
    fflush(stdout);
    limparBuffer();
}