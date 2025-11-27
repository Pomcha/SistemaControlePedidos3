//struct_cliente:
#include <stdio.h>
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