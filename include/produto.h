//validar cnpj
#ifdef VALIDADORES_H
#define VALIDADORES_H

int validarCNPJ(const char *cnpj);

#endif


int analisarProduto(Produto listaProdutos[], int totalProdutos, int idBuscado);
void cadastrarProduto(Produto listaProdutos[], int *totalProdutos);
