#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char tipo[2];
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];
} Cliente;

typedef struct {
    Cliente dados;
    char cpf[20];
} PessoaFisica;

typedef struct {
    Cliente dados;
    char cnpj[20];
    char nomeContato[100];
} PessoaJuridica;

void persistirCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    FILE *arquivo = fopen("Clientes.csv", "w");
    if(arquivo == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo Clientes.csv\n");
        return;
    }
    fprintf(arquivo, "Tipo;ID;Nome;Endereco;Telefone;Email;Documento;Contato\n");
    for(int i = 0; i <totalPF; i++) {
        fpritnf(arquivo, "PF;%d;%s;%s%s;%s;%s;\n", listaPF[i].dados.id, listaPF[i].dados.nome, listaPF[i].dados.endereco, listaPF[i].dados.telefone, listaPF[i].dados.email, listaPF[i].cpf);
    }
    for(int i = 0; i < totalPJ; i++) {
        fprintf(arquivo, "PJ;%d;%s;%s;%s;%s;%s\n", listaPJ[i].dados.id, listaPJ[i].dados.nome, listaPJ[i].dados.endereco, listaPJ[i].dados.telefone, listaPJ[i].dados.email, listaPJ[i].cnpj, listaPJ[i].nomeContato);
    }
    fclose(arquivo);
    printf("Clientes salvos no arquivo Clientes.csv!\n");
}