
#include "heap_sort.h"

#include <string.h> // strcmp

const Algoritimo alg_heap_sort = {
	.nome = "Heap Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = heap_sort
};

/**
 * algoritimo swap para trocar duas linhas
 *
 * @param a primeira linha
 * @param b segunda linha
 *
 * @warning causará falha de segmentação se a ou b forem NULL
 */
static inline void swap(char** a, char** b) {
	char* tmp = *a;
	*a = *b;
	*b = tmp;
}

/**
 * cria a max heap do vetor de linhas
 *
 * @note a implementação desse heapify tem complexidade O(n*log(n))
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
void heapify(char** linhas, unsigned int qtd_linhas) {

  // cria a max heap
  for (unsigned int i = 1; i < qtd_linhas; ++i) {
    unsigned int filho = i;
		unsigned int pai   = (filho - 1) / 2;
		while (filho > 0) {
			// se o filho for maior que o pai, troca os dois
			if (strcmp(linhas[filho], linhas[pai]) > 0) {
				swap(&linhas[filho], &linhas[pai]);
			}
			// atualiza os nós de busca e "heapficação"
			filho = pai;
			pai = (filho - 1) / 2;
		}
  }
}

/**
 * cria a max heap inicial do vetor de linhas
 *
 * @note pode ser um macro ou função inline
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
static inline void max_heap(char** linhas, unsigned int qtd_linhas) {
	heapify(linhas, qtd_linhas);
}

/**
 * ordena as linhas usando o algoritimo de ordenação heap sort.
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @returns zero se for executado com sucesso, não zero se houver um erro
 */
int heap_sort(char** linhas, unsigned int qtd_linhas) {

	// cria a max heap inicial
	max_heap(linhas, qtd_linhas);

	for (unsigned int i = qtd_linhas; i > 0;) {
		// troca o último elemento não ordenado com o primeiro
		swap(&linhas[0], &linhas[--i]);

		// heapficação
		heapify(linhas, i);
	}

	return 0;
}
