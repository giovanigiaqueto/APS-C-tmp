
#include "temporizar.h"

#include <time.h>
#include <stdlib.h>

int temporizar(char** linhas, unsigned int qtd_linhas,
		Algoritimo alg, tempo_t* tempo) {

	clock_t t0, t1;
	if (alg.tipo == ALG_VETOR) {

		char** linhas_copia = malloc(qtd_linhas * sizeof(char*));
		if (linhas_copia == NULL) return -1;

		memcpy(linhas_copia, linhas, qtd_linhas);

		t0 = clock();
		alg.func.alg_vetor();
		t1 = clock();

		free((void*) linhas_copia);

	} else {

		ListaLinhas* lista = criar_lista_linhas();
		if (lista == NULL) return -1;

		ListaLinhas_No* base = lista.base;
		for (unsigned int idx = 0; idx < qtd_linhas; ++idx) {
			if (inserir_lista_linhas(&base, linhas[idx]) != 0) {
				deletar_lista_linhas(lista);
				return -1;
			}
			base = &(base->proximo);
		}

		t0 = clock();
		alg.func.alg_lista(lista, qtd_linhas);
		t1 = clock();

		deletar_lista_linhas(lista);
	}

	return (tempo_t) (t1 - t0) / CLOCKS_PER_SEC;
}
