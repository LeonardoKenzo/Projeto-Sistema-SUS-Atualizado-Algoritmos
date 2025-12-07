#include "fila_por_prioridade.h"
#include "paciente.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct no_ NO;
struct no_{
    int prioridade;
    PACIENTE *paciente;
};

struct fila_prioridade_ {
    NO *pessoas;
    int capacidade;
    int quantNos;
};

// FUNCOES AUXILIARES -----------------------------------------------------------------------------

// Realiza a troca entre dois nos
void trocar(NO *a, NO *b) {
    NO temp = *a;
    *a = *b;
    *b = temp;
}

// Rebalanceia e corrige a fila na hora da insercao
void fix_up(FILA_PRIORIDADE *fila, int indice){
    if(indice == 0) return;

    int pai = (indice - 1) / 2;

    // Verifica se o pai do no tem menos prioridade que ele
    if(pai >= 0 && fila->pessoas[pai].prioridade > fila->pessoas[indice].prioridade){
        trocar(&fila->pessoas[indice], &fila->pessoas[pai]);
        fix_up(fila, pai);
    }
}

// Rebalanceia e corrige a fila na hora da remocao
void fix_down(FILA_PRIORIDADE *fila, int indice){
    int maior = indice;
    int filhoEsquerda = indice * 2 + 1;
    int filhoDireita = indice * 2 + 2;

    // Compara se os filhos do no sao maiores que ele mesmo, se sim troca, se nao mantem.
    if(filhoEsquerda <= fila->quantNos && fila->pessoas[filhoEsquerda].prioridade <= fila->pessoas[maior].prioridade)
        maior = filhoEsquerda;

    if(filhoDireita <= fila->quantNos && fila->pessoas[filhoDireita].prioridade <= fila->pessoas[maior].prioridade)
        maior = filhoDireita;

    // Recursao para percorrer toda a heap dessa maneira
    if(maior != indice){
        trocar(&fila->pessoas[indice], &fila->pessoas[maior]);
        fix_down(fila, maior);
    }
}

// FUNCOES DO TAD -----------------------------------------------------------------------------

// Cria uma fila
FILA_PRIORIDADE *fila_criar(int capacidade){
    FILA_PRIORIDADE *fila = (FILA_PRIORIDADE *)calloc(1, sizeof(FILA_PRIORIDADE));
    if(fila == NULL){
        return NULL;
    }

    fila->pessoas = (NO *)calloc(capacidade, sizeof(NO));
    if(fila->pessoas == NULL){
        free(fila);
        return NULL;
    }

    fila->capacidade = capacidade;
    fila->quantNos = 0;

    return fila;
}

// Remove e retorna o paciente com maior prioridade
PACIENTE *fila_remover(FILA_PRIORIDADE *fila){
    if(fila_vazia(fila)) return NULL;

    PACIENTE *removido = fila->pessoas[0].paciente;

    // Diminui a quantidade da Heap
    fila->quantNos--;

    // Troca a raiz com o ultimo no ocupado
    if(fila->quantNos > 0){
        fila->pessoas[0] = fila->pessoas[fila->quantNos];

        // Rebalanceia e corrige a Heap
        fix_down(fila, 0);
    }

    return removido;
}


// Insere um paciente com sua prioridade definida
bool fila_inserir(FILA_PRIORIDADE *fila, PACIENTE *paciente, int prioridade){
   if(fila_cheia(fila) || fila == NULL) return false;

   int indice = fila->quantNos;
   // Adicionar na Heap o paciente com sua respectiva prioridade
   fila->pessoas[fila->quantNos].paciente = paciente;
   fila->pessoas[fila->quantNos].prioridade = prioridade;
   fila->quantNos++;

   // Rebalanceia e corrige a Heap
   fix_up(fila, indice);

   return true;
}

bool fila_cheia(FILA_PRIORIDADE *fila){
    return (fila->capacidade == fila->quantNos);
}

bool fila_vazia(FILA_PRIORIDADE *fila){
    return (fila->quantNos == 0);
}

int fila_get_prioridade(FILA_PRIORIDADE *fila){
    if(fila_vazia(fila)) return -1;

    int prioridade = fila->pessoas[0].prioridade;

    return prioridade;
}

// Printa toda a fila
void fila_printar(FILA_PRIORIDADE *fila) {
    if (fila == NULL || fila->quantNos == 0) {
        printf("Fila vazia!\n");
        return;
    }
    
    // Criar cópia temporária
    FILA_PRIORIDADE *copia = fila_criar(fila->capacidade);
    if (copia == NULL) {
        printf("Erro ao criar cópia!\n");
        return;
    }
    
    // Copiar todos os elementos
    for (int i = 0; i < fila->quantNos; i++) {
        copia->pessoas[i] = fila->pessoas[i];
    }
    copia->quantNos = fila->quantNos;
    
    printf("%-10s | %-20s | %-20s\n", "ID", "NOME", "PRIORIDADE");
    
    // Remover da cópia (não afeta a original)
    while (copia->quantNos > 0) {
        printf("%-10d | %-20s | %-20d\n",
             paciente_get_id(copia->pessoas[0].paciente), 
             paciente_get_nome(copia->pessoas[0].paciente), 
             copia->pessoas[0].prioridade);

        // Remove o primeiro
        fila_remover(copia);
    }
    
    fila_free(&copia);
}

// Libera a fila, mas nao seus pacientes (funcao da relacao de pacientes)
void fila_free(FILA_PRIORIDADE **fila){
    if (*fila == NULL)
        return;
    
    free((*fila)->pessoas);
    free(*fila);
    *fila = NULL;
}
