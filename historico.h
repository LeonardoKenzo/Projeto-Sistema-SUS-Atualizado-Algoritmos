#ifndef HISTORICO_H
    #define HISTORICO_H
    #include <stdbool.h>

    typedef struct historico_ HISTORICO;
    HISTORICO *historico_criar();
    bool historico_remover_procedimento(HISTORICO *historico);
    bool historico_inserir_procedimento(HISTORICO *historico, char *procedimento);
    int historico_get_quantidade_caracteres(HISTORICO *historico);
    void historico_printar(HISTORICO *historico);
    void historico_free(HISTORICO **historico);
    bool historico_esta_vazio(HISTORICO *historico);
    char *historico_consultar_procedimento_topo(HISTORICO *historico);

#endif