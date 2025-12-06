#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "../include/persistencia.h"
#include "../include/pedido.h"

// Incluindo todos os cabeçalhos necessários do projeto
#include "globals.h"      // Para as structs e MAX
#include "cliente.h"      // Contém inicializarLista() e os protótipos de cliente.c
#include "produto.h"
#include "pedido.h"
#include "interface.h"    // Contém as funções mostrarMenu...()
#include "persistencia.h"
#include "validadores.h"

#define MAX_REGISTROS 100
#define MAX_ITENS 500

// ====================================================================
// Variáveis Globais (Estruturas de Dados do Sistema)
// ====================================================================
// Clientes
ListaClientes clientesGeral; // Lista de base (Cliente)
PessoaFisica listaPF[MAX_REGISTROS];
int totalPF = 0;
PessoaJuridica listaPJ[MAX_REGISTROS];
int totalPJ = 0;

// Produtos
Produto listaProdutos[MAX_REGISTROS];
int totalProdutos = 0;

// Pedidos
Pedido listaPedidos[MAX_REGISTROS];
int totalPedidos = 0;
itemPedido listaItensPedidos[MAX_ITENS];
int totalItensPedidos = 0;

// ====================================================================
// FUNÇÕES DE SUPORTE (Para encapsular a lógica de persistência e menus)
// ====================================================================

void sincronizarListaGeral() {
    int i;
    int indexGeral = 0;

    // 1. Limpar e recontar o tamanho da lista geral
    clientesGeral.tamanho = 0;

    // 2. Copiar Pessoas Físicas para a lista geral
    for (i = 0; i < totalPF; i++) {
        // Assumindo que a struct PessoaFisica tem um campo 'dados' do tipo Cliente
        clientesGeral.elementos[indexGeral] = listaPF[i].dados;
        indexGeral++;
    }

    // 3. Copiar Pessoas Jurídicas para a lista geral
    for (i = 0; i < totalPJ; i++) {
        // Assumindo que a struct PessoaJuridica tem um campo 'dados' do tipo Cliente
        clientesGeral.elementos[indexGeral] = listaPJ[i].dados;
        indexGeral++;
    }

    clientesGeral.tamanho = indexGeral;
}

// Funções de Persistência (Assumidas no persistencia.c)
void carregarDadosIniciais() {
    clear();
    mvprintw(2, 2, "Carregando dados iniciais...");
    refresh();

    carregarPedidosCSV(listaPedidos, &totalPedidos, listaItensPedidos, &totalItensPedidos);
    carregarClientesCSV(
        clientesGeral.elementos, &clientesGeral.tamanho,
        listaPF, &totalPF,
        listaPJ, &totalPJ
    );

    sincronizarListaGeral();

    mvprintw(3, 2, "Dados carregados. Pressione qualquer tecla.");
    getch();
}

void salvarDadosFinais() {
    clear();
    mvprintw(2, 2, "Salvando dados finais...");
    refresh();

    persistirCliente(listaPF, totalPF, listaPJ, totalPJ);
    persistirProdutos(listaProdutos, totalProdutos);

    salvarPedidosCSV(listaPedidos, totalPedidos, listaItensPedidos, totalItensPedidos);

    mvprintw(3, 2, "Dados salvos. Finalizando...");
    getch();
}

// Funções de Interface
int inicializarInterface() {
    initscr();              // Inicia o modo curses
    if (stdscr == NULL) {
        return 0;
    }
    noecho();               // Não imprime caracteres digitados
    cbreak();               // Entrada imediata sem buffer (passa Control-C)
    keypad(stdscr, TRUE);   // Ativa teclas especiais (setas)
    curs_set(0);            // Oculta o cursor

    // Tenta ativar o uso de cores (opcional)
    if (has_colors()) {
        start_color();
    }

    return 1;
}

void finalizarInterface() {
    endwin();
}

// Menu Principal
void mostrarMenuPrincipal() {
    char *opcoes[] = {
        "1. Manter Clientes",
        "2. Manter Produtos",
        "3. Manter Pedidos",
        "4. Sair do Sistema"
    };
    int n_opcoes = 4;
    int escolha = 0;
    int ch;

    while (1) {
        clear();
        box(stdscr, 0, 0); // Desenha a borda
        mvprintw(1, 2, "===== SISTEMA DE CONTROLE DE PEDIDOS (SCP) - MENU PRINCIPAL =====");

        // Imprimir opções
        for (int i = 0; i < n_opcoes; i++) {
            if (i == escolha) {
                attron(A_REVERSE); // Destaca a opção selecionada
                mvprintw(3 + i, 4, "%s", opcoes[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 4, "%s", opcoes[i]);
            }
        }

        mvprintw(10, 2, "Use as setas para navegar e ENTER para selecionar.");

        ch = getch();

        switch (ch) {
            case KEY_UP:
                escolha = (escolha > 0) ? escolha - 1 : n_opcoes - 1;
                break;
            case KEY_DOWN:
                escolha = (escolha < n_opcoes - 1) ? escolha + 1 : 0;
                break;
            case ENTER: // ENTER
                clear();
                switch (escolha) {
                    case 0: // Clientes
                        // CORREÇÃO 1: Passando a lista geral e o ponteiro para seu tamanho
                        mostrarMenuClientes(clientesGeral.elementos, &clientesGeral.tamanho, listaPF, &totalPF, listaPJ, &totalPJ);
                        break;
                    case 1: // Produtos
                        // CORREÇÃO 2 & 3: Corrigido typo e passado ponteiro para totalItensPedidos
                        mostrarMenuProdutos(listaProdutos, &totalProdutos, listaItensPedidos, &totalItensPedidos);
                        break;
                    case 2: // Pedidos
                        mostrarMenuPedidos(listaPedidos, &totalPedidos, listaItensPedidos, &totalItensPedidos);
                        break;
                    case 3: // Sair
                        return; // Sai da função do menu e finaliza o programa
                }
                break;
        }
    }
}

// ====================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ====================================================================

int main() {
    // Inicialização da interface ncurses
    if (!inicializarInterface()) {
        fprintf(stderr, "ERRO: Falha ao inicializar a interface ncurses.\n");
        return 1;
    }

    // CORREÇÃO 4: Inicializa a lista geral de clientes (define tamanho=0)
    inicializarLista(&clientesGeral);

    // Carregamento de dados iniciais dos arquivos CSV
    carregarDadosIniciais();

    // Execução do Menu Principal (loop principal do programa)
    mostrarMenuPrincipal();

    // Persistência de dados finais nos arquivos CSV
    salvarDadosFinais();

    // Finalização da interface ncurses
    finalizarInterface();

    return 0;
}