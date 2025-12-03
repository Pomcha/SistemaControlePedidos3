//validar cnpj
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validadores.h"

static void limparCNPJ(const char *orig, char *dest) {
    //remove caracteres não numéricos do CNPJ
    int j = 0;
    for (int i = 0; orig[i] != '\0'; i++) {
        if(isdigit(orig[i])) {
            dest[j++] = orig[i];
        }
    }
    dest[j] = '\0';
}
int validarCNPJ(const char *cnpj) {
    char numeros[15];
    limparCNPJ(cnpj, numeros);

    if(strlen(numeros) != 14)
    //CNPJ tem que ter exatamente 14 digitos
    return 0;

    int todosiguais = 1;
    for(int i = 1; i < 14; i++) {
        if(numeros[i] != numeros[0]) {
            todosiguais = 0;
            break;
        }
    }
    if (todosiguais)
    return 0;

    int pesos1[12] = {5,4,3,2,9,8,7,6,5,4,3,2};
    int soma = 0;

    for(int i = 0; i < 12; i++)
        soma += (numeros[i] - '0') *pesos2[i];

    resto = soma % 11
    int digito2 = (resto < 2) ? 0 : 11 - resto;

    if (digito2 != (numeros[13] - '0'))
    return 0;

    return 1;
}



// caso de uso análisar produto 
#include "produto.h"

int analisarProduto(Produto listaProdutos[], int totalProdutos, int idBuscado) {
    for (int i = 0; i < totalProdutos; i++) {
        if (listaProdutos[i].id == idBuscado) {
            return 1; // Encontrado
        }
    }
    return 0; // Não encontrado
}




// caso de uso cadastrar produto
#include <stdio.h>
#include <string.h>
#include "produto.h"

void cadastrarProduto(Produto listaProdutos[], int *totalProdutos) {
    Produto novo;
    int id;

    printf("Informe o ID do Produto: ");
    scanf("%d", &id);

    // 1 - Verificar se o ID já existe
    if (analisarProduto(listaProdutos, *totalProdutos, id)) {
        printf("Erro: ID do produto já existe!\n");
        return;
    }

    novo.id = id;

    // 2 - Descrição
    printf("Informe a descrição do produto: ");
    getchar(); 
    fgets(novo.descricao, 100, stdin);
    novo.descricao[strcspn(novo.descricao, "\n")] = '\0';

    // 3 - Preço
    printf("Informe o preço: ");
    scanf("%lf", &novo.preco);

    // 4 - Estoque
    printf("Informe a quantidade em estoque: ");
    scanf("%d", &novo.estoque);

    // 5 - Inserir no vetor
    listaProdutos[*totalProdutos] = novo;
    (*totalProdutos)++;

    printf("\nProduto cadastrado com sucesso!\n");
}



//caso de uso consultar produto
void consultarProduto(Produto lista[], int tamanho) {
    int id;
    printf("Digite o ID do produto a consultar: ");
    scanf("%d", &id);

    for (int i = 0; i < tamanho; i++) {
        if (lista[i].id == id) {
            printf("\n--- Produto Encontrado ---\n");
            printf("ID: %d\n", lista[i].id);
            printf("Descrição: %s\n", lista[i].descricao);
            printf("Preço: %.2lf\n", lista[i].preco);
            printf("Estoque: %d\n", lista[i].estoque);
            return;
        }
    }

    printf("\nERRO: Produto com ID %d não encontrado.\n", id);






    //caso de uso remover produto
int produtoEmPedido(int produtoId, ItemPedido itens[], int qtdItens) {
    for (int i = 0; i < qtdItens; i++) {
        if (itens[i].produtoId == produtoId) {
            return 1; // Produto está vinculado a pedido
        }
    }
    return 0;
}

void removerProduto(Produto lista[], int *tamanho, 
                    ItemPedido itens[], int qtdItens) {

    int id;
    printf("Digite o ID do produto a remover: ");
    scanf("%d", &id);

    int pos = -1;
    for (int i = 0; i < *tamanho; i++) {
        if (lista[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nERRO: Produto não encontrado.\n");
        return;
    }

    if (produtoEmPedido(id, itens, qtdItens)) {
        printf("\nERRO: Produto não pode ser removido pois existe em pedidos.\n");
        return;
    }

    char confirm;
    printf("Confirmar remoção do produto %d? (S/N): ", id);
    scanf(" %c", &confirm);

    if (confirm == 'S' || confirm == 's') {
        for (int i = pos; i < *tamanho - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*tamanho)--;

        printf("\nProduto removido com sucesso.\n");
    } else {
        printf("\nOperação cancelada.\n");
    }
}





//caso de uso listar produto
void listarProdutos(Produto lista[], int tamanho) {
    if (tamanho == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\n---- LISTA DE PRODUTOS ----\n");

    for (int i = 0; i < tamanho; i++) {
        printf("\nID: %d\n", lista[i].id);
        printf("Descrição: %s\n", lista[i].descricao);
        printf("Preço: %.2lf\n", lista[i].preco);
        printf("Estoque: %d\n", lista[i].estoque);
    }

    printf("\n---------------------------\n");
}