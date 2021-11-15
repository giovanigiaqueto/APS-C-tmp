
/* ======================= INCLUDES ======================= */

#include "geracao_dados.h"

#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include <stdio.h>

/* ================ VARIÁVEIS E CONSTANTES ================ */

const char ALFABETO_PADRAO[] = {\
		"abcdefghijklmnopqrstuvwxyz" \
		"abcdefghijklmnopqrstuvwxyz" \
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
		"12345678901234567890" \
		",. " "\0" \
};

const ConfGeracaoDados conf_padrao = {
	.tamanho_linha_min = 80,
	.tamanho_linha_max = 20,

	.alfabeto = ALFABETO_PADRAO,
	.tamanho_alfabeto = sizeof(ALFABETO_PADRAO) / sizeof(char)
};
const ConfGeracaoDados* conf_alternativa = NULL;


/* ============= CONFIGURAÇÃO E INICIALIZAÇÃO ============= */

int iniciado = 0;
void init_geracao_dados() {
	if (iniciado) return;
	iniciado = 1;

	srand(time(NULL));
	return;
}

int validar_conf_geracao_dados(const ConfGeracaoDados* conf) {
	if (conf->tamanho_linha_min >= conf->tamanho_linha_max) return -1;
	if (conf->tamanho_linha_max == 0) return -1;
	if (conf->alfabeto == NULL) return -1;
	if (conf->tamanho_alfabeto == 0) return -1;
	return 0;
}

const ConfGeracaoDados* set_conf_geracao_dados(const ConfGeracaoDados* conf) {
	// verifica se a configuracao de geração
	// é válida antes de prosseguir
	if (validar_conf_geracao_dados(conf) != 0) {
		return get_conf_geracao_dados();
	}
	return (conf_alternativa = conf);
}

const ConfGeracaoDados* get_conf_geracao_dados() {
	return conf_alternativa != NULL ? conf_alternativa : &conf_padrao;
}


/* ==================== GERAÇÃO DE DADOS ==================== */

#if ULONG_MAX > UINT_MAX
static inline long randlong() {
	return (((long) rand()) & ((long) ~0)) | (((long) rand()) << (8 * sizeof(int)));
}
#define __MY_LONG_RAND_MAX ((((long) RAND_MAX) & ((long) ~0)) | (((long) RAND_MAX) << (8 * sizeof(int))))
#else

static inline long randlong() {
	return (long) rand();
}
#define __MY_LONG_RAND_MAX ((long) RAND_MAX)
#endif

/**
 * geração de números aleatórios entre min e max, com distribuição uniforme.
 *
 * @param int valor minimo a ser gerado (incluso)
 * @param int valor máximo a ser gerado (não incluso)
 *
 * @returns valor aleatório gerado
 *
 * NOTA:
 *   fazer o módulo de rand() pelo tamanho da amostra necessária
 *   não gera números aleatórios uniformemente distribuídos,
 *   pois RAND_MAX raramente será divisível pelo tamanho
 *   da amostra, fazendo com que o módulo retorne mais valores
 *   próximos ao zero (principalmente se RAND_MAX for pequeno).
 *
 *   por conta disso essa implementação gera valores aleatórios
 *   maiores, descarta parte deles para que o resultado seja
 *   bem distribuído, e depois faz o módulo.
 */
int gerar_randint(int min, int max) {
	// calcula as regiões externa e interna à amostra,
	// necessárias para gerar o número aleatório
	long range = ((long) max) - ((long) min);
	long cutoff = range * (__MY_LONG_RAND_MAX / range);

	// gera o número na faixa necessária que é divisível por 'range'
	long valor;
	do {
		valor = randlong();
	} while(valor > cutoff || valor < -cutoff);

	// retorna o número gerado
	return min + (valor % range);
}

/**
 * implementação do algoritimo Fisher-Yates
 * para embaralhamento de vetores
 */
void embaralhar_dados(char** linhas, unsigned int qtd_linhas) {
	if (linhas == NULL || qtd_linhas <= 1) return;

	for (unsigned int i = 0; i < qtd_linhas - 1; ++i) {
		unsigned int idx = gerar_randint(i, qtd_linhas);
		char* tmp = linhas[i];
		linhas[i] = linhas[idx];
		linhas[idx] = tmp;
	}
}

char* gerar_linha(const ConfGeracaoDados* conf) {
	const unsigned int tamanho_linha = gerar_randint(\
		conf->tamanho_linha_min, conf->tamanho_linha_max);

	char* linha = malloc((tamanho_linha + 1) * sizeof(char*));
	if (linha != NULL) {
		for (unsigned int i = 0; i < tamanho_linha; ++i) {
			const unsigned int idx = gerar_randint(0, conf->tamanho_alfabeto);
			linha[i] = conf->alfabeto[idx];
		}
	}
	linha[tamanho_linha] = '\0';

	return linha;
}

char** gerar_dados(unsigned int qtd_linhas) {
	const ConfGeracaoDados* conf = get_conf_geracao_dados();
	if (conf == NULL) return NULL;

	char** linhas = malloc(qtd_linhas * sizeof(char*));
	if (linhas == NULL) return NULL;

	int erro = 0;
	unsigned int idx = 0;
	for (; idx < qtd_linhas; ++idx) {
		char* linha = gerar_linha(conf);
		if (linha == NULL) {
			erro = 1;
			break;
		}

		linhas[idx] = linha;
	}

	if (erro) {
		// um erro ocorreu, libera a memória alocada
		for (unsigned int i = 0; i < idx; ++i) {
			free((void*) linhas[idx]);
		}
		free((void*) linhas);
		linhas = NULL;
	}
	return linhas;
}

