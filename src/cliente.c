#include <stdio.h>
#include <string.h>

#define MAX 100

// Inicializar lista
void inicializarLista(ListaClientes *lista){
    lista->tamanho = 0;
}


// Inserir no final
int inserirFinal(ListaClientes *lista, Cliente c) {
    if (lista->tamanho >= MAX) {
        return 0; // lista cheia
    }

    lista->elementos[lista->tamanho] = c;
    lista->tamanho++;
    return 1;
}

// Buscar cliente por ID
int buscarPorId(ListaClientes *lista, int id) {
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->elementos[i].id == id) {
            return i; // posição encontrada
        }
    }
    return -1; // não encontrado
}

// Remover cliente por posição
int removerPosicao(ListaClientes *lista, int pos) {
    if (pos < 0 || pos >= lista->tamanho) {
        return 0; // posição inválida
    }

    for (int i = pos; i < lista->tamanho - 1; i++) {
        lista->elementos[i] = lista->elementos[i + 1];
    }

    lista->tamanho--;
    return 1;
}


// Remover cliente pelo ID
int removerPorId(ListaClientes *lista, int id) {
    int pos = buscarPorId(lista, id);
    if (pos == -1) return 0;
    return removerPosicao(lista, pos);
}

// Exibir lista (opcional, útil para testes)
void exibirLista(ListaClientes *lista) {
    for (int i = 0; i < lista->tamanho; i++) {
        printf("ID: %d | Nome: %s | Tipo: %s\n",
               lista->elementos[i].id,
               lista->elementos[i].nome,
               lista->elementos[i].tipo);
    }
}