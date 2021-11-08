#ifndef __GERACAO_DADOS_H
#define __GERACAO_DADOS_H

void init_geracao_dados();

typedef struct ConfGeracaoDados {
	unsigned int tamanho_linha_min;
	unsigned int tamanho_linha_max;

	const char*  alfabeto;
	unsigned int tamanho_alfabeto;
} ConfGeracaoDados;

int validar_conf_geracao_dados(const ConfGeracaoDados* conf);

const ConfGeracaoDados* set_conf_geracao_dados(const ConfGeracaoDados* conf);
const ConfGeracaoDados* get_conf_geracao_dados();

void embaralhar_dados(char** linhas, unsigned int qtd_linhas);
char** gerar_dados(unsigned int qtd_linhas);

#endif
