
/* ======================= INCLUDES ======================= */

#include "geracao_dados.h"
#include "aleatorio.h"

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

/**
 * implementação do algoritimo Fisher-Yates
 * para embaralhamento de vetores de linhas
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

