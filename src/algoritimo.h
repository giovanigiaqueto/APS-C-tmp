#ifndef __ALGORITIMO_H
#define __ALGORITIMO_H

#include "lista_linhas.h"

/**
 * módulo para facilitar o uso das funções de ordenação
 * pelo módulo de temporização.
 */

// enum dos tipos de função de ordenação suportados
typedef enum { ALG_VETOR, ALG_LISTA } TipoAlgoritimo;

// tipos de ponteiros para os tipos de função de ordenação suportados.
typedef int (*alg_vetor_fptr)(char**,       unsigned int);
typedef int (*alg_lista_fptr)(ListaLinhas*, unsigned int);

// união de ponteiros para os tipos de função de ordenação suportados.
typedef union FuncAlgoritimo {
	alg_vetor_fptr alg_vetor;
	alg_lista_fptr alg_lista;
} FuncAlgoritimo;

// estrutura com informações gerêricas de cada algoritimo,
// deve ser definida em cada módulo contendo uma função de ordenação.
typedef struct Algoritimo {
	TipoAlgoritimo tipo;
	FuncAlgoritimo func;
} Algoritimo;

#endif
