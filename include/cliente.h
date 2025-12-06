#ifndef CLIENTE_H
#define CLIENTE_H

#include "globals.h"

// ===========================================
// FUNÇÕES AUXILIARES E VALIDAÇÃO
// ===========================================

// Inicializa a lista geral (Requisito estrutural)
void inicializarLista(ListaClientes *lista);

int validarCPF(const char *cpf); 
int validarCNPJ(const char *cnpj);

int verificarIDGlobal(int id, PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

// ===========================================
// FUNÇÕES PRINCIPAIS 
// ===========================================

// Cadastrar
int cadastrarClientePF(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int totalPJ);
int cadastrarClientePJ(PessoaJuridica listaPJ[], int *totalPJ, PessoaFisica listaPF[], int totalPF);

// Consultar 
void telaConsultarCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

// Remover 
void telaRemoverCliente(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ);

// Listar 
void listarTodosClientes(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

#endif 