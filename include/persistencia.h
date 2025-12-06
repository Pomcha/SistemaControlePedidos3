

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "globals.h" 

void carregarPedidosCSV(Pedido listaPedidos[], int *totalPedidos, itemPedido listaItensPedidos[], int *totalItensPedidos);
void salvarPedidosCSV(Pedido listaPedidos[], int totalPedidos, itemPedido listaItensPedidos[], int totalItensPedidos);
void persistirProdutos(Produto listaProdutos[], int totalProdutos);
void carregarClientesCSV(Cliente listaClientes[], int *totalClientes, PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ);
void persistirCliente(PessoaFisica listaPF[], int totalPF, PessoaJuridica listaPJ[], int totalPJ);

#endif 