#ifndef CLIENTE_H
#define CLIENTE_H

#include "globals.h"

// ===========================================
// FUNÇÕES AUXILIARES E VALIDAÇÃO
// ===========================================

// Inicializa a lista geral (Requisito estrutural)
void inicializarLista(ListaClientes *lista);

// Validações (Corrigido: Adicionado 'const' para evitar conflito de tipos)
int validarCPF(const char *cpf); 
int validarCNPJ(const char *cnpj);

// Verifica unicidade de ID em todo o sistema (PF e PJ)
int verificarIDGlobal(int id, PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

// ===========================================
// FUNÇÕES PRINCIPAIS (CRUD)
// ===========================================

// Cadastrar
int cadastrarClientePF(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int totalPJ);
int cadastrarClientePJ(PessoaJuridica listaPJ[], int *totalPJ, PessoaFisica listaPF[], int totalPF);

// Consultar (Interface Visual)
void telaConsultarCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

// Remover (Interface Visual com Confirmação)
void telaRemoverCliente(PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ);

// Listar (Relatório Geral)
void listarTodosClientes(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

#endif // CLIENTE_H