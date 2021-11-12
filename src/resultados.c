
#include <stdlib.h>

#include "resultados.h"

int inicia_struct_resultado(const Algoritimo* alg,
		ResultadoAlgoritimo* res, unsigned int temporizacoes) {

	res->algoritimo = alg;
	res->tempos_tamanho = temporizacoes;
	res->tempos = malloc(temporizacoes * sizeof(tempo_t));
	res->erro = 0;
	if (res->tempos == NULL) return -1;

	for (unsigned int i = 0; i < temporizacoes; ++i) {
		res->tempos[i] = (tempo_t) 0;
	}

	return 0;
}

void finaliza_struct_resultado(ResultadoAlgoritimo* res) {
	if (res == NULL) return;

	free((void*) res->tempos);
	res->algoritimo = NULL;
	res->tempos_tamanho = 0;
	res->tempos = NULL;
}

ResultadoAlgoritimo* criar_vetor_resultados(const Algoritimo* const* algs,
		unsigned int qtd_algoritimos, unsigned int temporizacoes) {

	ResultadoAlgoritimo* res = malloc(qtd_algoritimos * sizeof(ResultadoAlgoritimo));
	if (res == NULL) return NULL;

	// inicializa as estruturas
	unsigned int idx;
	for (idx = 0; idx < qtd_algoritimos; ++idx) {
		if (inicia_struct_resultado(\
					algs[idx], &res[idx], temporizacoes) != 0) break;
	}

	// verifica se houve um erro de inicialização e libera memória
	if (idx != qtd_algoritimos) {
		for (unsigned int i = 0; i < idx; ++i) {
			finaliza_struct_resultado(&res[i]);
		}
		free((void*) res);
		res = NULL;
	}

	return res;
}

void destruir_vetor_resultados(ResultadoAlgoritimo* res, unsigned int qtd_algoritimos) {
	if (res == NULL) return;

	for (unsigned int i = 0; i < qtd_algoritimos; ++i) {
		finaliza_struct_resultado(&res[i]);
	}
	free((void*) res);
}

