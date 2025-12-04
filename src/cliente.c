//struct_cliente:
#include <stdio.h>
#include <string.h>
typedef struct{
    int id;
    char tipo[2];
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];
} Cliente;

//struct_pf
typedef struct{
    Cliente dados;
    char cpf[20];
} PessoaFisica;

// funcao: verificar se ID ja existe
int verificarID(int id, Cliente lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(lista[i].id == id) {
            return 1;
        }
    }
    return 0;
}

//funcao: verificar se CPF ja existe
int verificarCPF(char cpf[], PessoaFisica lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(strcmp(lista[i].cpf, cpf) == 0) {
            return 1;
        }
    }
    return 0;
}

//analisar_cliente
int analisarCliente(int id, char documento[], Cliente listaCliente[], int totalClientes, PessoaFisica listaPF[], int totalPF) {
    if(verificarID(id, listaCliente, totalClientes))
    return 1;
    if(verificarCPF(documento, listaPF, totalPF))
    return 2;

    return 0;
}