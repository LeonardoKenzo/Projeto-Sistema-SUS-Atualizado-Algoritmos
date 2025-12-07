#include "paciente.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct paciente_{
    int id;
    char nome[100];
    HISTORICO *historico;
    bool estaRemovido;
    bool emAtendimento;
};

//Cria um paciente com id, nome e historico proprio
PACIENTE *paciente_criar(int id, char *nome){
    PACIENTE *paciente = (PACIENTE *)calloc(1, sizeof(PACIENTE));
    if(paciente != NULL){

        //Inicializa todas as informacoes do paciente
        paciente->id = id;
        strncpy(paciente->nome, nome, (strlen(nome) + 1));
        paciente->historico = historico_criar();
        paciente->estaRemovido = false;
        paciente->emAtendimento = false;

        return paciente;
    }
    return NULL;
}

//Retorna o historico do paciente
HISTORICO *paciente_get_historico(PACIENTE *paciente){
    return paciente->historico;
}

//Libera a memoria de todas as informacoes do paciente, inclusive o historico
void paciente_free(PACIENTE **paciente){
    if(paciente == NULL || *paciente == NULL){
        return;
    }
    historico_free(&((*paciente)->historico));
    free(*paciente);
    *paciente = NULL;
}

int paciente_get_id(PACIENTE *paciente){
    if(paciente != NULL){
        return paciente->id;
    }
    return -1;
}

int paciente_get_esta_removido(PACIENTE *paciente){
    if(paciente != NULL){
        return paciente->estaRemovido;
    }
    return -1;
}

bool paciente_remover(PACIENTE *paciente){
    if(paciente != NULL && paciente->emAtendimento && paciente->estaRemovido == false){
        paciente->estaRemovido = true;
        paciente->emAtendimento = false;
        printf("Remoção do paciente %s registrado.\n\n", paciente->nome);
        return true;
    }
    else if(paciente != NULL && paciente->estaRemovido){
        printf("O paciente %s já foi removido.\n\n", paciente->nome);
        return false;
    }
    else if(paciente != NULL && paciente->emAtendimento == false){
        printf("Remoção do paciente %s falhou (não pode estar na fila de atendimento).\n\n", paciente->nome);
        return false;
    }
    return false;
}

int paciente_get_atendimento(PACIENTE *paciente){
    if(paciente != NULL){
        return paciente->emAtendimento;
    }
    return -1;
}

void paciente_em_atendimento(PACIENTE *paciente){
    if(paciente != NULL && paciente->emAtendimento == false && paciente->estaRemovido == false){
        paciente->emAtendimento = true;
        printf("Paciente %s em atendimento.\n", paciente->nome);
    }
    else if(paciente != NULL && paciente->emAtendimento){
        printf("O paciente %s já está em atendimento.\n", paciente->nome);
    }
    else if(paciente != NULL && paciente->estaRemovido){
        printf("O paciente %s foi removido dos registros e não pode ser atendido.\n", paciente->nome);
    }
}

char *paciente_get_nome(PACIENTE *paciente){
    if(paciente != NULL){
        return paciente->nome;
    }
    return NULL;
}
