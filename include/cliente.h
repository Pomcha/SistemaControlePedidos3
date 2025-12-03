//  lista de clientes. funcoes estao no .c

    #ifndef LISTA_CLIENTES
    #define LISTA_CLIENTES 

    #define MAX 100
    
    typedef struct{
        Cliente elementos[MAX];   
        int tamanho;             
    }ListaClientes;
    
#endif