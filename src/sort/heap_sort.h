
#ifndef __HEAP_SORT_H
#define __HEAP_SORT_H

#include "../algoritimo.h"

extern const Algoritimo alg_heap_sort;

/**
 * ordena as linhas usando o algoritimo de ordenação heap sort.
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @returns zero se for executado com sucesso, não zero se houver um erro
 */
int heap_sort(char** linhas, unsigned int qtd_int);

#endif
