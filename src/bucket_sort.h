#ifndef __BUCKET_SORT_H
#define __BUCKET_SORT_H

#include "algoritimo.h"

extern Algoritimo alg_bucket_sort;

/**
 * algoritimo de ordenação bucket sort
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas fornecidas
 *
 * @returns codigo de erro diferente de zero (se existir)
 */
int bucket_sort(ListaLinhas* linhas, unsigned int qtd_linhas);

#endif
