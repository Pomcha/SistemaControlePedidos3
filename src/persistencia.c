// função persistir produto
#include <stdio.h>
#include "produto.h"

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