#ifndef FILA_DE_ATENDIMENTO_H
    #define FILA_DE_ATENDIMENTO_H
    #include "paciente.h"

    typedef struct fila_de_atendimento_ FILA_DE_ATENDIMENTO;

    //Precisa implementar uma funcao que printe a fila de atendimento
    FILA_DE_ATENDIMENTO *fila_criar(int capacidade);
    PACIENTE *fila_remover(FILA_DE_ATENDIMENTO *fila);
    bool fila_inserir(FILA_DE_ATENDIMENTO *fila, PACIENTE *paciente);
    bool fila_cheia(FILA_DE_ATENDIMENTO *fila);
    bool fila_vazia(FILA_DE_ATENDIMENTO *fila);
    void fila_free(FILA_DE_ATENDIMENTO **fila);
    void fila_printar(FILA_DE_ATENDIMENTO *fila);

#endif