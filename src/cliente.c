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

//cadastrar_cliente
int cadastrarCliente(Cliente listaClientes[], int *totalClientes, PessoaFisica listaPF[], int *totalPF) {
    int id;
    char nome[100], endereco[200], telefone[20], email[100];
    char cpf[20];
    printf("Digite o ID do cliente: ");
    scanf("%d", &id);

    printf("Digite o nome: ");
    scanf(" %[^\n]", nome);

    printf("Digite o endereco: ");
    scanf(" %[^\n]", endereco);

    printf("Digite o telefone: ");
    scanf(" %[^/n]", telefone);

    printf("Digite o email: ");
    scanf(" %[^\n]", email );

    printf("Digite o CPF: ");
    scanf(" %[^\n]", cpf);

    int status = analisarCliente(id, cpf, listaClientes, *totalClientes, listaPF, *totalPF);
    if(status==1) {
        printf("ERRO: ID ja cadastrado\n");
        return 1;
    }
    if(status==2){
        printf("ERRO: CPF ja cadastrado\n");
    }
    listaClientes[*totalClientes].id = id;
    strcpy(listaClientes[*totalClientes].tipo, "F");
    strcpy(listaClientes[*totalClientes].nome, nome);
    strcpy(listaClientes[*totalClientes].endereco, endereco);
    strcpy(listaClientes[*totalClientes].telefone, telefone);
    strcpy(listaClientes[*totalClientes].email, email);
    (*totalClientes)++;

    listaPF[*totalPF].dados.id = id;
    strcpy(listaPF[*totalPF].dados.nome, nome);
    strcpy(listaPF[*totalPF].dados.endereco, endereco);
    strcpy(listaPF[*totalPF].dados.telefone, telefone);
    strcpy(listaPF[*totalPF].dados.email, email);
    strcpy(listaPF[*totalPF].cpf, cpf);
    (*totalPF)++;

    printf("Cliente cadastrado com sucesso!\n");
    return 0;

}