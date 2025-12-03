#include <ncurses.h>

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

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (1){
        clear();
        mvprintw(1, 2, "===== MENU DE PEDIDOS =====");

        //Opcoes
        for (int i = 0; i < qtdOpcoes; i++) {
            if (i == selecionado) {
                attron(A_REVERSE);  // destaca
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
        else if (ch == '\n') {  // ENTER - escolhe a opção
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
                    endwin();  // encerra ncurses
                    return;
            }

            mvprintw(20, 4, "Pressione qualquer tecla para voltar ao menu");
            getch();
        }
    }
    endwin(); // segurança
}

    void inserirPedido(Pedido pedidos[], int *qtdPedidos, itemPedido itens[], int *qtdItens)
    {
        Pedido p;
        p.id = *qtdPedidos + 1;

        printf("ID do cliente: ");
        scanf("%d", &p.clienteid);

        printf("Data (DD/MM/AAAA): ");
        scanf("%s", p.data);

        p.total = 0;

        int continuar = 1;
        while (continuar){
            itemPedido it;
            it.pedidoId = p.id;

            printf("ID do produto: ");
            scanf("%d", &it.produtoId);

            printf("Quantidade: ");
            scanf("%d", &it.quantidade);

            printf("Subtotal do item: ");
            scanf("%lf", &it.subtotal);

            itens[*qtdItens] = it;
            (*qtdItens)++;

            p.total += it.subtotal;

            printf("Adicionar outro item? (1 = sim, 0 = nao): ");
            scanf("%d", &continuar);
        }

    pedidos[*qtdPedidos] = p;
    (*qtdPedidos)++;

    printf("Pedido inserido.\n");
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