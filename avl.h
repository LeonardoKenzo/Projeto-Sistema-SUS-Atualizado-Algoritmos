#ifndef AVL_H
    #define AVL_H
    #include <stdbool.h>
    #include "paciente.h"
    #include "historico.h"

    typedef struct avl_ AVL;
    typedef struct no_avl_ NO;

    AVL *avl_criar();
    PACIENTE *avl_registro_busca(AVL *avl, int id);
    bool avl_esta_vazia(AVL *avl);
    bool avl_remover_paciente(AVL *avl, int id);
    bool avl_inserir_paciente(AVL* avl, PACIENTE* paciente);
    void avl_listar_pacientes(AVL *avl);
    void avl_free(AVL **avl);

#endif