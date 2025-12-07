#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

#define max(a,b) ((a > b) ? a : b)

struct no_avl_{
    PACIENTE* paciente;
    NO* esquerdo;
    NO* direito;
    int altura;
};

struct avl_{
    NO* raiz;
    int profundidade;
};

PACIENTE *avl_registro_busca_aux(NO* raiz, int id);
NO* avl_inserir_paciente_aux(NO* raiz, PACIENTE* paciente);
NO* avl_remover_aux(NO* raiz, int id);
void troca_max_esq(NO* troca, NO* raiz, NO* ant);

// Função para criar um nó da AVL
NO* avl_cria_no(PACIENTE* paciente){
    NO *no = (NO*)malloc(sizeof(NO));
    if (no != NULL){
        no->paciente = paciente;
        no->esquerdo = NULL;
        no->direito = NULL;
        no->altura = 0;
    }
    return no;
}

// Função auxiliar para obter a altura de um nó
int avl_altura_no(NO* no_avl){
    if (no_avl == NULL){
        return -1;
    } else{
        return no_avl->altura;
    }
}

// Função para criar a AVL
AVL* avl_criar(){
    AVL *avl = (AVL*)malloc(sizeof(AVL));
    if (avl != NULL){
        avl->raiz = NULL;
        avl->profundidade = -1;
    }
    return avl;
}

// Função auxiliar para apagar todos os nós da AVL
void avl_apagar_nos(NO* raiz){
    if (raiz != NULL){
        avl_apagar_nos((raiz->esquerdo));
        avl_apagar_nos((raiz->direito));
        paciente_free(&(raiz->paciente));
        free(raiz);
    }
}

// Função para apagar a AVL
void avl_free(AVL** avl){
    if (avl == NULL || *avl == NULL){
        return;
    }
    avl_apagar_nos((*avl)->raiz);
    free(*avl);
    *avl = NULL;
}

// Função para verificar se existe algum nó na AVL
bool avl_esta_vazia(AVL* avl){
    if(avl == NULL || avl->raiz == NULL){
        return true;
    }else{
        return false;
    }
}

// Função auxiliar para imprimir os pacientes
void avl_imprimir_aux(NO* no){
    if(no != NULL){
        avl_imprimir_aux(no->esquerdo);
        PACIENTE *p = no->paciente;
        int id = paciente_get_id(p);
        char *nome = paciente_get_nome(p);
        printf("%-10d | %-30s\n", id, nome);
        avl_imprimir_aux(no->direito);
    }
}

// Função para listar todos os pacientes armazenados na AVL
void avl_listar_pacientes(AVL *avl){
    if(avl == NULL){
        printf("Banco de Dados inválido.\n");
        return;
    }
    if(avl_esta_vazia(avl)){
        printf("Banco de Dados está Vazio!\n");
        return;
    }

    //Cria uma tabela listando todos os pacientes na relacao por ordem de id
    printf("%-10s | %-30s\n", "ID", "NOME");
    avl_imprimir_aux(avl->raiz);
    printf("\n");
    return;
}

// Função para realizar a rotação simples para a direita
NO* rotacao_direita(NO* a){
    if (a == NULL){
        return NULL;
    }
    NO* b = a->esquerdo;
    a->esquerdo = b->direito;
    b->direito = a;

    a->altura = max(avl_altura_no(a->esquerdo), avl_altura_no(a->direito)) + 1;
    b->altura = max(avl_altura_no(b->esquerdo), avl_altura_no(b->direito)) +1;

    return b;
}


// Função para realizar a rotação simples para a esquerda
NO* rotacao_esquerda(NO* a){
    if (a == NULL){
        return NULL;
    }
    NO* b = a->direito;
    a->direito = b->esquerdo;
    b->esquerdo = a;

    a->altura = max(avl_altura_no(a->esquerdo), avl_altura_no(a->direito)) + 1;
    b->altura = max(avl_altura_no(b->esquerdo), avl_altura_no(b->direito)) +1;

    return b;
}

// Função para realizar a rotação dupla Esquerda-Direita
NO* rotacao_esquerda_direita(NO* a){
    if (a == NULL){
        return NULL;
    }
    a->esquerdo = rotacao_esquerda(a->esquerdo);
    return rotacao_direita(a);
}

// Função para realizar a rotação dupla Direita-Esquerda
NO* rotacao_direita_esquerda(NO* a){
    if (a == NULL){
        return NULL;
    }
    a->direito = rotacao_direita(a->direito);
    return rotacao_esquerda(a);
}

// Função principal para buscar um paciente na AVL
PACIENTE *avl_registro_busca(AVL *avl, int id){
    if (avl == NULL){
        printf("Árvore inválida para busca\n");
        return NULL;
    }
    return(avl_registro_busca_aux(avl->raiz, id));
}

// Função auxiliar para buscar um paciente na AVL
PACIENTE *avl_registro_busca_aux(NO* raiz, int id){
    if (raiz == NULL){
        return NULL;
    }
    if (id == paciente_get_id(raiz->paciente)){
        return (raiz->paciente);
    }

    if (id < paciente_get_id(raiz->paciente)){
        return (avl_registro_busca_aux(raiz->esquerdo, id));
    }else{
        return (avl_registro_busca_aux(raiz->direito, id));
    }
}

// Função principal do inserir paciente
bool avl_inserir_paciente(AVL* avl, PACIENTE* paciente){
    if (avl == NULL){
        printf("Árvore inválida para a inserção\n");
        return false;
    }
    int id = paciente_get_id(paciente);
    if (avl_registro_busca(avl, id) != NULL){
        return false;
    }
    
    avl->raiz = avl_inserir_paciente_aux(avl->raiz, paciente);
    return true;
}

// Função auxiliar para inserir novo paciente na AVL
NO* avl_inserir_paciente_aux(NO* raiz, PACIENTE* paciente){
    if(raiz == NULL){
        raiz = avl_cria_no(paciente);
    }
    else if (paciente_get_id(paciente) < paciente_get_id(raiz->paciente)){
        raiz->esquerdo = avl_inserir_paciente_aux(raiz->esquerdo, paciente);
    }
    else if (paciente_get_id(paciente) > paciente_get_id(raiz->paciente)){
        raiz->direito = avl_inserir_paciente_aux(raiz->direito, paciente);
    }

    raiz->altura = max(avl_altura_no(raiz->esquerdo), avl_altura_no(raiz->direito)) + 1;
    int FB = avl_altura_no(raiz->esquerdo) - avl_altura_no(raiz->direito);

    if (FB == -2){
        if ((avl_altura_no(raiz->direito->esquerdo) - avl_altura_no(raiz->direito->direito)) <= 0){
            raiz = rotacao_esquerda(raiz);
        }else{
            raiz = rotacao_direita_esquerda(raiz);
        }        
    }
    if (FB == 2){
        if ((avl_altura_no(raiz->esquerdo->esquerdo) - avl_altura_no(raiz->esquerdo->direito)) >= 0){
            raiz = rotacao_direita(raiz);
        }else{
            raiz = rotacao_esquerda_direita(raiz);
        }
    }
    return raiz;
}

// Função principal para remover paciente da AVL
bool avl_remover_paciente(AVL *avl, int id){
    if (avl == NULL){
        printf("Árvore inválida para remoção\n");
        return false;
    }

    if (avl_registro_busca(avl,id) == NULL){
        return false;
    }
    
    avl->raiz = avl_remover_aux(avl->raiz, id);
    return true;
}

// Função auxiliar da remoção para pegar o maior elemento da árvore esquerda de um nó 
void troca_max_esq(NO* troca, NO* raiz, NO* ant){
    if (troca->direito != NULL){
        troca_max_esq(troca->direito, raiz, troca);
        return;
    }
    if (raiz == ant){
        ant->esquerdo = troca->esquerdo;
    }else{
        ant->direito = troca->esquerdo;
    }

    PACIENTE* paciente = raiz->paciente;
    raiz->paciente = troca->paciente;
    paciente_free(&paciente);
    free(troca);
    troca = NULL;
}

// Função auxiliar para remover um paciente da AVL
NO* avl_remover_aux(NO* raiz, int id){
    NO* p;

    if (raiz == NULL){
        return NULL;
    }
    else if (id == paciente_get_id(raiz->paciente)){
        if (raiz->esquerdo == NULL || raiz->direito == NULL){
            p = raiz;
            if (raiz->esquerdo == NULL){
                raiz = raiz->direito;
            }else{
                raiz = raiz->esquerdo;
            }
            paciente_free(&p->paciente);
            free(p);
            p = NULL;
        }else{
            troca_max_esq(raiz->esquerdo, raiz, raiz);
        }
    }
    else if (id < paciente_get_id(raiz->paciente)){
        raiz->esquerdo = avl_remover_aux(raiz->esquerdo, id);
    }
    else if (id > paciente_get_id(raiz->paciente)){
        raiz->direito = avl_remover_aux(raiz->direito, id);
    }

    if (raiz != NULL){
        raiz->altura = max(avl_altura_no(raiz->esquerdo), avl_altura_no(raiz->direito)) + 1;
        int FB = avl_altura_no(raiz->esquerdo) - avl_altura_no(raiz->direito);

        if (FB == -2){
            if (avl_altura_no(raiz->direito->esquerdo) - avl_altura_no(raiz->direito->direito) <= 0){
                raiz = rotacao_esquerda(raiz);
            }else{
                raiz = rotacao_direita_esquerda(raiz);
            }
        }
        if (FB == 2){
            if (avl_altura_no(raiz->esquerdo) - avl_altura_no(raiz->direito) >= 0){
                raiz = rotacao_direita(raiz);
            }else{
                raiz = rotacao_esquerda_direita(raiz);
            }
        }
    }
    return raiz;
}

// Função auxiliar recursiva para salvar
void avl_salvar_aux(NO *raiz, FILE *fp) {
    if (raiz == NULL) {
        return;
    }
    
    // Percorre esquerda
    avl_salvar_aux(raiz->esquerdo, fp);
    
    // Processa nó atual (salva no arquivo)
    PACIENTE *p = raiz->paciente;
    
    // Salvar ID
    int id = paciente_get_id(p);
    fwrite(&id, sizeof(int), 1, fp);

    // Salvar Nome
    char *nome = paciente_get_nome(p);
    int tam_nome = strlen(nome);
    fwrite(&tam_nome, sizeof(int), 1, fp);
    fwrite(nome, sizeof(char), tam_nome, fp);

    // Salvando o histórico do paciente
    HISTORICO *hist = paciente_get_historico(p);
    char *procedimentos[10];
    int count_procedimentos = 0;

    while (!historico_esta_vazio(hist) && count_procedimentos < 10) {
        char *proc_topo = historico_consultar_procedimento_topo(hist); 
        procedimentos[count_procedimentos] = strdup(proc_topo);
        historico_remover_procedimento(hist);
        count_procedimentos++;
    }
    
    fwrite(&count_procedimentos, sizeof(int), 1, fp);

    for (int i = count_procedimentos - 1; i >= 0; i--) {
        int tam_proc = strlen(procedimentos[i]);
        fwrite(&tam_proc, sizeof(int), 1, fp);
        fwrite(procedimentos[i], sizeof(char), tam_proc, fp);
        free(procedimentos[i]);
    }
    
    // Percorre direita
    avl_salvar_aux(raiz->direito, fp);
}

// Função principal
void avl_salvar_pacientes(AVL *avl, FILE *fp) {
    if (avl == NULL || fp == NULL) {
        return;
    }
    avl_salvar_aux(avl->raiz, fp);
}