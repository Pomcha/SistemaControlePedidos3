#include <ncurses.h>
#define ENTER 10
//funcao do menu de clientes
void mostrarMenuClientes(){
    char *opcoes[]={
        "Cadastrar Cliente",
        "Consultar Cliente",
        "Remover Cliente",
        "Listar Clientes",
        "Voltar ao Menu Principal"
    };
    int n_opcoes = 5;
    int escolha = 0;
    int tecla;
    initscr(); //Inicia o ncurses
    noecho(); //nao exibe caracteres digitados
    cbreak(); //entrada imediata sem buffer
    keypad(stdscr, TRUE); //ativa teclas especiais (setas)

    while(1){
        clear();
        mvprintw(1,2,"=====MENU-CLIENTES=====\n");
        //imprimir opcoes
        for(int i=0; i < n_opcoes; i++){
            if(i==escolha){
                attron(A_REVERSE); //destaca a opcao selecionada
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
            if(escolha >= n_opcoes) escolha =0;
            break;
            case ENTER:
            clear();
            
            switch(escolha){
                case 0:
                mvprintw(5,5,">> Cadastrar Cliente selecionado.");
                //inserirCliente();
                break;
                case 1:
                mvprintw(5,5,">> Consultar Cliente selecionado.");
                //consultarCliente();
                break;
                case 2:
                mvprintw(5,5,">> Remover Cliente selecionado.");
                //removerCliente();
                break;
                case 3:
                mvprintw(5,5,">> Listar Clientes selecionados.");
                //listarClientes();
                break;
                case 4:
                endwin(); //finalizar o ncurses
                return; //volta ao menu principal
            }
            mvprintw(10,4,"Pressione qualquer tecla para continuar...");
            getch();
            break;
        }
    }
    endwin(); //caso saia do loop
}