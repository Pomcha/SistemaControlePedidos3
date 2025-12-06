#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "produto.h"
#include "globals.h"



void persistirCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ) {
    FILE *arquivo = fopen("Clientes.csv", "w");
    if(arquivo == NULL) {
        printf("ERRO: Nao foi possivel criar o arquivo Clientes.csv\n");
        return;
    }
    fprintf(arquivo, "Tipo;ID;Nome;Endereco;Telefone;Email;Documento;Contato\n");
    for(int i = 0; i < totalPF; i++) {
    fprintf(arquivo, "PF;%d;%s;%s;%s;%s;%s;%s\n", // Alterado para terminar em %s\n
    listaPF[i].dados.id, listaPF[i].dados.nome, listaPF[i].dados.endereco, 
    listaPF[i].dados.telefone, listaPF[i].dados.email, listaPF[i].cpf, ""); // Adicionado um "" para o campo Contato
    }
    for(int i = 0; i < totalPJ; i++) {
        fprintf(arquivo, "PJ;%d;%s;%s;%s;%s;%s;%s\n", listaPJ[i].dados.id, listaPJ[i].dados.nome, listaPJ[i].dados.endereco, listaPJ[i].dados.telefone, listaPJ[i].dados.email, listaPJ[i].cnpj, listaPJ[i].nomeContato);
    }
    fclose(arquivo);
    printf("Clientes salvos no arquivo Clientes.csv!\n");
}



// função persistir produto

void persistirProdutos(Produto listaProdutos[], int totalProdutos) {
    FILE *arquivo = fopen("Produtos.csv", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo Produtos.csv!\n");
        return;
    }

    // Cabeçalho do CSV
    fprintf(arquivo, "id;descricao;preco;estoque\n");

    // Grava cada produto
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(
            arquivo,
            "%d;%s;%.2lf;%d\n",
            listaProdutos[i].id,
            listaProdutos[i].descricao,
            listaProdutos[i].preco,
            listaProdutos[i].estoque
        );
    }

    fclose(arquivo);
    printf("Produtos salvos com sucesso em Produtos.csv!\n");
}



void carregarClientesCSV(
    Cliente listaClientes[], int *totalClientes, 
    PessoaFisica listaPF[], int *totalPF, 
    PessoaJuridica listaPJ[], int *totalPJ
) {
    FILE *arquivo = fopen("Clientes.csv", "r");
    char linha[512];
    int modo = 0; // 1 = PF, 2 = PJ

    *totalPF = 0;
    *totalPJ = 0;

    if (arquivo == NULL) {
        printf("Arquivo Clientes.csv nao encontrado. Iniciando com dados vazios.\n");
        return; // Retorna se o arquivo não existir (primeira execução)
    }

    // Pular o cabeçalho
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char *token = strtok(linha, ";");
        if (token == NULL) continue;

        if (strcmp(token, "PF") == 0) {
            modo = 1;
        } else if (strcmp(token, "PJ") == 0) {
            modo = 2;
        } else {
            continue; // Linha inválida
        }

        if (modo == 1 && *totalPF < MAX_REGISTROS) { // Pessoa Física
            PessoaFisica *pf = &listaPF[*totalPF];
            
            // 1. ID
            token = strtok(NULL, ";");
            if (token) pf->dados.id = atoi(token);

            // 2. Nome
            token = strtok(NULL, ";");
            if (token) strcpy(pf->dados.nome, token);

            // 3. Endereco
            token = strtok(NULL, ";");
            if (token) strcpy(pf->dados.endereco, token);

            // 4. Telefone
            token = strtok(NULL, ";");
            if (token) strcpy(pf->dados.telefone, token);

            // 5. Email
            token = strtok(NULL, ";");
            if (token) strcpy(pf->dados.email, token);

            // 6. CPF (Documento)
            token = strtok(NULL, ";");
            if (token) {
                // Remove o '\n' se for o último token
                size_t len = strlen(token);
                if (len > 0 && token[len - 1] == '\n') {
                    token[len - 1] = '\0';
                }
                strcpy(pf->cpf, token);
            }
            
            // O campo Contato é ignorado para PF
            
            (*totalPF)++;
        } 
        else if (modo == 2 && *totalPJ < MAX_REGISTROS) { // Pessoa Jurídica
            PessoaJuridica *pj = &listaPJ[*totalPJ];

            // 1. ID
            token = strtok(NULL, ";");
            if (token) pj->dados.id = atoi(token);

            // 2. Nome
            token = strtok(NULL, ";");
            if (token) strcpy(pj->dados.nome, token);

            // 3. Endereco
            token = strtok(NULL, ";");
            if (token) strcpy(pj->dados.endereco, token);

            // 4. Telefone
            token = strtok(NULL, ";");
            if (token) strcpy(pj->dados.telefone, token);

            // 5. Email
            token = strtok(NULL, ";");
            if (token) strcpy(pj->dados.email, token);

            // 6. CNPJ (Documento)
            token = strtok(NULL, ";");
            if (token) strcpy(pj->cnpj, token);

            // 7. Nome Contato
            token = strtok(NULL, ";");
            if (token) {
                // Remove o '\n'
                size_t len = strlen(token);
                if (len > 0 && token[len - 1] == '\n') {
                    token[len - 1] = '\0';
                }
                strcpy(pj->nomeContato, token);
            }
            
            (*totalPJ)++;
        }
    }

    *totalClientes = *totalPF + *totalPJ;
    fclose(arquivo);
    printf("Clientes carregados com sucesso!\n");
}