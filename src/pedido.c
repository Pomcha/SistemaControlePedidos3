#include <stdio.h>
#include <string.h>
#include <ncurses.h>

//  analisar ~

int produtoExisteCSV(int idBuscado)
{
    FILE *fp = fopen("Produtos.csv", "r");
    if (!fp) {
        printf("ERRO: Nao foi possivel abrir produtos.csv\n");
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
        printf("ERRO: Nao foi possivel abrir clientes.csv\n");
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


int pedidoExisteCSV(int idBuscado)
{
    FILE *fp = fopen("Pedidos.csv", "r");
    if (!fp) {
        printf("ERRO: Nao foi possivel abrir pedidos.csv\n");
        return 0;
    }

    int id;
    char linha[300];

    fgets(linha, sizeof(linha), fp); // pular cabecalho

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








    //  Funcoes principais do menu


//  cadastrar o pedido
void inserirPedido(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens)
{
    Pedido p;

    // 1. Usuário informa o ID do pedido
    printf("ID do pedido: ");
    scanf("%d", &p.id);

    // 2. Verifica se o ID já existe
    while (pedidoExisteCSV(p.id)) {
        printf("\nERRO: O pedido com ID %d JA existe!\n", p.id);
        printf("Digite novamente o ID do pedido: ");
        scanf("%d", &p.id);
    }

    // 3. ADICIONA A DATA AQUI AGORA
    printf("Data (DD/MM/AAAA): ");
    scanf("%s", p.data);

    // 4. Usuário informa o cliente
    int clienteIdTemp;
    printf("ID do cliente: ");
    scanf("%d", &clienteIdTemp);

    // 5. Verifica cliente
    while (!clienteExisteCSV(clienteIdTemp)) {
        printf("\nERRO: O cliente com ID %d NAO existe!\n", clienteIdTemp);
        printf("Digite novamente o ID do cliente: ");
        scanf("%d", &clienteIdTemp);
    }

    p.clienteid = clienteIdTemp;

    p.total = 0;

    // 6. Cadastra itens do pedido
    int continuar = 1;
    int produtoIdTemp;

    while (continuar) {
        itemPedido it;
        it.pedidoId = p.id;

        printf("\nID do produto: ");
        scanf("%d", &produtoIdTemp);

        while (!produtoExisteCSV(produtoIdTemp)) {
            printf("\nERRO: O produto com ID %d NAO existe!\n", produtoIdTemp);
            printf("Digite novamente o ID do produto: ");
            scanf("%d", &produtoIdTemp);
        }

        it.produtoId = produtoIdTemp;

        printf("Quantidade: ");
        scanf("%d", &it.quantidade);

        printf("Subtotal do item: ");
        scanf("%lf", &it.subtotal);

        itens[*qtdItens] = it;
        (*qtdItens)++;

        p.total += it.subtotal;

        printf("\nAdicionar outro item? (1 = sim, 0 = nao): ");
        scanf("%d", &continuar);
    }

    // Salva pedido finalizado
    pedidos[*qtdPedidos] = p;
    (*qtdPedidos)++;

    printf("\nPedido inserido com sucesso.\n");
}



void listarPedidos(Pedido pedidos[], int qtdPedidos)
{
    if (qtdPedidos == 0) {
    printf("Nenhum pedido cadastrado.\n");
        return;
    }

    for (int i = 0; i < qtdPedidos; i++){
        printf("Pedido %d | Cliente: %d | Data: %s | Total: %.2f\n",
               pedidos[i].id,
               pedidos[i].clienteid,
               pedidos[i].data,
               pedidos[i].total);
    }
}

void detalharPedido(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens)
{
    int id;
    printf("Digite o ID do pedido: ");
    scanf("%d", &id);

    int encontrado = -1;
      for (int i = 0; i < qtdPedidos; i++){
         if (pedidos[i].id == id){
             encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Pedido nao encontrado.\n");
        return;
    }   

    Pedido p = pedidos[encontrado];
    printf("\n--- Detalhes do Pedido %d ---\n", p.id);
    printf("Cliente: %d\n", p.clienteid);
    printf("Data: %s\n", p.data);
    printf("Total: %.2f\n", p.total);

    printf("\nItens:\n");
    for (int i = 0; i < qtdItens; i++){
        if (itens[i].pedidoId == p.id){
            printf("Produto %d | Qtde: %d | Subtotal: %.2f\n",
                   itens[i].produtoId,
                    itens[i].quantidade,
                    itens[i].subtotal);
        }
    }
}

void salvarPedidosCSV(Pedido pedidos[], int qtdPedidos, itemPedido itens[], int qtdItens)
{
    FILE *fp = fopen("Pedidos.csv", "w");
    if (!fp){
        printf("Erro ao abrir Pedidos.csv\n");
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
    printf("Pedidos salvos.\n");
}

void carregarPedidosCSV(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens)
{
    FILE *fp = fopen("Pedidos.csv", "r");
    if (!fp) {
        printf("Arquivo Pedidos.csv nao encontrado.\n");
        return;
    }

    char linha[256];
    int modo = 0;  // 1 = lendo pedidos, 2 = lendo itens

    while (fgets(linha, sizeof(linha), fp)){

         if (strncmp(linha, "#PEDIDO", 7) == 0){
             modo = 1;
             continue;
        }
        if (strncmp(linha, "#ITENS", 6) == 0){
            modo = 2;
            continue;
        }

        if (modo == 1){
            sscanf(linha, "%d,%d,%[^,],%lf",
                    &pedidos[*qtdPedidos].id,
                    &pedidos[*qtdPedidos].clienteid,
                    pedidos[*qtdPedidos].data,
                    &pedidos[*qtdPedidos].total);

        (*qtdPedidos)++;
        }
        else if (modo == 2){
            sscanf(linha, "%d,%d,%d,%lf",
                   &itens[*qtdItens].pedidoId,
                   &itens[*qtdItens].produtoId,
                   &itens[*qtdItens].quantidade,
                   &itens[*qtdItens].subtotal);

        (*qtdItens)++;
        }
    }

    fclose(fp);
    printf("Pedidos carregados.\n");
}