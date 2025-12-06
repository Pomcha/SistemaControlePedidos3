#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_REGISTROS 100 
#define MAX_ITENS 500
#define MAX 100

// ===========================================
// ESTRUTURAS DE CLIENTE
// ===========================================


// Cliente
typedef struct {
    int id;
    char tipo[2]; 
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];
} Cliente;

// PessoaFisica
typedef struct {
    Cliente dados;
    char cpf[20];
} PessoaFisica;

// PessoaJuridica 
typedef struct {
    Cliente dados;
    char cnpj[20];
    char nomeContato[100];
} PessoaJuridica;

// Lista de clientes
typedef struct{
    Cliente elementos[MAX];
    int tamanho;
}ListaClientes;

// ===========================================
// ESTRUTURAS DE OUTROS MÓDULOS (Placeholder)
// ===========================================

typedef struct {
    int id;
    char descricao[100]; 
    float preco;
    int estoque;          
} Produto;

typedef struct {
    int pedidoId;        
    int produtoId;       
    int quantidade;
    double subtotal;
} itemPedido;

typedef struct {
    int id;            
    int clienteid;     
    char data[20];    
    double total;      
   
} Pedido;

#endif 