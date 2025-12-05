#ifndef IO_H
	#define IO_H
	#include "paciente.h"
    #include "historico.h"
    #include "fila_por_prioridade.h"
    #include "relacao_de_pacientes.h"
    #include <stdbool.h>

    bool SAVE(RELACAO_DE_PACIENTE *relacao, FILA_PRIORIDADE *fila); // Salva os itens da lista e da fila em arquivos
    bool LOAD(RELACAO_DE_PACIENTE **relacao, FILA_PRIORIDADE **fila); // Carrega os itens dos arquivos na lista e na fila
	  
#endif
