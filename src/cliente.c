#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//STRUCTS
//Cliente:
typedef struct {
    int id;
    char tipo[2];
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];
} Cliente;

//PessoaFisica:

typedef struct {
    Cliente dados;
    char cpf[20];
} PessoaFisica;

//PessoaJuridica:

typedef struct {
    Cliente dados;
    char cnpj[20];
    char nomeContato[100];
} PessoaJuridica;

//VERIFICACOES

int verificarID(int id, Cliente lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(lista[i].id == id)
        return 1;
    }
    return 0;
}

int verificarCPF(char cpf[], PessoaFisica lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(strcmp(lista[i].cpf, cpf) == 0)
        return 1;
    }
    return 0;
}

int verificarCNPJ(char cnpj[], PessoaJuridica lista[], int total) {
    for(int i = 0; i < total; i++) {
        if(strcmp(lista[i].cnpj, cnpj) == 0)
        return 1;
    }
    return 0;
}

//AnalisarCliente:

int analisarCliente(int id, char documento[], Cliente listaClientes[], int totalClientes, PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ, char tipo) {
    if(verificarID(id, listaClientes, totalClientes))
    return 1;
    if(tipo == 'F') {
        if(verificarCPF(documento, listaPF, totalPF))
        return 2;
    } else if(tipo == 'J') {
        if(verificarCNPJ(documento, listaPJ, totalPJ))
        return 2;
    }
    return 0;
}

//CadastrarCliente:

int cadastrarCliente(Cliente clientes[], int *totalClientes, PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ) {
    int id;
    char nome[100], endereco[200], telefone[20], email[100];
    char documento[30];
    char tipo;

    printf("Cliente Pessoa Fisica(F) ou Juridica(J)? ");
    scanf(" %c", &tipo);

    printf("Digite o ID: ");
    scanf("%d", &id);

    printf("Nome: ");
    scanf(" %[^\n]", nome);

    printf("Endereco: ");
    scanf(" %[^\n]", endereco);

    printf("Telefone: ");
    scanf(" %[^\n]", telefone);

    printf("E-mail: ");
    scanf(" %[^\n]", email);

    if(tipo == 'F'){
        printf("CPF: ");
        scanf(" %[^\n]", documento);
    } else {
        printf("CNPJ: ");
        scanf(" %[^\n]", documento);
    }
    int status = analisarCliente(id, documento, clientes, *totalClientes, listaPF, *totalPF, listaPJ, *totalPJ, tipo);
    if(status == 1) {
        printf("ERRO: ID ja existe.\n"); 
        return 1;
    }
    if(status == 2) {
        printf("ERRO: CPF/CNPJ ja existe.\n");
        return 2;
    }
    //CadastrarCliente base:
    clientes[*totalClientes].id = id;
    strcpy(clientes[*totalClientes].tipo, (tipo=='F' ? "F" : "J"));
    strcpy(clientes[*totalClientes].nome, nome);
    strcpy(clientes[*totalClientes].endereco, endereco);
    strcpy(clientes[*totalClientes].telefone, telefone);
    strcpy(clientes[*totalClientes].email, email);
    (*totalClientes)++;

    //CadastrarPF:

    if(tipo == 'F') {
        listaPF[*totalPF].dados = clientes[*totalClientes-1];
        strcpy(listaPF[*totalPF].cpf, documento);
        (*totalPF)++;
    }
    
    //CadastrarPJ:
    else {
        listaPJ[*totalPJ].dados = clientes[*totalClientes-1];
        strcpy(listaPJ[*totalPJ].cnpj, documento);
        printf("Nome do responsavel: ");
        scanf(" %[^\n]", listaPJ[*totalPJ].nomeContato);
        (*totalPJ)++;
    }
    printf("Cliente cadastrado com sucesso!\n");
    return 0;
}

//RemoverCliente:

int removerClientePF(char cpf[], PessoaFisica listaPF[], int *totalPF) {
    int pos = -1;
    for(int i =0; i < *totalPF; i++){ 
        if(strcmp(listaPF[i].cpf, cpf) == 0) {
            pos = i;
            break;
        }
    }
    if(pos == -1) return 0;
    for(int i = pos; i < *totalPF -1; i++)
    listaPF[i] = listaPF[i+1];
    (*totalPF)--;
    return 1;
}
int removerClientePJ(char cnpj[], PessoaJuridica listaPJ[], int *totalPJ) {
    int pos = -1;
    for(int i = 0; i<*totalPJ; i++){
        if(strcmp(listaPJ[i].cnpj, cnpj) == 0){
            pos = i;
            break;
        }
    }
    if(pos == -1) return 0;
    for(int i = pos; i < *totalPJ - 1; i++)
    listaPJ[i] = listaPJ[i+1];
    (*totalPJ)--;
    return 1;
}

void removerCliente(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ){
    char documento[30];
    int tipo;
    printf("Tipo de cliente (1=PF , 2=PJ): ");
    scanf("%d", &tipo);

    printf("Informe o CPF ou CNPJ: ");
    scanf("%s", documento);
    int sucesso = 0;
    if(tipo ==1) 
    sucesso = removerClientePF(documento, listaPF, totalPF);
    else if(tipo ==2)
    sucesso = removerClientePJ(documento, listaPJ, totalPJ);
    if(sucesso)
    printf("Cliente removido com sucesso!\n");
    else 
    printf("Cliente nao encontrado!\n");
}
//ListarClientes:

void listarClientes(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    printf("\n======LISTA DE CLIENTES======\n");
    printf("\n---Pessoa Fisica---\n");
    for(int i = 0; i < totalPF; i++) {
        printf("ID: %d\n", listaPF[i].dados.id);
        printf("Nome: %s\n", listaPF[i].dados.nome);
        printf("CPF: %s\n\n", listaPF[i].cpf);
    }
    printf("\n---Pessoa Juridica---\n");
    for(int i = 0; i < totalPJ; i++) {
        printf("ID: %d\n", listaPJ[i].dados.id);
        printf("Empresa: %s\n", listaPJ[i].dados.nome);
        printf("CNPJ: %s\n", listaPJ[i].cnpj);
        printf("Contato: %s\n\n", listaPJ[i].nomeContato);
    }
    printf("===============================\n");
}