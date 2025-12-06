#define ENTER 10

#ifndef INTERFACE_H
#define INTERFACE_H

#include "globals.h" 

void mostrarMenuClientes(Cliente listaClientes[], int *totalClientes, PessoaFisica listaPF[], int *totalPF, PessoaJuridica listaPJ[], int *totalPJ);

void mostrarMenuProdutos(Produto listaProdutos[], int *totalProdutos, itemPedido itensPedidos[], int *totalItensPedidos);
void mostrarMenuPedidos(Pedido listaPedidos[], int *totalPedidos, itemPedido listaItensPedidos[], int *totalItensPedidos);

#endif // INTERFACE_H