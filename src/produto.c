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

int analisarProduto(Produto listaProdutos[], int totalProdutos, int idBuscado) {
    for (int i = 0; i < totalProdutos; i++) {
        if (listaProdutos[i].id == idBuscado) {
            return 1; // Encontrado
        }
    }
    return 0; // Não encontrado
}

int produtoEmPedido(int produtoId, itemPedido itens[], int qtdItens) {
    for (int i = 0; i < qtdItens; i++) {
        if (itens[i].produtoId == produtoId) {
            return 1;
        }
    }
    return 0;
}


// ===========================================
// CADASTRO (Mantida)
// ===========================================

void cadastrarProduto(Produto listaProdutos[], int *totalProdutos) {
    Produto novo;
    int id, y = 3;
    char temp_preco[20], temp_estoque[20]; 

    clear();
    mvprintw(1, 4, "==== CADASTRO DE PRODUTO ====");

    if (*totalProdutos >= MAX) { // MAX é a constante que define o limite.
        mvprintw(y, 4, "ERRO: Limite de produtos atingido.");
        getch();
        return;
    }

    echo(); 
    curs_set(1); 

    // Leitura do ID
    mvprintw(y, 4, "ID: "); 
    refresh();
    scanw("%d", &id);
    
    if (analisarProduto(listaProdutos, *totalProdutos, id)) {
        noecho();
        curs_set(0);
        mvprintw(y + 1, 4, "ERRO: ID do produto ja existe!");
        mvprintw(y + 2, 4, "Pressione qualquer tecla para voltar.");
        getch(); 
        return;
    }
    novo.id = id;
    
    // Leitura da Descrição
    mvprintw(++y, 4, "Descrição: ");
    refresh();
    getstr(novo.descricao); 
    
    // Leitura do Preço
    mvprintw(++y, 4, "Preço: "); 
    refresh();
    getstr(temp_preco);
    novo.preco = atof(temp_preco); 
    
    // Leitura do Estoque
    mvprintw(++y, 4, "Estoque: ");
    refresh();
    getstr(temp_estoque);
    novo.estoque = atoi(temp_estoque);

    noecho(); 
    curs_set(0); 

    // Salvar e finalizar
    listaProdutos[*totalProdutos] = novo;
    (*totalProdutos)++;

    mvprintw(++y, 4, "Produto cadastrado com sucesso!");
    mvprintw(++y, 4, "Pressione qualquer tecla para voltar.");
    refresh();
    getch();
}

// ===========================================
// CONSULTA, REMOÇÃO E LISTAGEM 
// ===========================================

// Consultar Produto 
void consultarProduto(Produto lista[], int tamanho) {
    int id;
    int y = 3;
    
    clear();
    mvprintw(1, 4, "==== CONSULTAR PRODUTO ====");
    
    echo(); 
    curs_set(1);
    
    mvprintw(y, 4, "Digite o ID do produto a consultar: ");
    refresh();
    scanw("%d", &id); 
    
    noecho();
    curs_set(0);

    for (int i = 0; i < tamanho; i++) {
        if (lista[i].id == id) {
            mvprintw(++y, 4, "--- Produto Encontrado ---");
            mvprintw(++y, 4, "ID: %d", lista[i].id);
            mvprintw(++y, 4, "Descrição: %s", lista[i].descricao);
            mvprintw(++y, 4, "Preço: R$ %.2f", lista[i].preco);
            mvprintw(++y, 4, "Estoque: %d", lista[i].estoque);
            mvprintw(++y + 1, 4, "Pressione qualquer tecla para voltar.");
            refresh();
            getch(); 
            return;
        }
    }
    
    mvprintw(++y, 4, "ERRO: Produto com ID %d nao encontrado.", id);
    mvprintw(++y, 4, "Pressione qualquer tecla para voltar.");
    refresh();
    getch();
}

// Remover Produto (Corrigida)
void removerProduto(Produto lista[], int *tamanho, itemPedido itens[], int qtdItens) {
    int id;
    int y = 3;
    char confirm_str[5] = ""; 

    clear();
    mvprintw(1, 4, "==== REMOVER PRODUTO ====");

    echo();
    curs_set(1);
    
    mvprintw(y, 4, "Digite o ID do produto a remover: ");
    refresh();
    scanw("%d", &id);

    noecho();
    curs_set(0);

    int pos = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (lista[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        mvprintw(++y, 4, "ERRO: Produto nao encontrado.");
        mvprintw(++y, 4, "Pressione qualquer tecla para voltar."); refresh(); getch();
        return;
    }

    if (produtoEmPedido(id, itens, qtdItens)) {
        mvprintw(++y, 4, "ERRO: Produto nao pode ser removido pois existe em pedidos.");
        mvprintw(++y, 4, "Pressione qualquer tecla para voltar."); refresh(); getch();
        return;
    }

    // Confirmação
    echo();
    curs_set(1);
    mvprintw(++y, 4, "Confirmar remocao do produto %d? (S/N): ", id);
    refresh();
    getstr(confirm_str);
    noecho();
    curs_set(0);
    
    if (confirm_str[0] == 'S' || confirm_str[0] == 's') {
        for (int i = pos; i < *tamanho - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*tamanho)--;
        mvprintw(++y, 4, "Produto removido com sucesso.");
    } else {
        mvprintw(++y, 4, "Operacao cancelada.");
    }
    mvprintw(++y, 4, "Pressione qualquer tecla para voltar."); refresh(); getch();
}

// Listar Produtos (Corrigida)
void listarProdutos(Produto lista[], int tamanho) {
    clear();
    mvprintw(1, 4, "==== LISTAGEM GERAL DE PRODUTOS ====");
    
    if (tamanho == 0) {
        mvprintw(3, 4, "Nenhum produto cadastrado.");
        mvprintw(5, 4, "Pressione qualquer tecla para voltar.");
        refresh();
        getch();
        return;
    }
    
    int y = 3;
    // Header for listing (Ajustado o formato para melhor visualização no terminal)
    mvprintw(y++, 4, "%-5s | %-40s | %-10s | %s", "ID", "Descricao", "Preco", "Estoque");
    mvprintw(y++, 4, "----- | ---------------------------------------- | ---------- | -------");

    for (int i = 0; i < tamanho; i++) {
        // Dados
        mvprintw(y++, 4, "%-5d | %-40s | %-10.2f | %d", 
            lista[i].id, 
            lista[i].descricao, 
            lista[i].preco, 
            lista[i].estoque
        );
    }
    
    mvprintw(y + 2, 4, "Pressione qualquer tecla para voltar.");
    refresh();
    getch();
}