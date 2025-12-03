//menu produto
#include <ncurses.h>

void inserirProduto();
void consultarProduto();
void listarProdutos();
void removerProduto();

void mostrarMenuProduotos() {
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
        mvprintw(1, 2, "MENU DE PRODUTOS");

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

            case '/n':
            if (highlight = 0) inserir Produto();
            else if (highlight == 1) consultarProduto();
            else if (highlight == 2) listarProdutos();
            else if (highlight == 3) removerProduto();
            else if (highlight == 4) return; break; 
        }
    }
}


