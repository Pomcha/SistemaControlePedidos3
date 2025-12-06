#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_REGISTROS 100 
#define MAX_ITENS 500
#define MAX 100

// ===========================================
// ESTRUTURAS DE CLIENTE
// ===========================================


// Cliente (Geral - Base de dados)
typedef struct {
    int id;
    char tipo[2]; 
    char nome[100];
    char endereco[200];
    char telefone[20];
    char email[100];
} Cliente;

// PessoaFisica (Usa Cliente como base)
typedef struct {
    Cliente dados;
    char cpf[20];
} PessoaFisica;

// PessoaJuridica (Usa Cliente como base)
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
    char descricao[100]; // << NOVO: Necessário para produto.c e persistencia.c
    float preco;
    int estoque;          // << NOVO: Necessário para produto.c e persistencia.c
} Produto;

typedef struct {
    int pedidoId;        // Mudei para 'pedidoId' para consistência (era 'id_produto')
    int produtoId;       // Mudei para 'produtoId' para consistência
    int quantidade;
    double subtotal;
} itemPedido; // Recomendo manter 'subtotal' no itemPedido, ou ajustar 'pedido.c'

typedef struct {
    int id;             // Era 'id_pedido'
    int clienteid;      // Era 'id_cliente'
    char data[20];     // << NOVO: Necessário para pedido.c
    double total;      // << NOVO: Necessário para pedido.c
    // outros campos
} Pedido;

#endif // GLOBALS_H