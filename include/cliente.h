//struct pessoa física
#include <stdio.h>
typedef struct 
{
    int id; /* identificador único dp cliente */
    char nome[100]; /* nome completo */
    char cpf[15]; /* CPF do indivíduo */
    char endereco[200]; /* endereço completo */
    char telefone[20]; /* telefone do indivíduo */
    char email[100]; /* e-mail do indivíduo */
} PessoaFisica;