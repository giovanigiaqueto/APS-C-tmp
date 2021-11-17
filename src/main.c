
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "ler_arquivo.h"
#include "cmd_params.h"
#include "geracao_dados.h"
#include "algs_ordenacao.h"
#include "temporizacao.h"
#include "resultados.h"

const char* TEXTO_AJUDA = {
	"uso: benchmark [opcao...] [algoritimo...]\n"
	"faz o benchmark de todos os algoritimos\n"
	"ou dos algoritimos fornecidos, se isso nao\n"
	"for possivel essa mensagem de ajuda sera exibida\n"
	"\n"
	"exemplos:\n"
	"  benchmark -a (arquivo)          # benchmark dos algoritimos usando as linhas do arquivo\n"
	"  benchmark -g 'Merge Sort'       # benchmark do merge sort usando linhas aleatorias\n"
	"\n"
	"parametros:\n"
	"  --ajuda, --help, -h     mostra essa mensagen de ajuda\n"
	"  --repeticoes, -r        quantidade de vezes que cada algoritimo\n"
	"                          sera executado para que o tempo possa ser\n"
	"                          calculado, usando a media entre os tempos\n"
	"                          (pode dar resultados mais precisos, mas ainda\n"
	"                          esta sujeito a imprecisao por arredondamento),\n"
	"                          eh configurado como 100 por padrao\n"
	"  --embaralhar, -e        embaralha as linhas do arquivo antes\n"
	"                          de temporizar cada algoritimo, sem efeito\n"
	"                          se a opcao --arquivo-entrada nao for fornecida\n"
	"  --gerar-entrada, -g     gera as linhas a serem ordenadas\n"
	"                          de forma aleatoria e uniforme\n"
	"  --arquivo-entrada, -a   arquivo de entrada com as linhas a serem\n"
	"                          usadas nas ordenacoes, tem precedencia\n"
	"                          sobre o parametro --gerar-entrada\n"
	"  --qtd-linhas, -n        quantidade de linhas a serem usadas nas ordenacoes,\n"
	"                          pode ser um numero ou um arquivo contendo\n"
	"                          um numero por linha. se um arquivo for fornecido,\n"
	"                          linhas vazias ou somente com espacos serao ignoradas,\n"
	"                          a ordem das quantidades de linhas sera mantida.\n"
	"                          ao usar '-' essa opcao trata o valor seguinte\n"
	"                          como um nome de arquivo, mesmo que o nome dele\n"
	"                          seja um numero (deve haver espacos entre '-' e\n"
	"                          o numero). se nao for fornecido, 100 sera usado\n"
};

void ajuda() {
	printf("%s\n", TEXTO_AJUDA);
}

// não faz nada
char** mudar_dados_vazio(char** linhas, unsigned int qtd_linhas) {
	return linhas;
}

// gera os dados novamente
char** mudar_dados_gerar(char** linhas, unsigned int qtd_linhas) {
	return gerar_dados(qtd_linhas);
}

// embaralha os dados
char** mudar_dados_embaralhar(char** linhas, unsigned int qtd_linhas) {
	embaralhar_dados(linhas, qtd_linhas);
	return linhas;
}

// não faz nada
char** limpar_dados_vazio(char** linhas, unsigned int qtd_linhas) {
	return linhas;
}

// limpa os dados
char** limpar_dados_liberar(char** linhas, unsigned int qtd_linhas) {
	for (unsigned int i = 0; i < qtd_linhas; ++i) {
		free((void*) linhas[i]);
	}
	free((void*) linhas);
	return NULL;
}

int main(int argc, char* argv[]) {

	/* -------------- inicializacao -------------- */

	init_geracao_dados();

	/* ---------- leitura de parâmetros ---------- */

	Parametros params;
	int err = ler_parametros(argc, argv, &params);
	if (err != 0) {
		if ((err & ERRO_PARAM_AJUDA) == ERRO_PARAM_AJUDA) {
			ajuda();
		} else {
			fprintf(stderr, "erro na leitura dos parametros\n");
		}
		exit(EXIT_FAILURE);
	}


	/* ---------- pós processamento do parâmetro --entrada ---------- */

	unsigned int qtd_linhas_arquivo = 0;
	char** linhas_arquivo = NULL;
	char** linhas = NULL;
	char** (*mudar_dados)(char**, unsigned int);
	char** (*limpar_dados)(char**, unsigned int) = limpar_dados_vazio;
	if (params.entrada != NULL) {
		// lê o arquivo de entrada
		linhas_arquivo = ler_arquivo(params.entrada, &qtd_linhas_arquivo);
		linhas = linhas_arquivo;
		if (linhas_arquivo == NULL) {
			// erro de leitura
			fprintf(stderr, "erro: nao foi possivel ler o arquivo de entrada '%s'\n", params.entrada);
			exit(EXIT_FAILURE);
		} else if ((params.flags & PARAM_FLAG_QTD_LINHAS) == PARAM_FLAG_QTD_LINHAS) {
			// verifica se a quantidade de linhas no arquivo
			// é suficiente para todos os tamanhos de entrada.
			unsigned int max_linhas = 0;
			for (unsigned int i = 0; i < params.qtd_linhas_tamanho; ++i) {
				if (max_linhas < params.qtd_linhas[i]) {
					max_linhas = params.qtd_linhas[i];
				}
			}
			if (max_linhas < qtd_linhas_arquivo) {
				fprintf(stderr,
					"erro: o arquivo de entrada nao possui linhas o suficiente\n"
					"valor fornecido por --qtd-linhas requer um minimo de %u linhas\n",
					max_linhas);
				exit(EXIT_FAILURE);
			}
		}
		if ((params.flags & PARAM_FLAG_ENTRADA_EMBARALHADA) == PARAM_FLAG_ENTRADA_EMBARALHADA) {
			// embaralha a entrada do arquivo antes
			// de temporizar os algoritimos
			mudar_dados = mudar_dados_embaralhar;
		} else {
			// não altera a entrada do arquivo antes
			// de temporizar os algoritimos
			mudar_dados = mudar_dados_vazio;
		}
	} else if ((params.flags & PARAM_FLAG_ENTRADA_GERADA) == PARAM_FLAG_ENTRADA_GERADA) {
		// dados gerados aleatóriamente
		mudar_dados = mudar_dados_gerar;
		limpar_dados = limpar_dados_liberar;
	} else {
		// paramêtros faltando, mostra o texto de ajuda
		ajuda();
		exit(EXIT_FAILURE);
	}

	/* ---------- alocação para os resultados ---------- */

	// cria o vetor de resultados
	ResultadoAlgoritimo* resultados;
	unsigned int         qtd_resultados;
	if (params.algoritimos == NULL) {
		// cria o vetor de resultados
		qtd_resultados = NUM_ALGORITIMOS;
		resultados = criar_vetor_resultados(\
				LISTA_ALGORITIMOS, qtd_resultados, params.qtd_linhas_tamanho);
	} else {
		qtd_resultados = params.algoritimos_tamanho;
		resultados = criar_vetor_resultados(\
				params.algoritimos, params.algoritimos_tamanho, params.qtd_linhas_tamanho);
	}
	if (resultados == NULL) {
		fprintf(stderr, "erro: sem memoria\n");
		exit(EXIT_FAILURE);
	}


	/* --------------- temporização --------------- */

	for (unsigned int i = 0; i < params.qtd_linhas_tamanho; ++i) {
		unsigned int qtd_linhas = params.qtd_linhas[i];
		if (qtd_linhas == 0) {
			fprintf(stderr,
				"erro: a quantidade de linhas eh zero, parando\n"
			);
			break;
		}
		for (unsigned int j = 0; j < qtd_resultados; ++j) {
			ResultadoAlgoritimo* res = &resultados[j];
			tempo_t tempo = 0;
			if (res->erro != 0) continue;
			for (unsigned int k = 0; k < params.repeticoes; ++k) {
				linhas = mudar_dados(linhas, qtd_linhas);
				tempo_t tempo_frac;
				int err = temporizar(linhas, qtd_linhas, res->algoritimo, &tempo_frac, &res->erro);
				if (err != 0) {
					fprintf(stderr,
						"aviso: a temporizacao do algoritimo '%s' falhou, ignorando\n"
						"cod. erro da funcao de temporizacao:  %d\n"
						"cod. erro do algoritimo de ordenacao: %d\n",
						res->algoritimo->nome, err, res->erro);
					break;
				}
				linhas = limpar_dados(linhas, qtd_linhas);
				tempo += tempo_frac;
			}
			if (res->erro != 0) continue;
			res->tempos[i] = tempo / (tempo_t) params.repeticoes;
		}
	}

	for (unsigned int i = 0; i < params.qtd_linhas_tamanho; ++i) {
		printf("qtd linhas: %u\n\n", params.qtd_linhas[i]);
		for (unsigned int j = 0; j < qtd_resultados; ++j) {
			printf("%24s : %4fs\n", resultados[j].algoritimo->nome, resultados[j].tempos[i]);
		}
		printf("\n");
	}

	/* ---------- liberação de memória ---------- */

	free((void*) linhas_arquivo);
	if (linhas != linhas_arquivo) {
		free((void*) linhas);
	}
	qtd_linhas_arquivo = 0;
	linhas_arquivo = NULL;
	linhas = NULL;

	destruir_vetor_resultados(resultados, qtd_resultados);

	free((void*) params.algoritimos);
	free((void*) params.qtd_linhas);

	return 0;
}
