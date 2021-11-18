
#include <string.h>

#include "algs_ordenacao.h"

const Algoritimo* const LISTA_ALGORITIMOS[] = {
	&alg_bubble_sort,
	&alg_select_sort,
	&alg_quick_sort,
	&alg_insertion_sort,
	&alg_binary_insertion_sort,
	&alg_merge_sort,
	&alg_heap_sort,
	&alg_bucket_sort
};
const unsigned int NUM_ALGORITIMOS = \
	(sizeof(LISTA_ALGORITIMOS) / sizeof(const Algoritimo* const));

const Algoritimo* procurar_algoritimo(const char* nome) {
	for (unsigned int i = 0; i < NUM_ALGORITIMOS; ++i) {
		const Algoritimo* alg = LISTA_ALGORITIMOS[i];
		if (strcmp(nome, alg->nome) == 0) return alg;
	}
	return NULL;
}

