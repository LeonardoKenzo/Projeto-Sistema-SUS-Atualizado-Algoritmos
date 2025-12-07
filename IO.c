#include "IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCEDIMENTOS 10
#define MAX_TAMANHO_FILA 30

bool SAVE(AVL *relacao, FILA_PRIORIDADE *fila) {
    if (!relacao || !fila) {
        return false;
    }

    // Array temporário para guardar os ponteiros dos pacientes da fila
    PACIENTE *pacientes_na_fila[MAX_TAMANHO_FILA];
    int prioridade_na_fila[MAX_TAMANHO_FILA];
    int contador_fila = 0;

    FILE *fp_fila = fopen("fila_prioridade.bin", "wb");
    if (!fp_fila) {
        return false;
    }

    //Esvazia a fila, salvando os IDs no arquivo e os pacientes e suas prioridades no array
    int prioridade = fila_get_prioridade(fila);
    PACIENTE *p_fila = fila_remover(fila);
    while (p_fila != NULL) {
        // Salva o ID
        int id = paciente_get_id(p_fila);
        fwrite(&id, sizeof(int), 1, fp_fila);
        fwrite(&prioridade, sizeof(int), 1, fp_fila);

        // Guarda o paciente no array temporário
        pacientes_na_fila[contador_fila] = p_fila;
        
        // Salva a prioridade do paciente no array temporario
        prioridade_na_fila[contador_fila] = prioridade;
        
        prioridade = fila_get_prioridade(fila);
        p_fila = fila_remover(fila);
        contador_fila++;
    }
    fclose(fp_fila);

    //Reconstrói a fila a partir do array temporário para liberar depois
    for (int i = 0; i < contador_fila; i++) {
        fila_inserir(fila, pacientes_na_fila[i], prioridade_na_fila[i]);
    }

    //Salvar a relação de pacientes
    FILE *fp_relacao = fopen("relacao_pacientes.bin", "wb");
    if (!fp_relacao) {
        return false;
    }

    //Salvando a Relação de Pacientes
    avl_salvar_pacientes(relacao, fp_relacao);

    fclose(fp_relacao);

    return true;
}

bool LOAD(AVL **relacao, FILA_PRIORIDADE **fila) {
    if (!(*relacao) || !(*fila)) {
        return false;
    }

    FILE *fp_relacao = fopen("relacao_pacientes.bin", "rb");
    if (!fp_relacao) {
        // Se o arquivo não existe, pode ser a primeira execução. Retorna true com estruturas vazias
        return true; 
    }

    //Carregando a Relação de Pacientes
    int id;
    while (fread(&id, sizeof(int), 1, fp_relacao) == 1) {
        // Ler nome
        int tam_nome;
        fread(&tam_nome, sizeof(int), 1, fp_relacao);
        char *nome = (char *)malloc((tam_nome + 1) * sizeof(char));
        fread(nome, sizeof(char), tam_nome, fp_relacao);
        nome[tam_nome] = '\0';

        PACIENTE *p = paciente_criar(id, nome);
        free(nome);

        // Ler e carregar histórico
        HISTORICO *hist = paciente_get_historico(p);
        int count_procedimentos;
        fread(&count_procedimentos, sizeof(int), 1, fp_relacao);

        for (int i = 0; i < count_procedimentos; i++) {
            int tam_proc;
            fread(&tam_proc, sizeof(int), 1, fp_relacao);
            char *procedimento = (char *)malloc((tam_proc + 1) * sizeof(char));
            fread(procedimento, sizeof(char), tam_proc, fp_relacao);
            procedimento[tam_proc] = '\0';
            
            historico_inserir_procedimento(hist, procedimento);
            free(procedimento);
        }

        avl_inserir_paciente(*relacao, p);
    }
    fclose(fp_relacao);

    //Carregando a fila de atendimento
    FILE *fp_fila = fopen("fila_prioridade.bin", "rb");
    if (!fp_fila) {
        // Se o arquivo não existe, pode ser a primeira execução. Retorna true com estruturas vazias.
        return true;
    }

    int id_fila, prioridade;
    while (fread(&id_fila, sizeof(int), 1, fp_fila) == 1) {
        if (fread(&prioridade, sizeof(int), 1, fp_fila) != 1) {
            fclose(fp_fila);
            return false;
        }

        // Busca o paciente já carregado na relação
        PACIENTE *p_encontrado = avl_registro_busca(*relacao, id_fila);
        if (p_encontrado != NULL) {
            fila_inserir(*fila, p_encontrado, prioridade);
        }
    }
    fclose(fp_fila);

    return true;
}


