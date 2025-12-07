# Projeto w – Pronto Socorro SUS Atualizado
## 📌 Introdução

O projeto consiste em simular um sistema de atendimento médico de emergência, seguindo a ordem de prioridade e chegada dos pacientes e registrando seu histórico de procedimentos. Utiliza Tipos Abstratos de Dados (TADs) com AVLs, Heaps e pilhas para gerenciar dados, fila de espera por prioridade e persistência de dados para armazenar as informações dos pacientes.

## 🎯 Objetivo

**Implementar um sistema que:**

- Cadastre pacientes (com ID único e nome).
- Gerencie a fila de espera com prioridade, considerando a ordem de chegada.
- Mantenha o histórico médico do paciente com possibilidade de desfazer o último procedimento.
- Registre a remoção do paciente do sistema do PS.
- Persista os dados em disco, carregando-os ao iniciar o sistema e salvando-os ao encerrar.

## 🧍 Estrutura do Paciente

|**Campo**|**Descrição**                                    |
|---------|-------------------------------------------------|
|Id       |Identificador único do paciente                  |
|Nome     |Nome completo do paciente                        |
|Histórico|Até 10 procedimentos, texto ≤ 100 caracteres cada|

## 💻 Interface do Sistema

**Menu do Sistema:**
1. Registrar paciente.
2. Remover paciente.
3. Adicionar procedimento ao histórico médico.
4. Desfazer procedimento do histórico médico.
5. Dar alta ao paciente.
6. Mostrar fila de espera.
7. Mostrar histórico do paciente.
8. Listar pacientes.
9. Sair.

## 📚 Estruturas de Dados Utilizadas

O sistema foi desenvolvido com TADs (Tipos Abstratos de Dados) específicos para cada funcionalidade principal:

### 🔹 Histórico Médico → Pilha

Implementado como um array fixo de até 10 procedimentos por paciente

- **Justificativa:** a pilha (LIFO) permite desfazer o último procedimento de forma eficiente.

- **Eficiência:** inserção e remoção em O(1).

- **Adequação:** reflete bem a ideia de "desfazer" ações médicas recentes.

### 🔹 Fila de Atendimento por Prioridade → Heap Sequencial
Estrutura baseada em uma Heap de Máximo (max-heap) sequencial, que ordena os pacientes pela prioridade de atendimento (classificação de risco), e em caso de empate, pela ordem de chegada.

- **Justificativa:**  Garante que o paciente com a maior prioridade (maior risco/urgência) seja sempre o primeiro a ser atendido, modelando a dinâmica real de triagem e atendimento emergencial.
  
- **Eficiência:**
  - Inserção (Chegada do Paciente): O(log n) devido à necessidade de manter a propriedade da heap (subida/heapify-up).
  - Remoção (Chamada para Atendimento): O(log n) devido à remoção da raiz e posterior reorganização da heap (descida/heapify-down).
    
- **Adequação:** Ideal para a dinâmica de pronto-socorro, pois a operação de encontrar e remover o elemento de maior prioridade é muito rápida.

### 🔹 Relação de Pacientes → AVL

Estrutura de árvore de busca binária balanceada, onde cada nó armazena os dados do paciente, e a ordenação é feita pelo ID do paciente.

- **Justificativa:** Mantém um registro organizado de pacientes, permitindo operações rápidas de busca, inserção e exclusão, enquanto garante que a árvore permaneça balanceada.

- **Eficiência:**
  - Busca: O(log n) (Devido ao balanceamento da árvore, a altura é sempre logarítmica).
  - Inserção/Remoção: O(log n) (Devido ao balanceamento da árvore, as operações são rápidas, podendo envolver rotações O(1) para manter o equilíbrio).

- **Adequação:** Perfeita para sistemas de cadastro persistente, onde a consulta rápida (por ID) e a manutenção eficiente dos registros são cruciais, evitando o desempenho de pior caso O(n) de uma lista encadeada desbalanceada.

## 💾 Persistência dos Dados

- **Ao iniciar** → Carregar lista completa de pacientes, fila de espera com suas prioridades e históricos salvos.
- **Ao encerrar** → Salvar todos os dados em disco.
  
Implementado com:
- Um TAD de I/O centralizado que cria e lê arquivos binários para salvar os dados

## 👨‍🎓 Alunos Responsáveis
Leonardo Kenzo Tanaka [Github: [LeonardoKenzo](https://github.com/LeonardoKenzo)]

Pedro Teidi de Sa Yamacita [Github: [pedroYamacita](https://github.com/pedroYamacita)]

Gustavo de Faria Fernandes [Github: [Gustavo-Fernandes04](https://github.com/Gustavo-Fernandes04)]
