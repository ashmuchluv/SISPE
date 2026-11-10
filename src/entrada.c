/******************************************************************************
 *  SISPE - Modulo de entrada de dados
 *  Toda leitura do teclado passa por aqui, para que nenhuma entrada errada
 *  (letras no lugar de numeros, campos vazios, datas impossiveis) chegue
 *  ao vetor de acoes.
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"

/* Le uma linha inteira do teclado e retira o ENTER do final.
   Retorna 1 se a linha coube no vetor e 0 se o texto era grande demais. */
int lerLinha(char linha[], int tamanho){
    int i, c, coube;

    if (fgets(linha, tamanho, stdin) == NULL){
        printf("\nFim da entrada de dados. Encerrando o SISPE.\n");
        exit(0);
    }

    i = 0;
    while (linha[i] != '\0' && linha[i] != '\n') i++;

    coube = 1;
    if (linha[i] == '\n'){
        linha[i] = '\0';
    }else{
        // o texto nao coube: descarta o que sobrou para nao atrapalhar a proxima leitura
        coube = 0;
        do{
            c = getchar();
        }while (c != '\n' && c != EOF);
    }

    // remove o \r que sobra quando o ENTER vem de um arquivo do Windows
    if (i > 0 && linha[i-1] == '\r') linha[i-1] = '\0';

    return coube;
}

/* Le um numero inteiro.
   Retorna -1 quando o usuario digita algo que nao e numero.
   Como nenhum campo do sistema aceita valor negativo, -1 sempre sera recusado. */
int lerInteiro(char mensagem[]){
    char linha[TAM_LINHA];
    char sobra;
    int valor;

    printf("%s", mensagem);
    if (lerLinha(linha, TAM_LINHA) == 0) return -1;
    if (strlen(linha) > 9) return -1;      // evita numeros grandes demais para um int
    if (sscanf(linha, "%d %c", &valor, &sobra) != 1) return -1;

    return valor;
}

/* Le um numero real. Aceita virgula ou ponto (6,5 ou 6.5).
   Retorna -1 quando o valor digitado nao e numero. */
float lerReal(char mensagem[]){
    char linha[TAM_LINHA];
    char sobra;
    float valor;
    int i;

    printf("%s", mensagem);
    if (lerLinha(linha, TAM_LINHA) == 0) return -1;

    for (i = 0; linha[i] != '\0'; i++){
        if (linha[i] == ',') linha[i] = '.';
    }

    if (sscanf(linha, "%f %c", &valor, &sobra) != 1) return -1;

    return valor;
}

/* Le um texto obrigatorio, sem espacos sobrando no inicio e no fim.
   Retorna 1 se o texto e valido e 0 se esta vazio ou grande demais. */
int lerTexto(char mensagem[], char destino[], int tamanho){
    char linha[TAM_LINHA];
    int inicio, fim;

    printf("%s", mensagem);
    if (lerLinha(linha, TAM_LINHA) == 0){
        printf("Erro: texto muito longo (maximo de %d caracteres).\n", tamanho - 1);
        return 0;
    }

    // pula os espacos do inicio
    inicio = 0;
    while (linha[inicio] == ' ' || linha[inicio] == '\t') inicio++;

    // apaga os espacos do fim
    fim = (int) strlen(linha) - 1;
    while (fim >= inicio && (linha[fim] == ' ' || linha[fim] == '\t')){
        linha[fim] = '\0';
        fim--;
    }

    if (linha[inicio] == '\0'){
        printf("Erro: este campo e obrigatorio e nao pode ficar vazio.\n");
        return 0;
    }

    if ((int) strlen(&linha[inicio]) > tamanho - 1){
        printf("Erro: texto muito longo (maximo de %d caracteres).\n", tamanho - 1);
        return 0;
    }

    strcpy(destino, &linha[inicio]);

    return 1;
}

/* Confere se a data esta no formato DD/MM/AAAA e se o dia existe no mes.
   Retorna 1 (verdadeiro) ou 0 (falso). */
int dataValida(char data[]){
    int diasMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dia, mes, ano, i;

    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;

    for (i = 0; i < 10; i++){
        if (i != 2 && i != 5 && (data[i] < '0' || data[i] > '9')) return 0;
    }

    // converte os caracteres em numeros usando a tabela ASCII
    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    ano = (data[6] - '0') * 1000 + (data[7] - '0') * 100 + (data[8] - '0') * 10 + (data[9] - '0');

    if (ano < 2000 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;

    // ano bissexto: fevereiro tem 29 dias
    if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) diasMes[1] = 29;

    if (dia < 1 || dia > diasMes[mes - 1]) return 0;

    return 1;
}

/* Copia o texto trocando as letras maiusculas (A a Z) por minusculas. */
void paraMinusculo(char origem[], char destino[]){
    int i;

    for (i = 0; origem[i] != '\0'; i++){
        if (origem[i] >= 'A' && origem[i] <= 'Z') destino[i] = origem[i] + 32;
        else destino[i] = origem[i];
    }
    destino[i] = '\0';
}

/* Compara dois textos sem diferenciar maiusculas de minusculas. */
int mesmoTexto(char texto1[], char texto2[]){
    char minusculo1[TAM_LINHA], minusculo2[TAM_LINHA];

    paraMinusculo(texto1, minusculo1);
    paraMinusculo(texto2, minusculo2);

    return strcmp(minusculo1, minusculo2) == 0;
}

/* Verifica se "busca" aparece dentro de "texto", sem diferenciar maiusculas. */
int contemTexto(char texto[], char busca[]){
    char minusculoTexto[TAM_LINHA], minusculoBusca[TAM_LINHA];

    paraMinusculo(texto, minusculoTexto);
    paraMinusculo(busca, minusculoBusca);

    return strstr(minusculoTexto, minusculoBusca) != NULL;
}

void pausar(){
    char linha[TAM_LINHA];

    printf("\nPressione ENTER para voltar ao menu...");
    lerLinha(linha, TAM_LINHA);
}
