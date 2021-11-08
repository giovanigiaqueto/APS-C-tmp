#ifndef __ALGS_ORDENACAO_H
#define __ALGS_ORDENACAO_H

#include "algoritimo.h"

#include "sort/bubble_sort.h"
#include "sort/select_sort.h"
#include "sort/quick_sort.h"
#include "sort/insertion_sort.h"
#include "sort/binary_insertion_sort.h"
#include "sort/merge_sort.h"
//#include "sort/heap_sort.h"
#include "sort/bucket_sort.h"

extern const Algoritimo* const LISTA_ALGORITIMOS[];
extern const unsigned int      NUM_ALGORITIMOS;

const Algoritimo* procurar_algoritimo(const char* nome);

#endif
