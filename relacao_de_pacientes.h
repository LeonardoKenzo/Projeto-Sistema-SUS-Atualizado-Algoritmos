#ifndef RELACAO_DE_PACIENTES_H
    #define RELACAO_DE_PACIENTES_H
    #include <stdbool.h>
    #include "paciente.h"
    #include "historico.h"

    typedef struct relacao_de_pacientes_ RELACAO_DE_PACIENTE;

    RELACAO_DE_PACIENTE *relacao_criar();
    PACIENTE *relacao_registro_busca(RELACAO_DE_PACIENTE *relacao, int id);
    PACIENTE *relacao_remover_paciente_fim(RELACAO_DE_PACIENTE *relacao);
    bool relacao_esta_vazia(RELACAO_DE_PACIENTE *relacao);
    bool relacao_apagar_paciente(RELACAO_DE_PACIENTE *relacao, int id);
    bool relacao_inserir_paciente(RELACAO_DE_PACIENTE *relacao, PACIENTE *paciente);
    void relacao_listar_pacientes(RELACAO_DE_PACIENTE *relacao);
    void relacao_free(RELACAO_DE_PACIENTE **relacao);


#endif