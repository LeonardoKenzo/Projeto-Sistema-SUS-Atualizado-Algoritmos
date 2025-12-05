#ifndef FILA_POR_PRIORIDADE
    #define FILA_POR_PRIORIDADE
    #include "paciente.h"

    typedef struct fila_prioridade_ FILA_PRIORIDADE;

    FILA_PRIORIDADE *fila_criar();
    PACIENTE *fila_remover(FILA_PRIORIDADE *fila);
    bool fila_inserir(FILA_PRIORIDADE *fila, PACIENTE *paciente, int prioridade);
    bool fila_cheia(FILA_PRIORIDADE *fila);
    bool fila_vazia(FILA_PRIORIDADE *fila);
    void fila_printar(FILA_PRIORIDADE *fila);
    void fila_free(FILA_PRIORIDADE **fila);

#endif