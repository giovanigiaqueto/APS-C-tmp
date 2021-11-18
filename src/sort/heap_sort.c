
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

#ifdef WANTS_HEAP_SORT_SIFT_UP
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, definicao do heapify 'sift-up' ativa
#endif
/**
 * cria a max heap do vetor de linhas
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
void heapify_sift_up(char** linhas, unsigned int qtd_linhas) {

  // conserta a heap usando uma peneira de cima pra baixo
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
#endif

#ifdef WANTS_HEAP_SORT_SIFT_DOWN
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, definicao do heapify 'sift-down' ativa
#endif
/**
 * cria a max heap do vetor de linhas
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
void heapify_sift_down(char** linhas, unsigned int qtd_linhas) {

  // conserta a heap usando uma peneira de baixo pra cima
	// NOTA: 'i' é inicializado com a posição do último nó pai
  for (int i = ((int) qtd_linhas - 2) / 2; i >= 0; --i) {

		// pai e seu filho na esquerda
		unsigned int pai   = i;
		unsigned int filho = 2 * pai + 1;

		while (filho < qtd_linhas) {
			// nó a ser trocado
			unsigned int troca = pai;

			// maior entre o pai e o filho da esquerda
			if (strcmp(linhas[filho], linhas[troca]) > 0) {
				troca = filho;
			}
			// maior entre o pai e os dois filhos
			if (filho + 1 < qtd_linhas && strcmp(linhas[filho+1], linhas[troca]) > 0) {
				troca = filho + 1;
			}

			// se o maior valor for o pai, a heap está pronta
			if (troca == pai) break;
			else {
				// troca o pai com o filho e continua trocando
				// todos os descendentes do filho trocado com o pai
				swap(&linhas[pai], &linhas[troca]);
				pai = troca;
			}

			// filho da esquerda
			filho = 2 * pai + 1;
		}
  }
}
#endif

#ifdef WANTS_HEAP_SORT_BOTTOM_UP
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, definicao do heapify 'bottom-up' ativa
#endif

/**
 * função auxiliar do heapify_bottom_up
 *
 * procura o maior valor na heap que não tem um filho
 *
 * @param linhas o vetor de linhas a ser procurado
 * @param qtd_linhas a quantidade de linhas no vetor
 *
 * @returns o indice do maior valor na heap que não tem um filho
 */
static inline unsigned int __heapify_procura_filho(\
		char** linhas, unsigned int qtd_linhas) {

	// procura o pai ou filho à direita com maior valor na heap
	unsigned int pai   = 0;
	unsigned int filho = 2 * pai + 2;
	while (filho < qtd_linhas) {
		// verifica qual filho é o maior
		if (strcmp(linhas[filho], linhas[filho - 1]) > 0) {
			pai = filho;
		} else {
			pai = filho - 1;
		}
		filho = 2 * pai + 2;
	}

	// o maior pai encontrado pelo algoritimo a cima ainda pode
	// ter um filho à esquerda, caso isso for verdade retorne ele
	filho = 2 * pai + 1;
	return (filho < qtd_linhas) ? filho : pai;
}

/**
 * cria a max heap do vetor de linhas
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
void heapify_bottom_up(char** linhas, unsigned int qtd_linhas) {
	
	// procura o maior valor na heap que não tenha nenhum filho,
	// depois acha o primeiro ancestral desse filho que é maior
	// do que o valor que foi trocado pelo heap sort (primeiro indice)
	unsigned int j = __heapify_procura_filho(linhas, qtd_linhas);
	while (strcmp(linhas[0], linhas[j]) > 0) j = (j - 1) / 2;

	// insere o valor na heap
	char* tmp = linhas[j];
	linhas[j] = linhas[0];
	while (j > 0) {
		unsigned int pai = (j - 1) / 2;
		swap(&tmp, &linhas[pai]);
		j = pai;
	}
}
#endif

/**
 * cria a max heap do vetor de linhas
 *
 * @note pode ser um macro
 *
 * @param linhas as linhas a serem "heapficadas"
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @warning causará falha de segmentação se 'linhas' for NULL
 * @warning causará falha de segmentação se uma ou mais linhas forem NULL
 */
#ifdef HEAP_SORT_HEAPIFY_BOTTOM_UP

// define o max_heap, pois o heapify dessa implementação
// só corrige a heap, mas não é capaz de ordena-la
#ifdef WANTS_HEAP_SORT_SIFT_UP

#define max_heap(linhas, qtd_linhas) heapify_sift_up((linhas), (qtd_linhas))

#else
#ifdef WANTS_HEAP_SORT_SIFT_DOWN

#define max_heap(linhas, qtd_linhas) heapify_sift_down((linhas), (qtd_linhas))

#else
#error heap sort, erro de implementacao 'bottom-up', nao foi possivel definir \
um algoritimo para a funcao max_heap
#endif
#endif

#define heapify(linhas, qtd_linhas) heapify_bottom_up((linhas), (qtd_linhas))
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, implementacao do heapify 'bottom-up'
#endif

#else
#ifdef HEAP_SORT_HEAPIFY_SIFT_UP

#define heapify(linhas, qtd_linhas) heapify_sift_up((linhas), (qtd_linhas))
#define max_heap(linhas, qtd_linhas) heapify((linhas), (qtd_linhas))
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, implementacao do heapify 'sift-up'
#endif

#else
#ifdef HEAP_SORT_HEAPIFY_SIFT_DOWN

#define heapify(linhas, qtd_linhas) heapify_sift_down((linhas), (qtd_linhas))
#define max_heap(linhas, qtd_linhas) heapify((linhas), (qtd_linhas))
#ifdef DEBUG_HEAP_SORT_IMPL
#warning heap sort, implementacao do heapify 'sift-down'
#endif

#endif
#endif
#endif

/**
 * ordena as linhas usando o algoritimo de ordenação heap sort.
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas no vetor de linhas
 *
 * @returns zero se for executado com sucesso, não zero se houver um erro
 */
int heap_sort(char** linhas, unsigned int qtd_linhas) {

	// cria a max heap
	max_heap(linhas, qtd_linhas);

	for (long int i = qtd_linhas - 1; i > 0; --i) {
		// troca o último elemento não ordenado com o primeiro
		swap(&linhas[0], &linhas[i]);

		// heapficação
		heapify(linhas, i);
	}

	return 0;
}
