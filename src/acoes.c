/******************************************************************************
 *  SISPE - Modulo de acoes
 *  Implementa as seis funcionalidades do sistema sobre o vetor de acoes:
 *  cadastrar, listar, pesquisar, atualizar situacao, registrar participantes
 *  e gerar resumo.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "acoes.h"
#include "entrada.h"

// nome de cada situacao; a posicao no vetor e o codigo da situacao (1, 2 ou 3)
char nomeSituacao[4][11] = {"", "Planejada", "Realizada", "Cancelada"};

// totais de um grupo de acoes, usados no resumo (geral, por subtema ou por escola)
typedef struct {
    char nome[TAM_TEXTO];
    int acoes, planejadas, realizadas, canceladas;
    int previstos, previstosRealizadas, participantes;
    float somaEstresse;
} Grupo;

void linhaSeparadora(int tamanho){
    int i;

    for (i = 0; i < tamanho; i++) printf("-");
    printf("\n");
}

/* Avisa e retorna 1 (verdadeiro) quando ainda nao ha acoes cadastradas. */
int nenhumaAcao(int total){
    if (total == 0){
        printf("Nenhuma acao cadastrada.\n");
        return 1;
    }
    return 0;
}

/* Procura a acao pelo codigo.
   Retorna a posicao no vetor ou -1 se o codigo nao existir. */
int buscarPorCodigo(Acao acoes[], int total, int codigo){
    int i;

    for (i = 0; i < total; i++){
        if (acoes[i].codigo == codigo) return i;
    }
    return -1;
}

/* Caso de uso "Localizar acao pelo codigo": le o codigo e procura a acao.
   Retorna a posicao no vetor ou -1 (com mensagem de erro) se nao encontrar. */
int localizarAcao(Acao acoes[], int total){
    int indice;

    indice = buscarPorCodigo(acoes, total, lerInteiro("Codigo da acao: "));
    if (indice == -1) printf("Erro: nenhuma acao encontrada com o codigo informado.\n");

    return indice;
}

/* Mostra todos os campos de uma acao (usada na pesquisa e na atualizacao). */
void mostrarAcao(Acao acao){
    linhaSeparadora(50);
    printf("Codigo ..................: %d\n", acao.codigo);
    printf("Escola ..................: %s\n", acao.escola);
    printf("Subtema .................: %s\n", acao.subtema);
    printf("Data prevista ...........: %s\n", acao.dataPrevista);
    printf("Publico-alvo ............: %s\n", acao.publicoAlvo);
    printf("Responsavel .............: %s\n", acao.responsavel);
    printf("Participantes previstos .: %d\n", acao.quantidadePrevista);
    printf("Situacao ................: %s\n", nomeSituacao[acao.situacao]);

    if (acao.situacao == REALIZADA){
        printf("Participantes efetivos ..: %d\n", acao.quantidadeRealizada);
        printf("Estresse medio do grupo .: %.1f (escala de 0 a 10)\n", acao.estresseMedio);
    }
    linhaSeparadora(50);
}

/* Caso de uso "Validar dados da acao": le e confere cada dado da nova acao.
   So grava na posicao livre do vetor (acoes[total]) se tudo estiver certo.
   Retorna 1 (dados validos) ou 0 no primeiro erro encontrado. */
int lerDadosAcao(Acao acoes[], int total){
    Acao nova;

    nova.codigo = lerInteiro("Codigo da acao (numero inteiro maior que zero): ");
    if (nova.codigo <= 0){
        printf("Erro: o codigo deve ser um numero inteiro maior que zero.\n");
        return 0;
    }
    if (buscarPorCodigo(acoes, total, nova.codigo) != -1){
        printf("Erro: ja existe uma acao com o codigo %d.\n", nova.codigo);
        return 0;
    }

    if (!lerTexto("Escola: ", nova.escola, TAM_TEXTO)) return 0;
    if (!lerTexto("Subtema (ex.: bullying, uso de telas, ansiedade): ", nova.subtema, TAM_TEXTO)) return 0;

    if (!lerTexto("Data prevista (DD/MM/AAAA): ", nova.dataPrevista, TAM_DATA)) return 0;
    if (!dataValida(nova.dataPrevista)){
        printf("Erro: data invalida. Use o formato DD/MM/AAAA com uma data existente.\n");
        return 0;
    }

    if (!lerTexto("Publico-alvo (ex.: 8o ano, ensino medio): ", nova.publicoAlvo, TAM_TEXTO)) return 0;
    if (!lerTexto("Responsavel pela acao: ", nova.responsavel, TAM_TEXTO)) return 0;

    nova.quantidadePrevista = lerInteiro("Quantidade prevista de participantes: ");
    if (nova.quantidadePrevista <= 0 || nova.quantidadePrevista > MAX_PARTICIPANTES){
        printf("Erro: a quantidade prevista deve ser um numero inteiro de 1 a %d.\n", MAX_PARTICIPANTES);
        return 0;
    }

    // toda acao nova comeca como planejada
    nova.situacao = PLANEJADA;
    nova.quantidadeRealizada = 0;
    nova.estresseMedio = 0;

    acoes[total] = nova;
    return 1;
}

/* OPCAO 1 - Cadastra uma nova acao. Retorna o novo total de acoes.
   Segue o pseudocodigo da 1a entrega: qualquer erro cancela o cadastro. */
int cadastrarAcao(Acao acoes[], int total){
    printf("\n========== CADASTRAR ACAO ==========\n");

    if (total >= MAX_ACOES){
        printf("Erro: limite de %d acoes atingido. Nao e possivel cadastrar novas acoes.\n", MAX_ACOES);
        return total;
    }

    if (!lerDadosAcao(acoes, total)){
        printf("Cadastro cancelado. Nenhum dado foi salvo.\n");
        return total;
    }

    printf("\nAcao cadastrada com sucesso! Codigo: %d | Situacao: Planejada\n", acoes[total].codigo);
    return total + 1;
}

/* OPCAO 2 - Lista todas as acoes em forma de tabela. */
void listarAcoes(Acao acoes[], int total){
    int i;

    printf("\n========== LISTA DE ACOES ==========\n");
    if (nenhumaAcao(total)) return;

    printf("%-4s %-20s %-16s %-10s %-12s %-14s %5s %5s  %-10s\n",
           "Cod", "Escola", "Subtema", "Data", "Publico", "Responsavel", "Prev.", "Real.", "Situacao");
    linhaSeparadora(106);

    for (i = 0; i < total; i++){
        printf("%-4d %-20.20s %-16.16s %-10s %-12.12s %-14.14s %5d ",
               acoes[i].codigo, acoes[i].escola, acoes[i].subtema, acoes[i].dataPrevista,
               acoes[i].publicoAlvo, acoes[i].responsavel, acoes[i].quantidadePrevista);

        if (acoes[i].situacao == REALIZADA) printf("%5d  ", acoes[i].quantidadeRealizada);
        else printf("%5s  ", "-");

        printf("%s\n", nomeSituacao[acoes[i].situacao]);
    }

    linhaSeparadora(106);
    printf("Total: %d acao(oes) cadastrada(s) de %d possiveis.\n", total, MAX_ACOES);
}

/* OPCAO 3 - Pesquisa por codigo, escola ou subtema.
   Na escola e no subtema basta digitar parte do nome, sem diferenciar maiusculas. */
void pesquisarAcoes(Acao acoes[], int total){
    int tipo, indice, i, encontradas;
    char busca[TAM_TEXTO];

    printf("\n========== PESQUISAR ACAO ==========\n");
    if (nenhumaAcao(total)) return;

    printf("1 - Pesquisar por codigo\n");
    printf("2 - Pesquisar por escola\n");
    printf("3 - Pesquisar por subtema\n");
    tipo = lerInteiro("Escolha o tipo de pesquisa: ");

    if (tipo == 1){
        indice = localizarAcao(acoes, total);
        if (indice != -1) mostrarAcao(acoes[indice]);

    }else if (tipo == 2 || tipo == 3){
        if (!lerTexto("Nome (ou parte do nome) a pesquisar: ", busca, TAM_TEXTO)) return;

        encontradas = 0;
        for (i = 0; i < total; i++){
            if ((tipo == 2 && contemTexto(acoes[i].escola, busca)) ||
                (tipo == 3 && contemTexto(acoes[i].subtema, busca))){
                mostrarAcao(acoes[i]);
                encontradas++;
            }
        }

        if (encontradas == 0) printf("Nenhuma acao encontrada para \"%s\".\n", busca);
        else printf("%d acao(oes) encontrada(s).\n", encontradas);

    }else{
        printf("Erro: tipo de pesquisa invalido. Escolha 1, 2 ou 3.\n");
    }
}

/* Le a quantidade efetiva de participantes e o estresse medio do grupo.
   So grava na acao se os dois valores forem validos (retorna 1);
   caso contrario a acao nao e alterada (retorna 0). */
int lerDadosRealizacao(Acao acoes[], int indice){
    int quantidade;
    float estresse;

    quantidade = lerInteiro("Quantidade efetiva de participantes: ");
    if (quantidade < 1 || quantidade > MAX_PARTICIPANTES){
        printf("Erro: a quantidade efetiva deve ser um numero inteiro de 1 a %d.\n", MAX_PARTICIPANTES);
        printf("(Se ninguem participou, marque a acao como cancelada.)\n");
        return 0;
    }

    estresse = lerReal("Estresse medio relatado pelo grupo (0 a 10): ");
    if (estresse < 0 || estresse > 10){
        printf("Erro: o estresse medio deve ser um numero de 0 a 10.\n");
        return 0;
    }

    acoes[indice].quantidadeRealizada = quantidade;
    acoes[indice].estresseMedio = estresse;
    return 1;
}

/* OPCAO 4 - Altera a situacao da acao (planejada, realizada ou cancelada).
   Ao marcar como realizada, os participantes sao registrados na mesma hora. */
void atualizarSituacao(Acao acoes[], int total){
    int indice, novaSituacao;

    printf("\n========== ATUALIZAR SITUACAO ==========\n");
    if (nenhumaAcao(total)) return;

    indice = localizarAcao(acoes, total);
    if (indice == -1) return;

    mostrarAcao(acoes[indice]);
    printf("Nova situacao: 1 - Planejada | 2 - Realizada | 3 - Cancelada\n");
    novaSituacao = lerInteiro("Escolha a nova situacao: ");

    if (novaSituacao < PLANEJADA || novaSituacao > CANCELADA){
        printf("Erro: situacao invalida. Escolha 1, 2 ou 3. Nenhuma alteracao foi feita.\n");
        return;
    }
    if (novaSituacao == acoes[indice].situacao){
        printf("A acao ja esta nessa situacao. Nenhuma alteracao foi feita.\n");
        return;
    }

    if (novaSituacao == REALIZADA){
        printf("Para marcar como realizada, informe os dados da participacao:\n");
        if (!lerDadosRealizacao(acoes, indice)){
            printf("Nenhuma alteracao foi feita.\n");
            return;
        }
    }else if (acoes[indice].situacao == REALIZADA){
        // a acao deixou de ser realizada: os participantes nao podem continuar no resumo
        acoes[indice].quantidadeRealizada = 0;
        acoes[indice].estresseMedio = 0;
        printf("Aviso: os participantes registrados foram removidos, pois a acao deixou de ser realizada.\n");
    }

    acoes[indice].situacao = novaSituacao;
    printf("\nSituacao da acao %d atualizada para: %s\n", acoes[indice].codigo, nomeSituacao[novaSituacao]);
}

/* OPCAO 5 - Registra (ou corrige) os participantes de uma acao ja realizada. */
void registrarParticipantes(Acao acoes[], int total){
    int indice;

    printf("\n========== REGISTRAR PARTICIPANTES ==========\n");
    if (nenhumaAcao(total)) return;

    indice = localizarAcao(acoes, total);
    if (indice == -1) return;

    if (acoes[indice].situacao != REALIZADA){
        printf("Erro: a acao %d esta com a situacao %s.\n", acoes[indice].codigo, nomeSituacao[acoes[indice].situacao]);
        printf("So e possivel registrar participantes de acoes realizadas.\n");
        printf("Use a opcao 4 para marcar a acao como realizada.\n");
        return;
    }

    printf("Participantes previstos: %d | Registrados atualmente: %d | Estresse atual: %.1f\n",
           acoes[indice].quantidadePrevista, acoes[indice].quantidadeRealizada, acoes[indice].estresseMedio);

    if (lerDadosRealizacao(acoes, indice)){
        printf("\nParticipantes da acao %d registrados com sucesso: %d participante(s).\n",
               acoes[indice].codigo, acoes[indice].quantidadeRealizada);
    }else{
        printf("Nenhuma alteracao foi feita.\n");
    }
}

/* Junta as acoes em grupos e soma os totais de cada grupo.
   tipo 'e' = por escola, 's' = por subtema, 'g' = todas num grupo so (resumo geral).
   Nomes iguais com maiusculas diferentes ficam no mesmo grupo.
   Retorna quantos grupos foram criados. */
int agruparAcoes(Acao acoes[], int total, char tipo, Grupo grupos[]){
    Grupo vazio = {0};      // grupo com todos os contadores em zero
    char nome[TAM_TEXTO];
    int totalGrupos, i, j, posicao;

    totalGrupos = 0;
    for (i = 0; i < total; i++){
        if (tipo == 'e') strcpy(nome, acoes[i].escola);
        else if (tipo == 's') strcpy(nome, acoes[i].subtema);
        else strcpy(nome, "Geral");

        // procura o grupo; se ainda nao existe, cria um novo
        posicao = -1;
        for (j = 0; j < totalGrupos; j++){
            if (mesmoTexto(grupos[j].nome, nome)) posicao = j;
        }
        if (posicao == -1){
            posicao = totalGrupos;
            grupos[posicao] = vazio;
            strcpy(grupos[posicao].nome, nome);
            totalGrupos++;
        }

        grupos[posicao].acoes++;
        grupos[posicao].previstos += acoes[i].quantidadePrevista;

        if (acoes[i].situacao == PLANEJADA){
            grupos[posicao].planejadas++;
        }else if (acoes[i].situacao == REALIZADA){
            grupos[posicao].realizadas++;
            grupos[posicao].previstosRealizadas += acoes[i].quantidadePrevista;
            grupos[posicao].participantes += acoes[i].quantidadeRealizada;
            grupos[posicao].somaEstresse += acoes[i].estresseMedio;
        }else{
            grupos[posicao].canceladas++;
        }
    }
    return totalGrupos;
}

/* Mostra a tabela de grupos do resumo por subtema ou por escola. */
void mostrarGrupos(Grupo grupos[], int totalGrupos, char titulo[]){
    int j;

    printf("\n--- RESUMO POR %s ---\n", titulo);
    printf("%-24s %5s %5s %5s %5s %9s %9s\n", "Nome", "Acoes", "Plan.", "Real.", "Canc.", "Particip.", "Estresse");
    linhaSeparadora(68);

    for (j = 0; j < totalGrupos; j++){
        printf("%-24.24s %5d %5d %5d %5d %9d ", grupos[j].nome, grupos[j].acoes, grupos[j].planejadas,
               grupos[j].realizadas, grupos[j].canceladas, grupos[j].participantes);

        if (grupos[j].realizadas > 0) printf("%9.1f\n", grupos[j].somaEstresse / grupos[j].realizadas);
        else printf("%9s\n", "-");
    }
    linhaSeparadora(68);
}

/* OPCAO 6 - Resumo geral, por subtema e por escola (fluxograma 9.2 da 1a entrega). */
void gerarResumo(Acao acoes[], int total){
    Grupo geral[1], grupos[MAX_ACOES];
    int totalGrupos;

    printf("\n========== RESUMO GERAL ==========\n");
    if (nenhumaAcao(total)) return;

    agruparAcoes(acoes, total, 'g', geral);

    printf("Acoes cadastradas ...................: %d\n", geral[0].acoes);
    printf("  Planejadas ........................: %d\n", geral[0].planejadas);
    printf("  Realizadas ........................: %d\n", geral[0].realizadas);
    printf("  Canceladas ........................: %d\n", geral[0].canceladas);
    printf("Participantes previstos (todas) .....: %d\n", geral[0].previstos);
    printf("Participantes previstos (realizadas) : %d\n", geral[0].previstosRealizadas);
    printf("Participantes efetivos (realizadas) .: %d\n", geral[0].participantes);

    if (geral[0].realizadas > 0){
        // o percentual compara so as acoes que aconteceram com o que elas previam
        printf("Percentual de participacao ..........: %.1f%%\n",
               (float) geral[0].participantes / geral[0].previstosRealizadas * 100);
        printf("Estresse medio relatado (0 a 10) ....: %.1f\n", geral[0].somaEstresse / geral[0].realizadas);
    }else{
        printf("Percentual de participacao ..........: sem acoes realizadas\n");
        printf("Estresse medio relatado (0 a 10) ....: sem acoes realizadas\n");
    }

    totalGrupos = agruparAcoes(acoes, total, 's', grupos);
    mostrarGrupos(grupos, totalGrupos, "SUBTEMA");

    totalGrupos = agruparAcoes(acoes, total, 'e', grupos);
    mostrarGrupos(grupos, totalGrupos, "ESCOLA");
}
