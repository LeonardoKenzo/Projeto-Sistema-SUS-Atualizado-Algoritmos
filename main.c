#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "paciente.h"
#include "fila_de_atendimento.h"
#include "relacao_de_pacientes.h"
#include "IO.h"
#define TAM_FILA 26

PACIENTE *ler_paciente(RELACAO_DE_PACIENTE *relacao);
PACIENTE *buscar_paciente(RELACAO_DE_PACIENTE *relacao);
bool ler_verificar_numero(int *saida);
void registrar_paciente(RELACAO_DE_PACIENTE *relacao, FILA_DE_ATENDIMENTO *fila, PACIENTE *novoPaciente);
void registrar_obito(RELACAO_DE_PACIENTE *relacao);
void adicionar_procedimento(RELACAO_DE_PACIENTE *relacao);
void desfazer_procedimento(RELACAO_DE_PACIENTE *relacao);
void chamar_paciente_atendimento(FILA_DE_ATENDIMENTO *fila);
void mostrar_historico(RELACAO_DE_PACIENTE *relacao);
void printar_menu();

int main(void){
    RELACAO_DE_PACIENTE *relacao = relacao_criar();
    FILA_DE_ATENDIMENTO *fila = fila_criar(TAM_FILA);

    //CARREGAR RELAÇÃO E FILA DE EXECUÇÕES ANTERIORES
    if (LOAD(&relacao, &fila)== false){
        printf("Não foi possível acessar o arquivo binário para LOAD\n");
    }

    int comando;
    do{
        //Garante que o comando seja selecionado
        do{
            printar_menu();
        }while(!ler_verificar_numero(&comando));

        //Garante que o comando esteja entre 1 e 8
        if(comando < 1 || comando > 8)
            printf("Selecione um comando válido.\n\n");
        
        switch (comando)
        {
        case 1: //Registro de Pacientes
            PACIENTE *novoPaciente = ler_paciente(relacao);
            registrar_paciente(relacao, fila, novoPaciente);
            break;
        case 2: //Registrar obito do Paciente
            registrar_obito(relacao);
            break;
        case 3: //Adicionar procedimento ao historico medico
            adicionar_procedimento(relacao);
            break;
        case 4: //Desfazer procedimento do historico medico
            desfazer_procedimento(relacao);
            break;
        case 5: //Chamar paciente para atendimento
            chamar_paciente_atendimento(fila);
            break;
        case 6: //Mostrar fila de espera
            fila_printar(fila);
            printf("\n");
            break;
        case 7: //Mostrar historico do paciente
            mostrar_historico(relacao);
            break;
        }
    }while(comando != 8);

    //SALVAR RELAÇÂO E FILA
    if(SAVE(relacao, fila)==false){
        printf("Não foi possível acessar o arquivo binário para o SAVE\n");
    }

    //LIBERAR MEMORIA
    fila_free(&fila);
    relacao_free(&relacao);

    return 0;
}

void printar_menu(){
    printf("Selecione o Comando:\n");
    printf("1. Registrar paciente.\n");
    printf("2. Registrar óbito de paciente.\n");
    printf("3. Adicionar procedimento ao histórico médico.\n");
    printf("4. Desfazer procedimento do histórico médico.\n");
    printf("5. Chamar paciente para atendimento.\n");
    printf("6. Mostrar fila de espera.\n");
    printf("7. Mostrar histórico do paciente.\n");
    printf("8. Sair.\n");
}

PACIENTE *ler_paciente(RELACAO_DE_PACIENTE *relacao){
    int id;
    char nome[100];
    //Le o nome e id do paciente
    printf("\nDigite o nome do paciente: ");
    scanf(" %[^\n]", nome);
    getchar();

    //Garante a leitura correta do id
    do{
        printf("Digite o id a ser registrado: ");
    }while(!ler_verificar_numero(&id));

    //Verifica se o id ja existe
    while(paciente_get_id(relacao_registro_busca(relacao, id)) != -1){
        printf("Esse id já existe, escolha outro: ");
        scanf(" %d", &id);
    }

    //Retorna o paciente dado
    PACIENTE *novoPaciente = paciente_criar(id, nome);
    return novoPaciente;
}

void registrar_paciente(RELACAO_DE_PACIENTE *relacao, FILA_DE_ATENDIMENTO *fila, PACIENTE *novoPaciente){
    //Se nao foi possivel inserir o paciente no registro libera memoria
    if(!relacao_inserir_paciente(relacao, novoPaciente)){
        paciente_free(&novoPaciente);
        return;
    }
    if(!fila_inserir(fila, novoPaciente)){
        relacao_apagar_paciente(relacao, paciente_get_id(novoPaciente));
        return;
    }
    printf("Paciente registrado e dentro da fila de espera!\nNome: %s\nId: %d\n\n", paciente_get_nome(novoPaciente), paciente_get_id(novoPaciente));
}

void registrar_obito(RELACAO_DE_PACIENTE *relacao){
    PACIENTE *paciente = buscar_paciente(relacao);

    //Registra obito do paciente
    if(paciente_registrar_obito(paciente)){
        relacao_apagar_paciente(relacao, paciente_get_id(paciente));
    }
}

void adicionar_procedimento(RELACAO_DE_PACIENTE *relacao){
    PACIENTE *paciente = buscar_paciente(relacao);
    
    //Insere o procedimento
    char procedimento[100];
    printf("Escreva o procedimento abaixo:\n");
    scanf(" %[^\n]", procedimento);
    if(historico_inserir_procedimento(paciente_get_historico(paciente), procedimento)){
        printf("Procedimento inserido!\n");
    }    
    printf("\n");
}

void desfazer_procedimento(RELACAO_DE_PACIENTE *relacao){
    PACIENTE *paciente = buscar_paciente(relacao);

    //Desfaz o procedimento
    printf("Paciente: %s\n", paciente_get_nome(paciente));
    char * procedimento = historico_consultar_procedimento_topo(paciente_get_historico(paciente));
    if(procedimento == NULL){
        return;
    }
    historico_remover_procedimento(paciente_get_historico(paciente));
    
    printf("\"%s\" foi removido do histórico médico.\n", procedimento);
    printf("\n");
}

void chamar_paciente_atendimento(FILA_DE_ATENDIMENTO *fila){
    //Retira o paciente da fila de atendimento
    PACIENTE *paciente = fila_remover(fila);
    if(paciente == NULL){
        printf("Não tem pacientes para serem atendidos.\n");
        return;
    }
    paciente_em_atendimento(paciente);
    printf("\n");
}

void mostrar_historico(RELACAO_DE_PACIENTE *relacao){
    PACIENTE *paciente = buscar_paciente(relacao);
    printf("Paciente: %s\n", paciente_get_nome(paciente));
    historico_printar(paciente_get_historico(paciente));
    printf("\n");
}

//Funcao auxiliar que busca um paciente dentro do registro
PACIENTE *buscar_paciente(RELACAO_DE_PACIENTE *relacao){
    int id;
    PACIENTE *paciente = NULL;
    
    //Garante que o id existe
    do{
        printf("\nDigite o id do paciente: ");
        if(ler_verificar_numero(&id)){
            paciente = relacao_registro_busca(relacao, id);
        }
        else
            continue;
        if(paciente == NULL){
            printf("Paciente não registrado, digite um id válido.\n");
        }
    }while(paciente == NULL);

    return paciente;
}

//Funcao auxiliar que garante a leitura correta do id
bool ler_verificar_numero(int *saida){
    char entrada[100];
    char *fim;

    //Le o numero de entrada
    scanf(" %[^\n]", entrada);

    entrada[strcspn(entrada, "\n")] = '\0';
    if(entrada[0] == '-') {
        printf("Formato inválido.\n\n");
        return false;
    }

    //Verifica se todos os caracteres são dígitos
    for(int i = 0; entrada[i] != '\0'; i++) {
        if(!isdigit((unsigned char)entrada[i])) {
            printf("Formato inválido.\n\n");
            return false;
        }
    }

    //Converte para número
    *saida = strtol(entrada, &fim, 10);

    //Confere se converteu e não sobrou lixo
    if(fim == entrada || *fim != '\0') {
        printf("Formato inválido.\n\n");
        return false;
    }

    return true;
}
