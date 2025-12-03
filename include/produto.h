//struct produto
#include <stdio.h>
typedef struct
{
    int id; /* identificador único do cliente */
    int estoque; /* quantidade em estoque */
    char descricao[100]; /* descrição do produto */
    double preco; /* preço unitário */
} Produto;