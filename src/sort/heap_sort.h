
#ifndef __HEAP_SORT_H
#define __HEAP_SORT_H

#include "../algoritimo.h"

// configurações da implementação do heap sort
#include "heap_sort_impl.h"

// implementações possíveis do heap sort
#define HEAP_SORT_SIFT_UP   1
#define HEAP_SORT_SIFT_DOWN 2
#define HEAP_SORT_BOTTOM_UP 3

// escolhe a implementação do heap sort baseado em macros
#ifdef WANTS_HEAP_SORT_BOTTOM_UP

// requer pelo menos um desses para criação da max heap inicial,
// pois o heapify da implementação 'bottom-up' só corrige a heap,
// mas não é capaz de ordena-la
#ifndef WANTS_HEAP_SORT_SIFT_DOWN
#ifndef WANTS_HEAP_SORT_SIFT_UP
#define WANTS_HEAP_SORT_SIFT_UP
#endif
#endif

#define HEAP_SORT_HEAPIFY_BOTTOM_UP
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort com implementacao 'bottom-up'
#endif

#else
#ifdef WANTS_HEAP_SORT_SIFT_UP

#define HEAP_SORT_HEAPIFY_SIFT_UP
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort com implementacao 'sift-up'
#endif

#else
#ifdef WANTS_HEAP_SORT_SIFT_DOWN

#define HEAP_SORT_HEAPIFY_SIFT_DOWN
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort com implementacao 'sift-down'
#endif

#else

#define HEAP_SORT_DEFAULT_IMPL

#endif
#endif
#endif

// implementação padrão do heap sort
#ifdef HEAP_SORT_DEFAULT_IMPL

#define WANTS_HEAP_SORT_SIFT_DOWN
#define HEAP_SORT_HEAPIFY_SIFT_DOWN
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort com implementacao padrao 'sift-down'
#endif

#endif

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
