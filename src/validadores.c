#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Validar CPF:
int validarCPF(char cpf[]) {

    char numeros[12];
    int j = 0;

    // remove caracteres que não são números
    for (int i = 0; i < strlen(cpf); i++) {
        if (isdigit(cpf[i])) {
            numeros[j++] = cpf[i];
        }
    }
    numeros[j] = '\0';  // terminador correto!

    // verifica se tem exatamente 11 dígitos
    if (strlen(numeros) != 11)
        return 0;

    // verifica se todos os dígitos são iguais -> inválido
    int iguais = 1;
    for (int i = 1; i < 11; i++) {
        if (numeros[i] != numeros[0]) {
            iguais = 0;
            break;
        }
    }
    if (iguais)
        return 0;

    // cálculo dos dígitos verificadores
    int soma = 0, resto, digito1, digito2;

    // primeiro dígito
    for (int i = 0, peso = 10; i < 9; i++, peso--) {
        soma += (numeros[i] - '0') * peso;
    }
    resto = soma % 11;
    digito1 = (resto < 2) ? 0 : 11 - resto;

    // segundo dígito
    soma = 0;
    for (int i = 0, peso = 11; i < 10; i++, peso--) {
        soma += (numeros[i] - '0') * peso;
    }
    resto = soma % 11;
    digito2 = (resto < 2) ? 0 : 11 - resto;

    // comparação final CORRIGIDA
    if (digito1 == (numeros[9] - '0') &&
        digito2 == (numeros[10] - '0'))
        return 1;  // CPF válido

    return 0;  // CPF inválido
}

//Validar CNPJ:

int validadorCNPJ(char cnpj[]) {
    char numeros[15];
    int j = 0;

    for(int i = 0; i < strlen(cnpj); i++) {
        if(isdigit(cnpj[i])) {
            numeros[j++] = cnpj[i];
        }
    }
    numeros[j] = '\0';

    if(strlen(numeros) != 14)
    return 0;

    int iguais =1;
    for(int i =1; i <14; i++) {
        if(numeros[i] != numeros[0]) {
            iguais = 0;
            break;
        }
    }
    if(iguais)
    return 0;
}