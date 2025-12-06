#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "globals.h" 
#include "cliente.h" 
#include "validadores.h"

// ===========================================
// ALGORITMOS DE VALIDAÇÃO
// ===========================================

// Verifica se CNPJ já existe na lista
int verificarCNPJ(char cnpj[], PessoaJuridica lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(strcmp(lista[i].cnpj, cnpj) == 0) return 1;
    }
    return 0;
}

// Verifica ID em ambas as listas (PF e PJ) para garantir unicidade global
int verificarIDGlobal(int id, PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    for(int i=0; i<totalPF; i++) if(listaPF[i].dados.id == id) return 1;
    for(int i=0; i<totalPJ; i++) if(listaPJ[i].dados.id == id) return 1;
    return 0;
}

// ===========================================
// FUNÇÕES DE CADASTRO
// ===========================================

int cadastrarClientePF(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    clear();
    mvprintw(1, 4, "==== CADASTRO DE CLIENTE (PESSOA FISICA) ====");

    if (*totalPF >= MAX) {
        mvprintw(3, 4, "ERRO: Limite de clientes atingido.");
        getch();
        return 0;
    }

    int id;
    char cpf[20], nome[100], endereco[200], telefone[20], email[100];
    int y = 3;

    echo(); curs_set(1);

    mvprintw(y, 4, "ID: "); scanw("%d", &id);
    
    if (verificarIDGlobal(id, listaPF, *totalPF, listaPJ, totalPJ)) {
        noecho();
        mvprintw(y+1, 4, "ERRO: ID ja existe!");
        getch(); return 0;
    }

    mvprintw(++y, 4, "Nome: "); getstr(nome);
    
    mvprintw(++y, 4, "CPF (apenas numeros): "); getstr(cpf);
    
    if (!validarCPF(cpf)) {
        noecho(); mvprintw(++y, 4, "ERRO: CPF Invalido!"); getch(); return 0;
    }
    
    for(int i=0; i<*totalPF; i++) {
        if(strcmp(listaPF[i].cpf, cpf)==0) {
            noecho(); mvprintw(++y, 4, "ERRO: CPF ja cadastrado!"); getch(); return 0;
        }
    }

    mvprintw(++y, 4, "Endereco: "); getstr(endereco);
    mvprintw(++y, 4, "Telefone: "); getstr(telefone);
    mvprintw(++y, 4, "Email: "); getstr(email);

    noecho(); curs_set(0);

    listaPF[*totalPF].dados.id = id;
    strcpy(listaPF[*totalPF].dados.tipo, "F");
    strcpy(listaPF[*totalPF].dados.nome, nome);
    strcpy(listaPF[*totalPF].dados.endereco, endereco);
    strcpy(listaPF[*totalPF].dados.telefone, telefone);
    strcpy(listaPF[*totalPF].dados.email, email);
    strcpy(listaPF[*totalPF].cpf, cpf);
    (*totalPF)++;

    mvprintw(++y, 4, "Sucesso! Pressione qualquer tecla.");
    getch();
    return 1;
}

int cadastrarClientePJ(PessoaJuridica listaPJ[], int *totalPJ, PessoaFisica listaPF[], int totalPF) {
    clear();
    mvprintw(1, 4, "==== CADASTRO DE CLIENTE (PESSOA JURIDICA) ====");

    if (*totalPJ >= MAX) {
        mvprintw(3, 4, "ERRO: Limite de clientes atingido.");
        getch();
        return 0;
    }

    int id;
    char cnpj[20], razaoSocial[100], nomeContato[100], endereco[200], telefone[20], email[100];
    int y = 3;

    echo(); curs_set(1);

    mvprintw(y, 4, "ID: "); scanw("%d", &id);
    if (verificarIDGlobal(id, listaPF, totalPF, listaPJ, *totalPJ)) {
        noecho(); mvprintw(y+1, 4, "ERRO: ID ja existe!"); getch(); return 0;
    }

    // 7. II. Razão social e nome de contato 
    mvprintw(++y, 4, "Razao Social: "); getstr(razaoSocial);
    mvprintw(++y, 4, "Nome Contato: "); getstr(nomeContato); 
    mvprintw(++y, 4, "CNPJ (apenas numeros): "); getstr(cnpj);

    // 4. II. Valida CNPJ 
    if (!validarCNPJ(cnpj)) {
        noecho(); mvprintw(++y, 4, "ERRO: CNPJ Invalido!"); getch(); return 0;
    }

    if (verificarCNPJ(cnpj, listaPJ, *totalPJ)) {
        noecho(); mvprintw(++y, 4, "ERRO: CNPJ ja cadastrado!"); getch(); return 0;
    }

    mvprintw(++y, 4, "Endereco: "); getstr(endereco);
    mvprintw(++y, 4, "Telefone: "); getstr(telefone);
    mvprintw(++y, 4, "Email: "); getstr(email);

    noecho(); curs_set(0);

    // Salvar
    listaPJ[*totalPJ].dados.id = id;
    strcpy(listaPJ[*totalPJ].dados.tipo, "J");
    strcpy(listaPJ[*totalPJ].dados.nome, razaoSocial);
    strcpy(listaPJ[*totalPJ].dados.endereco, endereco);
    strcpy(listaPJ[*totalPJ].dados.telefone, telefone);
    strcpy(listaPJ[*totalPJ].dados.email, email);
    strcpy(listaPJ[*totalPJ].cnpj, cnpj);
    strcpy(listaPJ[*totalPJ].nomeContato, nomeContato);
    (*totalPJ)++;

    mvprintw(++y, 4, "PJ Cadastrada com Sucesso!");
    getch();
    return 1;
}

// ===========================================
// FUNÇÕES DE CONSULTA, REMOÇÃO E LISTAGEM
// ===========================================

// Função auxiliar visual para exibir dados
void exibirDadosCliente(Cliente c, char *docExtra, char *contatoExtra) {
    clear();
    mvprintw(2, 4, "=== DETALHES DO CLIENTE ===");
    mvprintw(4, 4, "ID: %d", c.id);
    mvprintw(5, 4, "Tipo: %s", (strcmp(c.tipo, "F")==0) ? "Pessoa Fisica" : "Pessoa Juridica");
    mvprintw(6, 4, "Nome/Razao: %s", c.nome);
    if(contatoExtra[0] != '\0') mvprintw(6, 40, "Contato: %s", contatoExtra);
    mvprintw(7, 4, "Documento: %s", docExtra);
    mvprintw(8, 4, "Endereco: %s", c.endereco);
    mvprintw(9, 4, "Telefone: %s", c.telefone);
    mvprintw(10, 4, "Email: %s", c.email);
}

void telaConsultarCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    int idBusca, encontrado = 0;
    echo(); curs_set(1);
    clear();
    mvprintw(2, 4, "Digite o ID do Cliente para consultar: ");
    scanw("%d", &idBusca);
    noecho(); curs_set(0);

    // Busca em PF
    for(int i=0; i<totalPF; i++) {
        if(listaPF[i].dados.id == idBusca) {
            exibirDadosCliente(listaPF[i].dados, listaPF[i].cpf, "");
            encontrado = 1; break;
        }
    }
    // Se não achou, Busca em PJ
    if(!encontrado) {
        for(int i=0; i<totalPJ; i++) {
            if(listaPJ[i].dados.id == idBusca) {
                exibirDadosCliente(listaPJ[i].dados, listaPJ[i].cnpj, listaPJ[i].nomeContato);
                encontrado = 1; break;
            }
        }
    }

    // CORREÇÃO: Remove a tag de referência
    if(!encontrado) mvprintw(4, 4, "Cliente nao existe.");
    
    mvprintw(12, 4, "Pressione qualquer tecla para voltar.");
    getch();
}

void telaRemoverCliente(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ) {
    int idBusca, index = -1, tipo = 0; // 1=PF, 2=PJ
    echo(); curs_set(1);
    clear();
    mvprintw(2, 4, "Digite o ID do Cliente para REMOVER: ");
    scanw("%d", &idBusca);
    noecho(); curs_set(0);

    // Buscar índice
    for(int i=0; i<*totalPF; i++) {
        if(listaPF[i].dados.id == idBusca) { index=i; tipo=1; break; }
    }
    if(index == -1) {
        for(int i=0; i<*totalPJ; i++) {
            if(listaPJ[i].dados.id == idBusca) { index=i; tipo=2; break; }
        }
    }

    if(index == -1) {
        // CORREÇÃO: Remove as tags e
        mvprintw(4, 4, "Cliente nao existe.");
        getch(); return;
    }

    mvprintw(4, 4, "Tem certeza que deseja remover o cliente ID %d? (S/N): ", idBusca);
    int confirm = getch();
    if(confirm == 'S' || confirm == 's') {
        if(tipo == 1) { // Remover PF
             for(int i=index; i < *totalPF -1; i++) listaPF[i] = listaPF[i+1];
             (*totalPF)--;
        } else { // Remover PJ
             for(int i=index; i < *totalPJ -1; i++) listaPJ[i] = listaPJ[i+1];
             (*totalPJ)--;
        }
        mvprintw(6, 4, "Cliente removido com sucesso.");
    } else {
        mvprintw(6, 4, "Operacao cancelada.");
    }
    getch();
}

void listarTodosClientes(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    clear();
    int row = 2;
    mvprintw(0, 4, "=== LISTAGEM GERAL DE CLIENTES ===");
    
    attron(A_BOLD);
    mvprintw(row++, 2, "%-5s %-20s %-15s %-15s", "ID", "Nome/Razao", "Documento", "Tipo");
    attroff(A_BOLD);

    // Lista PF
    for(int i=0; i<totalPF; i++) {
        mvprintw(row++, 2, "%-5d %-20s %-15s %-15s", 
            listaPF[i].dados.id, listaPF[i].dados.nome, listaPF[i].cpf, "Fisica");
    }
    // Lista PJ
    for(int i=0; i<totalPJ; i++) {
        mvprintw(row++, 2, "%-5d %-20s %-15s %-15s", 
            listaPJ[i].dados.id, listaPJ[i].dados.nome, listaPJ[i].cnpj, "Juridica");
    }

    mvprintw(row+2, 4, "Pressione qualquer tecla para voltar.");
    getch();
}

// ===========================================
// FUNÇÕES DE SUPORTE À LISTA (Requisito Estrutural)
// ===========================================

void inicializarLista(ListaClientes *lista){
    lista->tamanho = 0;
}