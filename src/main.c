/******************************************************************************
 *  SISPE - Sistema de Apoio ao Planejamento e ao Acompanhamento de Acoes
 *          de Saude Mental no Programa Saude na Escola (PSE)
 *
 *  AEP 2026 - Analise e Desenvolvimento de Sistemas - UniCesumar
 *  Integrantes: Lyvia Vitoria dos Santos Silva e Gabriel Detomini Rabelo
 *
 *  Os dados ficam em memoria (vetor de structs) durante a execucao.
 *  Use somente dados ficticios e coletivos: o sistema nao guarda dados
 *  de estudantes, nao faz diagnostico e nao recomenda tratamento.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "acoes.h"
#include "entrada.h"

void exibirMenu(int total){
    printf("\n==================================================\n");
    printf("   SISPE - Acoes de Saude Mental no PSE\n");
    printf("==================================================\n");
    printf(" Acoes cadastradas: %d de %d\n", total, MAX_ACOES);
    printf(" (use apenas dados ficticios e coletivos)\n");
    printf("--------------------------------------------------\n");
    printf(" 1 - Cadastrar acao\n");
    printf(" 2 - Listar acoes\n");
    printf(" 3 - Pesquisar acao\n");
    printf(" 4 - Atualizar situacao\n");
    printf(" 5 - Registrar participantes\n");
    printf(" 6 - Gerar resumo\n");
    printf(" 0 - Sair\n");
    printf("--------------------------------------------------\n");
}

int main(){
    Acao acoes[MAX_ACOES];
    int totalAcoes = 0;
    int opcao;

    // o menu se repete ate o usuario escolher 0 (fluxograma 9.1)
    do{
        exibirMenu(totalAcoes);
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao){
            case 1:
                totalAcoes = cadastrarAcao(acoes, totalAcoes);
                break;
            case 2:
                listarAcoes(acoes, totalAcoes);
                break;
            case 3:
                pesquisarAcoes(acoes, totalAcoes);
                break;
            case 4:
                atualizarSituacao(acoes, totalAcoes);
                break;
            case 5:
                registrarParticipantes(acoes, totalAcoes);
                break;
            case 6:
                gerarResumo(acoes, totalAcoes);
                break;
            case 0:
                printf("\nEncerrando o SISPE. Ate logo!\n");
                break;
            default:
                printf("\nErro: opcao invalida! Digite um numero de 0 a 6.\n");
        }

        if (opcao != 0) pausar();
    }while (opcao != 0);

    return 0;
}
