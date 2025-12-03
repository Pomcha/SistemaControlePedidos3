    //  mostrar o menu de pedidos

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