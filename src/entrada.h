/******************************************************************************
 *  SISPE - Modulo de entrada de dados
 *  Funcoes de leitura do teclado e de validacao das entradas do usuario.
 ******************************************************************************/
#ifndef ENTRADA_H
#define ENTRADA_H

#define TAM_LINHA 200

int lerLinha(char linha[], int tamanho);
int lerInteiro(char mensagem[]);
float lerReal(char mensagem[]);
int lerTexto(char mensagem[], char destino[], int tamanho);
int dataValida(char data[]);
void paraMinusculo(char origem[], char destino[]);
int mesmoTexto(char texto1[], char texto2[]);
int contemTexto(char texto[], char busca[]);
void pausar();

#endif
