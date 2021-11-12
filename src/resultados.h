#ifndef __RESULTADOS_H
#define __RESULTADOS_H

#include "algoritimo.h"
#include "temporizacao.h"

typedef struct ResultadoAlgoritimo {
	const Algoritimo* algoritimo;
	tempo_t*          tempos;
	unsigned int      tempos_tamanho;
	int               erro;
} ResultadoAlgoritimo;

int inicia_struct_resultado(const Algoritimo* alg,
		ResultadoAlgoritimo* res, unsigned int temporizacoes);

void finaliza_struct_resultado(ResultadoAlgoritimo* res);

ResultadoAlgoritimo* criar_vetor_resultados(const Algoritimo* const* algs,
		unsigned int qtd_algoritimos, unsigned int temporizacoes);

void destruir_vetor_resultados(\
		ResultadoAlgoritimo* res, unsigned int qtd_algoritimos);

#endif
