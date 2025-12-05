#include "fila_de_atendimento.h"
#include "paciente.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// A fila implementada é uma fila encadeada circular
struct fila_de_atendimento_
{
    PACIENTE **pacientes;
    int capacidade;
    int inicio;
    int fim;
    PACIENTE *proximo;
};

FILA_DE_ATENDIMENTO *fila_criar(int capacidade){
    FILA_DE_ATENDIMENTO *fila = (FILA_DE_ATENDIMENTO *)malloc(sizeof(FILA_DE_ATENDIMENTO));
    if (fila == NULL) {
        return NULL;
    }

    fila->pacientes = (PACIENTE **)malloc(capacidade * sizeof(PACIENTE *));
    if (fila->pacientes == NULL) {
        free(fila);
        return NULL;
    }

    //Inicializa as variaveis da fila
    fila->capacidade = capacidade;
    fila->inicio = 0;
    fila->fim = 0;
    fila->proximo = NULL;
    return fila;
}

bool fila_vazia(FILA_DE_ATENDIMENTO *fila) {
    if (fila == NULL) {
        return true;
    }
    return fila->inicio == fila->fim;
}

bool fila_cheia(FILA_DE_ATENDIMENTO *fila) {
    if (fila == NULL) {
        return false;
    }
    return (fila->fim + 1) % fila->capacidade == fila->inicio;
}

bool fila_inserir(FILA_DE_ATENDIMENTO *fila, PACIENTE *paciente) {
    if ((fila == NULL || paciente == NULL)) {
        printf("Não foi possivel inserir o paciente na triagem.\n");
        return false;
    }
    if(fila_cheia(fila)){
        printf("A fila de atendimento está cheia.\n");
        return false;
    }
    fila->pacientes[fila->fim] = paciente;
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->proximo = fila->pacientes[fila->fim];
    return true;
}

//Remove o primeiro paciente que entrou na fila
PACIENTE *fila_remover(FILA_DE_ATENDIMENTO *fila) {
    if (fila != NULL && fila_vazia(fila)) {
        return NULL;
    }
    
    PACIENTE *pacienteRemovido = fila->pacientes[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    return pacienteRemovido;
}

//Libera a fila e os pacientes apontam para NULL (liberam os pacientes)
void fila_free(FILA_DE_ATENDIMENTO **fila) {
    if (fila == NULL || *fila == NULL) {
        return;
    }

    //Tira todos os pacientes da fila
    for (int i = 0; i < ((*fila)->fim) - 1; i++){
        (*fila)->pacientes[i] = NULL;
    }
    free((*fila)->pacientes);
    free(*fila);
    *fila = NULL;
}

void fila_printar(FILA_DE_ATENDIMENTO *fila){
    if(fila == NULL || fila_vazia(fila)){
        printf("Fila de atendimento vazia.\n");
        return;
    }

    //Printa a fila com a posicao do paciente e suas informacoes
    printf("Fila de atendimento:\n");
    int i = fila->inicio;
    int pos=1;
    while(i != fila->fim){
        PACIENTE *paciente = fila->pacientes[i];
        printf("Paciente %d: %s, ID: %d\n", pos, paciente_get_nome(paciente), paciente_get_id(paciente));
        i = (i + 1) % fila->capacidade;
        pos++;
    }
}