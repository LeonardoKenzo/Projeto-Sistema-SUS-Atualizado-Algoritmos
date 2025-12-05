#include "historico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define QUANTPROCED 10
#define QUANTCARACTER 100   

//Funciona como uma Pilha de procedimentos medicos
struct historico_{
    char procedimentos[QUANTPROCED][QUANTCARACTER];
    int tamanho;
    int quantidadeCaracteres;
};

//Verifica se o historico esta cheio
bool historico_esta_cheio(HISTORICO *historico){
    if((historico != NULL && historico->tamanho == 10) || (historico != NULL && historico->quantidadeCaracteres >= 1000)){
        return true;
    }
    return false;
}

//Verifica se o historico esta vazio
bool historico_esta_vazio(HISTORICO *historico){
    if(historico != NULL && historico->tamanho == 0){
        return true;
    }
    return false;
}

//Cria e inicializa um historico novo
HISTORICO *historico_criar(){
    HISTORICO *historico = (HISTORICO *)calloc(1, sizeof(HISTORICO));
    if(historico != NULL){
        historico->quantidadeCaracteres = 0;
        historico->tamanho = 0; 
        return historico;
    }
    return NULL;
}

//Libera a memoria do historico
void historico_free(HISTORICO **historico){
    if(historico == NULL || *historico == NULL){
        return;
    }

    //Libera a memoria do historico
    free(*historico);
    *historico = NULL;
}

//Printa o historico medico inteiro
void historico_printar(HISTORICO *historico){
    if(historico != NULL && !historico_esta_vazio(historico)){
        for(int i = 0; i < historico->tamanho; i++){
            printf("%d. %s\n", i + 1, historico->procedimentos[i]);
        }
    }
    else if(historico_esta_vazio(historico)){
        printf("O histórico está vazio.\n");
    }
}

//Insere um procedimento ao historico
bool historico_inserir_procedimento(HISTORICO *historico, char *procedimento){
    if(!historico_esta_cheio(historico) && strlen(procedimento) <= 100){
        historico->quantidadeCaracteres += strlen(procedimento);

        //Insere o procedimento
        strncpy(historico->procedimentos[historico->tamanho], procedimento, strlen(procedimento) + 1);
        historico->tamanho++;
        return true;
    }
    //Se nao conseguir inserir
    printf("O histórico médico está cheio.\n");
    return false;
}

//Remove o ultimo procedimento adicionado do historico
bool historico_remover_procedimento(HISTORICO *historico){
    if(!historico_esta_vazio(historico)){

        //Remove o procedimento do historico
        historico->tamanho--;
        return true;
    }
    //Se nao conseguir remover
    printf("Não há procedimentos para remover.\n");
    return false;
}

int historico_get_quantidade_caracteres(HISTORICO *historico){
    return historico->quantidadeCaracteres;
}

char *historico_consultar_procedimento_topo(HISTORICO *historico){
    if (historico == NULL || historico_esta_vazio(historico)){
        return NULL;
    }

     return historico->procedimentos[historico->tamanho - 1];
}