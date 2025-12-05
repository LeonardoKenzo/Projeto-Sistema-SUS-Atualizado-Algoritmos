#include <stdio.h>
#include <stdlib.h>
#include "relacao_de_pacientes.h"

#define CAPACIDADE_INICIAL 10

struct relacao_de_pacientes_{
    PACIENTE **pacientes;
    int quantidade;
    int capacidade;
};

//Funcao auxiliar recursiva para realizar a busca binaria de um paciente na relacao
int busca_binaria_indice(RELACAO_DE_PACIENTE *relacao, int id, int inicio, int fim){
    if(inicio > fim){
        return -1;
    }

    int meio = inicio + (fim - inicio) / 2;
    int idMeio = paciente_get_id(relacao->pacientes[meio]);

    if(idMeio == id){
        return meio;
    }
    else if(idMeio > id){
        return busca_binaria_indice(relacao, id, inicio, meio-1);
    }
    else{
        return busca_binaria_indice(relacao, id, meio+1, fim);
    }
}

//Funcao auxiliar para inserir paciente na relacao
int busca_binaria_insercao(RELACAO_DE_PACIENTE *relacao, int id){
    int inicio = 0;
    int fim = (relacao->quantidade)-1;

    while(inicio <= fim){
        int meio = inicio + (fim - inicio)/2;
        int id_meio = paciente_get_id(relacao->pacientes[meio]);

        if (id_meio < id){
            inicio = meio + 1;
        }else{
            fim = meio - 1;
        }
    }
    return inicio;
}

RELACAO_DE_PACIENTE *relacao_criar(){
    RELACAO_DE_PACIENTE *relacao = (RELACAO_DE_PACIENTE *)calloc(1, sizeof(RELACAO_DE_PACIENTE));
    if(relacao == NULL) {
        return NULL;
    }
    relacao->pacientes = (PACIENTE **)calloc(CAPACIDADE_INICIAL, sizeof(PACIENTE*));
    if(relacao->pacientes == NULL) {
        free(relacao);
        return NULL;
    }
    relacao->capacidade = CAPACIDADE_INICIAL;
    relacao->quantidade = 0;
    return relacao;
}

//Libera a relacao e todos os pacientes nela (Responsavel por liberar os pacientes)
void relacao_free(RELACAO_DE_PACIENTE **relacao){
    if (relacao == NULL || *relacao == NULL){
        return;
    }
    for(int i = 0; i < (*relacao)->quantidade; i++){
        paciente_free(&((*relacao)->pacientes[i]));
    }
    free((*relacao)->pacientes);
    free(*relacao);
    *relacao = NULL;
    return;
}

bool relacao_esta_vazia(RELACAO_DE_PACIENTE *relacao){
    if(relacao == NULL){
        return true;
    }else{
        return relacao->quantidade == 0;
    }
}

void relacao_listar_pacientes(RELACAO_DE_PACIENTE *relacao){
    if(relacao == NULL){
        printf("Relação inválida.\n");
        return;
    }
    if(relacao_esta_vazia(relacao)){
        printf("Relação está Vazia!\n");
        return;
    }

    //Cria uma tabela listando todos os pacientes na relacao por ordem de id
    printf("%-10s | %-30s\n", "ID", "NOME");
    for(int i = 0; i < relacao->quantidade; i++){        
        PACIENTE *p = relacao->pacientes[i];
        int id = paciente_get_id(p);
        char *nome = paciente_get_nome(p);

        printf("%-10d | %-30s\n", id, nome);
    }
    printf("Total de pacientes: %d\n\n", relacao->quantidade);
    return;
}

//Busca um paciente pela id dentro da relacao
PACIENTE *relacao_registro_busca(RELACAO_DE_PACIENTE *relacao, int id){
    if(relacao == NULL){
        printf("Relação inválida.\n");
        return NULL;
    }
    if(relacao_esta_vazia(relacao)){
        return NULL;
    }
    //Busca o id procurado e retorna o paciente
    int aux = busca_binaria_indice(relacao, id, 0, (relacao->quantidade)-1);
    if(aux == -1){
        return NULL;
    }else{
        return relacao->pacientes[aux];
    }
}

bool relacao_inserir_paciente(RELACAO_DE_PACIENTE *relacao, PACIENTE *paciente){
    if(relacao == NULL || paciente == NULL){
        printf("Não foi possivel registrar o paciente.\n");
        return false;
    }

    //Aumenta a capacidade da relacao se estiver cheio
    if(relacao->capacidade == relacao->quantidade){
        PACIENTE **aux = realloc(relacao->pacientes, ((relacao->capacidade) * 2) * sizeof(PACIENTE*));
        if(aux == NULL){
            printf("Limite de memória máximo atingido.\n");
            return false;
        }else{
            relacao->pacientes = aux;
            relacao->capacidade = ((relacao->capacidade) * 2);
        }
    }
    
    //Mantem ordenado
    int id_atual = paciente_get_id(paciente);
    int indice_insercao = busca_binaria_insercao(relacao, id_atual);
    for(int i = relacao->quantidade; i > indice_insercao; i--){
        relacao->pacientes[i] = relacao->pacientes[i-1];
    }

    //Adiciona o paciente na relacao
    relacao->pacientes[indice_insercao] = paciente;
    relacao->quantidade += 1;

    return true;
}
    
bool relacao_apagar_paciente(RELACAO_DE_PACIENTE *relacao, int id){
    if(relacao == NULL){
        printf("Relação inválida.\n");
        return false;
    }

    int indice = busca_binaria_indice(relacao, id, 0, (relacao->quantidade)-1);
    if(indice == -1){
        printf("Paciente inexistente.\n");
        return false;
    }
    PACIENTE* pacienteRemover = relacao->pacientes[indice];
    paciente_free(&pacienteRemover);

    for(int i = indice; i < (relacao->quantidade)- 1; i++){
        relacao->pacientes[i] = relacao->pacientes[i + 1];
    }

    relacao->quantidade -= 1;
    return true;
}

PACIENTE *relacao_remover_paciente_fim(RELACAO_DE_PACIENTE *relacao){
    if (relacao == NULL || relacao_esta_vazia(relacao)){
        return NULL;
    }
    relacao->quantidade -= 1;
    
    return relacao->pacientes[relacao->quantidade];   
}