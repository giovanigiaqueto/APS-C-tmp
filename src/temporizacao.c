
#include "temporizacao.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

/**
 * temporiza o algoritimo de ordenação usando as
 * linhas fornecidas e retorna o tempo de excução.
 *
 * NOTA:
 *   faz uma cópia interna da lista de linhas,
 *   preservando a lista fornecida.
 *
 * AVISO:
 *   resultado indefinido se linhas, alg,
 *   tempo ou err_alg forem NULL.
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas fornecidas
 * @param alg o algoritimo a ser temporizado
 * @param tempo o pointeiro usado para
 *      retornar o tempo de execução.
 * @param err_alg ponteiro usado para
 *      retornar o código de erro do
 *      algoritimo usado se ele falhar.
 *
 */
int temporizar(char** linhas, unsigned int qtd_linhas,
		Algoritimo* alg, tempo_t* tempo, int* err_alg) {

	clock_t t0, t1;
	if (alg->tipo == ALG_VETOR) {

		char** linhas_copia = malloc(qtd_linhas * sizeof(char*));
		if (linhas_copia == NULL) {
			*err_alg = 0;
			return -1;
		}

		memcpy(linhas_copia, linhas, qtd_linhas);

		t0 = clock();
		*err_alg = alg->func.alg_vetor(linhas_copia, qtd_linhas);
		t1 = clock();

		free((void*) linhas_copia);

	} else {

		ListaLinhas* lista = criar_lista_linhas();
		if (lista == NULL) return -1;

		ListaLinhas_No** base = &lista->base;
		for (unsigned int idx = 0; idx < qtd_linhas; ++idx) {
			if (inserir_lista_linhas(base, linhas[idx]) != 0) {
				deletar_lista_linhas(&lista);
				*err_alg = 0;
				return -1;
			}
			base = &(*base)->proximo;
		}

		t0 = clock();
		*err_alg = alg->func.alg_lista(lista, qtd_linhas);
		t1 = clock();

		deletar_lista_linhas(&lista);
	}

	*tempo = (tempo_t) (t1 - t0) / CLOCKS_PER_SEC;
	return (*err_alg != 0) ? -2:0;
}
