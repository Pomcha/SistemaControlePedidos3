#include <ncurses.h>
#include <string.h> 
#include "globals.h"
#include "cliente.h"
#include "produto.h" 
#include "pedido.h"
#define ENTER 10

// ===============================================
// PROTÓTIPOS (Conforme cliente.h, produto.h, pedido.h)
// ===============================================

// Protótipos de Cliente (NOVOS)
int cadastrarClientePF(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int totalPJ);
int cadastrarClientePJ(PessoaJuridica listaPJ[], int *totalPJ, PessoaFisica listaPF[], int totalPF);
void telaConsultarCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);
void telaRemoverCliente(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ);
void listarTodosClientes(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

// Protótipos de Produto
void cadastrarProduto(Produto listaProdutos[], int *totalProdutos); 
void consultarProduto(Produto listaProdutos[], int totalProdutos);
void listarProdutos(Produto listaProdutos[], int totalProdutos);
void removerProduto(Produto listaProdutos[], int *totalProdutos, itemPedido listaItensPedidos[], int totalItensPedidos); 

// Protótipos de Pedido
void inserirPedido(Pedido pedidos[], int *qtdPedidos,itemPedido itens[], int *qtdItens);
void listarPedidos(Pedido pedidos[], int qtdPedidos);
void detalharPedido(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens);
void salvarPedidosCSV(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens);
void carregarPedidosCSV(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens);


// ===============================================
// MENU DE CLIENTES
// ===============================================

void mostrarMenuClientes(Cliente listaClientes[], int *totalClientes, PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ){
    char *opcoes[]={
        "Cadastrar Pessoa Fisica",
        "Cadastrar Pessoa Juridica", 
        "Consultar Cliente",
        "Remover Cliente",
        "Listar Todos",
        "Voltar ao Menu Principal"
    };
    int n_opcoes = 6;
    int escolha = 0;
    int tecla;

    while(1){
        clear();
        mvprintw(1,2,"===== MENU DE CLIENTES (PF/PJ) ====="); 
        
        // Desenho do Menu (Ncurses)
        for(int i=0; i < n_opcoes; i++){
            if(i==escolha){
                attron(A_REVERSE);
                mvprintw(3 + i, 4, "%s", opcoes[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 4, "%s", opcoes[i]);
            }
        }
        
        tecla = getch();
        switch(tecla){
            case KEY_UP:
                escolha--;
                if(escolha < 0) escolha = n_opcoes -1;
                break;
            case KEY_DOWN:
                escolha++;
                if(escolha >= n_opcoes) escolha =0; 
                break;
            case ENTER:
                switch(escolha){
                    case 0: // Cadastrar PF
                        // totalPJ é passado por valor (*totalPJ) pois não será alterado
                        cadastrarClientePF(listaPF, totalPF, listaPJ, *totalPJ);
                        break;
                    case 1: // Cadastrar PJ
                        // totalPF é passado por valor (*totalPF) pois não será alterado
                        cadastrarClientePJ(listaPJ, totalPJ, listaPF, *totalPF);
                        break;
                    case 2: // Consultar
                        telaConsultarCliente(listaPF, *totalPF, listaPJ, *totalPJ);
                        break;
                    case 3: // Remover
                        // totalPF e totalPJ são passados por referência (ponteiro) pois podem ser alterados
                        telaRemoverCliente(listaPF, totalPF, listaPJ, totalPJ);
                        break;
                    case 4: // Listar
                        listarTodosClientes(listaPF, *totalPF, listaPJ, *totalPJ);
                        break;
                    case 5: // Voltar
                        return;
                }
                break;
        }
    }
}


// ===============================================
// MENU DE PRODUTOS
// ===============================================

void mostrarMenuProdutos(Produto listaProdutos[], int *totalProdutos, itemPedido listaItensPedidos[], int *totalItensPedidos) { 
    int escolha;
    int highlight = 0;

    char *opcoes[] = {
        "Cadastrar Produto",
        "Consultar Produto",
        "Listar Produtos",
        "Remover Produto",
        "Voltar",
    };
    int n_opcoes = 5;

    while (1) {
        clear();
        box(stdscr, 0, 0);
        mvprintw(1, 2, "===== MENU DE PRODUTOS =====");

        for (int i = 0; i < n_opcoes; i++) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(3 + i, 4, "%s", opcoes[i]);
            if (i == highlight) attroff(A_REVERSE);
        }
        escolha = getch();

        switch (escolha) {
            case KEY_UP:
            highlight--;
            if (highlight < 0) highlight = n_opcoes -1;
            break;

            case KEY_DOWN:
            highlight++;
            if (highlight >= n_opcoes) highlight = 0;
            break;

            case ENTER:
                clear();
                
                if (highlight == 0) cadastrarProduto(listaProdutos, totalProdutos);
                else if (highlight == 1) consultarProduto(listaProdutos, *totalProdutos);
                else if (highlight == 2) listarProdutos(listaProdutos, *totalProdutos);
                // Remover Produto precisa checar itens do pedido, por isso recebe a lista e o total de itens
                else if (highlight == 3) removerProduto(listaProdutos, totalProdutos, listaItensPedidos, *totalItensPedidos);
                else if (highlight == 4) return; 

                mvprintw(10,4,"Pressione qualquer tecla para continuar...");
                getch();
                break;
        }
    }
}


// ===============================================
// MENU DE PEDIDOS
// ===============================================

void mostrarMenuPedidos(Pedido pedidos[], int *qtdPedidos,itemPedido itens[], int *qtdItens)
{
    const char *opcoes[] = {
        "Inserir Pedido",
        "Listar Pedidos",
        "Detalhar Pedido",
        "Salvar Pedidos em CSV",
        "Carregar Pedidos do CSV",
        "Voltar"
    };

    int qtdOpcoes = 6;
    int selecionado = 0;
    int ch;

    while (1){
        clear();
        mvprintw(1, 2, "===== MENU DE PEDIDOS =====");

        //Opcoes
        for (int i = 0; i < qtdOpcoes; i++) {
            if (i == selecionado) {
                attron(A_REVERSE); 
                mvprintw(3 + i, 4, "%s", opcoes[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(3 + i, 4, "%s", opcoes[i]);
            }
        }

        ch = getch();

        if (ch == KEY_UP) {
            selecionado--;
            if (selecionado < 0)
                selecionado = qtdOpcoes - 1;
        }
        else if (ch == KEY_DOWN) {
            selecionado++;
            if (selecionado >= qtdOpcoes)
                selecionado = 0;
        }
        else if (ch == ENTER) { 
            clear();
            switch (selecionado) {

                case 0:
                    inserirPedido(pedidos, qtdPedidos, itens, qtdItens);
                    break;

                case 1:
                    listarPedidos(pedidos, *qtdPedidos);
                    break;

                case 2:
                    detalharPedido(pedidos, *qtdPedidos, itens, *qtdItens);
                    break;

                case 3:
                    salvarPedidosCSV(pedidos, *qtdPedidos, itens, *qtdItens);
                    break;

                case 4:
                    carregarPedidosCSV(pedidos, qtdPedidos, itens, qtdItens);
                    break;

                case 5:
                    return; 
            }

            mvprintw(20, 4, "Pressione qualquer tecla para voltar ao menu");
            getch();
        }
    }
}