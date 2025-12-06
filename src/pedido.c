#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <ncurses.h>
#include "globals.h" 
#include "pedido.h"


// ====================================================================
// FUNÇÕES AUXILIARES DE I/O PARA NCURSES
// ====================================================================

/**
 * @brief 
 */
void obterStringNcurses(int y, int x, char *str, int max_len) {
   
    for (int i = 0; i < max_len; i++) mvaddch(y, x + i, ' ');
    
    move(y, x); 
    echo();            
    curs_set(1);     
    mvgetnstr(y, x, str, max_len);
    noecho();           
    curs_set(0);       
    refresh();
}

/**
 * @brief O
 */
int obterInteiroNcurses(int y, int x) {
    char input[12];
    obterStringNcurses(y, x, input, 11);
    return atoi(input);
}

/**
 * @brief Obtém um double (float) do usuário na interface ncurses.
 */
double obterDoubleNcurses(int y, int x) {
    char input[30]; 
    obterStringNcurses(y, x, input, 29);
    return atof(input);
}

int produtoExisteCSV(int idBuscado)
{
    FILE *fp = fopen("Produtos.csv", "r");
    if (!fp) {
        mvprintw(15, 2, "ERRO: Nao foi possivel abrir Produtos.csv. Pressione ENTER.");
        getch();
        return 0;
    }

    int id;
    char linha[500];

    fgets(linha, sizeof(linha), fp); // pular cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
    
        sscanf(linha, "%d;", &id); 
        if (id == idBuscado) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


int clienteExisteCSV(int idBuscado)
{
    FILE *fp = fopen("Clientes.csv", "r");
    if (!fp) {
        mvprintw(15, 2, "ERRO: Nao foi possivel abrir Clientes.csv. Pressione ENTER.");
        getch();
        return 0;
    }

    int id;
    char linha[500];
    char tipo[5]; 

    fgets(linha, sizeof(linha), fp); // pular cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        if (sscanf(linha, "%[^;];%d;", tipo, &id) == 2) { 
            if (id == idBuscado) {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}


int pedidoExisteCSV(int idBuscado)
{
    FILE *fp = fopen("Pedidos.csv", "r");
    if (!fp) {
        mvprintw(15, 2, "ERRO: Nao foi possivel abrir Pedidos.csv. Pressione ENTER.");
        getch();
        return 0;
    }

    int id;
    char linha[300];

    fgets(linha, sizeof(linha), fp); // pular cabecalho

    while (fgets(linha, sizeof(linha), fp)) {
        sscanf(linha, "%d,", &id);  
        if (id == idBuscado) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}



// ====================================================================
// FUNÇÕES PRINCIPAIS DO MENU (I/O CORRIGIDO PARA NCURSES)
// ====================================================================

void inserirPedido(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens)
{
    clear();
    mvprintw(1, 2, "===== INSERIR NOVO PEDIDO =====");

    Pedido p;
    int linha = 3;

    mvprintw(linha, 2, "ID do pedido: ");
    p.id = obterInteiroNcurses(linha, 18);

 
    while (pedidoExisteCSV(p.id)) {
        mvprintw(linha + 1, 2, "ERRO: O pedido com ID %d JA existe! Digite outro ID.", p.id);
        mvprintw(linha, 18, "                    "); 
        p.id = obterInteiroNcurses(linha, 18);
        mvprintw(linha + 1, 2, "                                                                "); 
    }
    linha += 2;

    mvprintw(linha, 2, "Data (DD/MM/AAAA): ");
    obterStringNcurses(linha, 21, p.data, 11);
    linha++;

    int clienteIdTemp;
    mvprintw(linha, 2, "ID do cliente (0 para CANCELAR): ");
    clienteIdTemp = obterInteiroNcurses(linha, 35); // Posição de input inicial
    linha++;

    while (!clienteExisteCSV(clienteIdTemp)) {
        
        if (clienteIdTemp == 0) {
            clear();
            mvprintw(2, 2, "Cadastro de pedido cancelado pelo usuario.");
            mvprintw(4, 2, "Pressione qualquer tecla para retornar ao menu...");
            refresh();
            getch();
            return; // SAI DA FUNCAO
        }

      
        mvprintw(linha + 1, 2, "ERRO: O cliente com ID %d NAO existe! Digite outro ID (0 para CANCELAR).", clienteIdTemp);
       
        mvprintw(linha, 35, "                    "); 
       
        clienteIdTemp = obterInteiroNcurses(linha, 35); 
        mvprintw(linha + 1, 2, "                                                                                "); // Limpa a mensagem de erro
    }
    p.clienteid = clienteIdTemp;
    linha += 2;

    p.total = 0;

    // 6. Cadastra itens do pedido
    int continuar = 1;
    int produtoIdTemp;

    mvprintw(linha, 2, "--- Cadastro de Itens ---");
    linha++;
    
    while (continuar) {
        clear();
        mvprintw(1, 2, "===== INSERIR ITEM PARA O PEDIDO %d =====", p.id);
        int item_linha = 3;

        itemPedido it;
        it.pedidoId = p.id;

        mvprintw(item_linha, 2, "ID do produto (0 para CANCELAR): ");
        produtoIdTemp = obterInteiroNcurses(item_linha, 36); 
        item_linha++;

        while (!produtoExisteCSV(produtoIdTemp)) {
            if (produtoIdTemp == 0) {
                mvprintw(item_linha + 2, 2, "Adicao de itens cancelada pelo usuario.");
                mvprintw(item_linha + 3, 2, "Pressione qualquer tecla para retornar.");
                refresh();
                getch();
                continuar = 0; 
                break; 
            }
         
            mvprintw(item_linha, 2, "ERRO: O produto com ID %d NAO existe! Digite outro ID (0 para CANCELAR).", produtoIdTemp);
        
            mvprintw(item_linha - 1, 36, "                    "); 
          
            produtoIdTemp = obterInteiroNcurses(item_linha - 1, 36);
            mvprintw(item_linha, 2, "                                                                                "); // Limpa a mensagem de erro
        }

  
        if (!continuar) continue;

        it.produtoId = produtoIdTemp;
        item_linha++;

        mvprintw(item_linha, 2, "Quantidade: ");
        it.quantidade = obterInteiroNcurses(item_linha, 14);
        item_linha++;

        mvprintw(item_linha, 2, "Subtotal do item: ");
        it.subtotal = obterDoubleNcurses(item_linha, 20);
        item_linha++;

        if (*qtdItens >= MAX_ITENS) {
            mvprintw(item_linha + 1, 2, "ERRO: Limite de itens de pedido atingido. Nao eh possivel adicionar mais.");
            getch();
            break;
        }

        itens[*qtdItens] = it;
        (*qtdItens)++;

        p.total += it.subtotal;

        mvprintw(item_linha + 1, 2, "Adicionar outro item? (1 = sim, 0 = nao): ");
        continuar = obterInteiroNcurses(item_linha + 1, 44);
    }

    if (p.total == 0 && *qtdItens > 0 && itens[*qtdItens-1].pedidoId == p.id) {
        
        if (continuar == 0) { 
            
        }
    }
    
    if (*qtdItens == 0 || p.total == 0) {
    
        
    }
    
    
    int itens_adicionados_ao_pedido = 0;
    for (int i = 0; i < *qtdItens; i++){
        if (itens[i].pedidoId == p.id) {
            itens_adicionados_ao_pedido = 1;
            break;
        }
    }

    if (!itens_adicionados_ao_pedido) {
         clear();
         mvprintw(2, 2, "Nenhum item adicionado. Pedido cancelado.");
         mvprintw(4, 2, "Pressione qualquer tecla para retornar ao menu...");
         refresh();
         getch();
         return; // Nao salva o pedido vazio
    }


   
    if (*qtdPedidos >= MAX_REGISTROS) {
        mvprintw(15, 2, "ERRO: Limite de pedidos atingido. Pedido nao inserido.");
    } else {
        pedidos[*qtdPedidos] = p;
        (*qtdPedidos)++;
        mvprintw(15, 2, "Pedido inserido com sucesso.");
    }
}


void listarPedidos(Pedido pedidos[], int qtdPedidos)
{
    clear();
    mvprintw(1, 2, "===== LISTA DE PEDIDOS =====");
    int linha = 3;

    if (qtdPedidos == 0) {
        mvprintw(linha, 2, "Nenhum pedido cadastrado.");
        return;
    }

    mvprintw(linha, 2, "ID | Cliente ID | Data       | Total");
    linha++;
    mvprintw(linha, 2, "---|------------|------------|--------");
    linha++;

    for (int i = 0; i < qtdPedidos; i++){
        mvprintw(linha + i, 2, "%d | %d | %s | %.2f",
                pedidos[i].id,
                pedidos[i].clienteid,
                pedidos[i].data,
                pedidos[i].total);
    }
}

void detalharPedido(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens)
{
    clear();
    mvprintw(1, 2, "===== DETALHAR PEDIDO =====");
    int linha = 3;

    int id;
    mvprintw(linha, 2, "Digite o ID do pedido: ");
    id = obterInteiroNcurses(linha, 25);
    linha += 2;

    int encontrado = -1;
     for (int i = 0; i < qtdPedidos; i++){
         if (pedidos[i].id == id){
             encontrado = i;
             break;
        }
    }

    if (encontrado == -1) {
        mvprintw(linha, 2, "Pedido nao encontrado.");
        return;
    }  

    Pedido p = pedidos[encontrado];
    mvprintw(linha, 2, "--- Detalhes do Pedido %d ---", p.id);
    linha++;
    mvprintw(linha, 2, "Cliente ID: %d", p.clienteid);
    linha++;
    mvprintw(linha, 2, "Data: %s", p.data);
    linha++;
    mvprintw(linha, 2, "Total: %.2f", p.total);
    linha += 2;

    mvprintw(linha, 2, "Itens:");
    linha++;
    mvprintw(linha, 2, "Produto ID | Quantidade | Subtotal");
    linha++;
    mvprintw(linha, 2, "-----------|------------|-----------");
    linha++;
    int itens_encontrados = 0;

    for (int i = 0; i < qtdItens; i++){
        if (itens[i].pedidoId == p.id){
            itens_encontrados++;
            mvprintw(linha, 2, "%10d | %10d | %9.2f",
                    itens[i].produtoId,
                     itens[i].quantidade,
                     itens[i].subtotal);
            linha++;
        }
    }
    if (itens_encontrados == 0) {
        mvprintw(linha, 2, "Nenhum item encontrado para este pedido.");
    }
}

void salvarPedidosCSV(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens)
{
    FILE *fp = fopen("Pedidos.csv", "w");
    if (!fp){
        mvprintw(20, 4, "Erro ao abrir Pedidos.csv para escrita.");
        return;
    }

    // bloco de pedidos
    fprintf(fp, "#PEDIDO\n");
    for (int i = 0; i < qtdPedidos; i++){
        
        fprintf(fp, "%d,%d,%s,%.2f\n",
             pedidos[i].id,
             pedidos[i].clienteid,
             pedidos[i].data,
             pedidos[i].total);
    }

    // bloco de itens
    fprintf(fp, "#ITENS\n");
    for (int i = 0; i < qtdItens; i++) {
        
        fprintf(fp, "%d,%d,%d,%.2f\n",
                 itens[i].pedidoId,
                 itens[i].produtoId,
                 itens[i].quantidade,
                 itens[i].subtotal);
    }

    fclose(fp);
    mvprintw(20, 4, "Pedidos salvos.");
}

void carregarPedidosCSV(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens)
{
    FILE *fp = fopen("Pedidos.csv", "r");
    if (!fp) {
        mvprintw(4, 2, "Arquivo Pedidos.csv nao encontrado. Nenhuma informacao de pedidos carregada.");
        return;
    }

    char linha[256];
    int modo = 0;  // 1 = lendo pedidos, 2 = lendo itens

    // Resetar contadores antes de carregar
    *qtdPedidos = 0;
    *qtdItens = 0;

    while (fgets(linha, sizeof(linha), fp)){

          if (strncmp(linha, "#PEDIDO", 7) == 0){
              modo = 1;
              continue;
        }
        if (strncmp(linha, "#ITENS", 6) == 0){
            modo = 2;
            continue;
        }
        
        // Verifica se ainda há espaço nas listas
        if (modo == 1 && *qtdPedidos < MAX_REGISTROS) {
           
            sscanf(linha, "%d,%d,%[^,],%lf",
                      &pedidos[*qtdPedidos].id,
                      &pedidos[*qtdPedidos].clienteid,
                      pedidos[*qtdPedidos].data,
                      &pedidos[*qtdPedidos].total);

        (*qtdPedidos)++;
        }
        else if (modo == 2 && *qtdItens < MAX_ITENS){
          
            sscanf(linha, "%d,%d,%d,%lf",
                     &itens[*qtdItens].pedidoId,
                     &itens[*qtdItens].produtoId,
                     &itens[*qtdItens].quantidade,
                     &itens[*qtdItens].subtotal);

        (*qtdItens)++;
        }
    }

    fclose(fp);
    mvprintw(4, 2, "Pedidos carregados.");
}