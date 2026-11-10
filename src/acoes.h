/******************************************************************************
 *  SISPE - Modulo de acoes
 *  Estrutura de dados da acao do PSE e funcoes de cada opcao do menu.
 ******************************************************************************/
#ifndef ACOES_H
#define ACOES_H

#define MAX_ACOES 50            // limite de acoes guardadas no vetor
#define TAM_TEXTO 50            // tamanho dos campos de texto (49 letras + '\0')
#define TAM_DATA 11             // DD/MM/AAAA + '\0'
#define MAX_PARTICIPANTES 5000  // limite de participantes por acao

// codigos da situacao da acao
#define PLANEJADA 1
#define REALIZADA 2
#define CANCELADA 3

// Estrutura de uma acao coletiva do PSE (nenhum campo identifica estudante)
typedef struct {
    int codigo;
    char escola[TAM_TEXTO];
    char subtema[TAM_TEXTO];
    char dataPrevista[TAM_DATA];    // formato DD/MM/AAAA
    char publicoAlvo[TAM_TEXTO];    // ex.: 6o ano, ensino medio
    char responsavel[TAM_TEXTO];    // profissional que conduz a acao
    int quantidadePrevista;
    int situacao;                   // 1 planejada, 2 realizada, 3 cancelada
    int quantidadeRealizada;        // preenchida somente quando realizada
    float estresseMedio;            // media relatada pelo grupo, de 0 a 10
} Acao;

// uma funcao para cada opcao do menu
int cadastrarAcao(Acao acoes[], int total);
void listarAcoes(Acao acoes[], int total);
void pesquisarAcoes(Acao acoes[], int total);
void atualizarSituacao(Acao acoes[], int total);
void registrarParticipantes(Acao acoes[], int total);
void gerarResumo(Acao acoes[], int total);

#endif
