
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "ler_arquivo.h"
#include "cmd_params.h"
#include "algs_ordenacao.h"

int linha_isspace(const char* linha) {
	for (; *linha != '\0'; linha = &linha[1]) {
		if (!isspace((unsigned char) *linha)) return 0;
	}
	return 1;
}

typedef struct ParametrosBruto {
	unsigned int repeticoes;
	unsigned int qtd_linhas;
	const char*  qtd_linhas_arquivo;
	const char*  entrada;

	const Algoritimo** algoritimos;
	unsigned int       algoritimos_tamanho;

	unsigned int flags;

} ParametrosBruto;

/**
 * converte string para long.
 *
 * se a conversão for possível retorna 1, se o texto
 * é vazio ou composto somente por espaços retorna 0,
 * em qualquer outro caso retorna -1.
 */
int str_para_long(const char* str, long* val) {
	if (str == NULL || val == NULL) return -1;

	char* end;
	*val = strtol(str, &end, 10);

	// ignora linhas vazias ou só com espaços
	if (end == str || linha_isspace(str) != 0) return 0;

	// verifica se a linha é um número válido
	if (linha_isspace(end) == 0) return -1;

	// sucesso
	return 1;
}

int ler_parametros_bruto(int argc, char* argv[], ParametrosBruto* params) {

	params->repeticoes = 100;
	params->qtd_linhas = 100;
	params->qtd_linhas_arquivo = NULL;
	params->entrada    = NULL;

	params->algoritimos = NULL;
	params->algoritimos_tamanho = 0;

	params->flags  = 0;

	/* ---------- leitura de parâmetros ---------- */

	int erro = ERRO_SEM_ERRO;
	char* parametro = NULL;
	int curr_arg;
	for (curr_arg = 1; curr_arg < argc; ++curr_arg) {
		parametro = argv[curr_arg];

		if (parametro[0] == '-') {
			/* ----- opções longas e curtas ----- */
			const char* opcao = &parametro[1];
			if (strcmp(opcao, "h") == 0 || strcmp(opcao, "-ajuda") == 0 || strcmp(opcao, "-help") == 0) {
				erro |= ERRO_PARAM_AJUDA;
				curr_arg = argc;
				break;
			} else if (strcmp(opcao, "r") == 0 || strcmp(opcao, "-repeticoes") == 0) {
				if (++curr_arg >= argc) {
					erro |= ERRO_PARAM_FALTANDO | ERRO_PARAM_REPETICOES;
					break;
				}

				long valor;
				if (str_para_long(argv[curr_arg], &valor) != 1) {
					fprintf(stderr, "erro: valor invalido para o parametro --repeticoes\n");
					erro |= ERRO_PARAM_INVALIDO | ERRO_PARAM_REPETICOES;
					break;
				}

				if (valor <= 0) {
					fprintf(stderr, "erro: o numero de repeticoes deve ser positivo e diferente de zero\n");
					erro |= ERRO_PARAM_INVALIDO | ERRO_PARAM_REPETICOES;
					break;
				} else if (valor > NUM_REPETICOES_MAX) {
					fprintf(stderr, "erro: numero de repeticoes excede o limite maximo\n");
					erro |= ERRO_PARAM_INVALIDO | ERRO_PARAM_REPETICOES;
					break;
				} else {
					params->repeticoes = (unsigned int) valor;
					params->flags |= PARAM_FLAG_REPETICOES;
				}
			} else if (strcmp(opcao, "e") == 0 || strcmp(opcao, "-embaralhar") == 0) {
				params->flags |= PARAM_FLAG_ENTRADA_EMBARALHADA;
			} else if (strcmp(opcao, "g") == 0 || strcmp(opcao, "-gerar-entrada") == 0) {
				params->flags |= PARAM_FLAG_ENTRADA_GERADA;
			} else if (strcmp(opcao, "a") == 0 || strcmp(opcao, "-arquivo-entrada") == 0) {
				if (++curr_arg >= argc) { erro = ERRO_PARAM_FALTANDO; break; }
				params->entrada = argv[curr_arg];
			} else if (strcmp(opcao, "n") == 0 || strcmp(opcao, "-qtd-linhas") == 0) {
				if (++curr_arg >= argc) {
					erro |= ERRO_PARAM_FALTANDO | ERRO_PARAM_QTD_LINHAS;
					break;
				}
				if (strcmp(argv[curr_arg], "-") == 0) {
					if (++curr_arg >= argc) {
						erro |= ERRO_PARAM_FALTANDO | ERRO_PARAM_QTD_LINHAS;
						break;
					}
					params->qtd_linhas_arquivo = argv[curr_arg];
					params->flags |= PARAM_FLAG_QTD_LINHAS;
				} else {
					long valor;
					int err = str_para_long(argv[curr_arg], &valor);
					if (err == -1) {
						params->qtd_linhas_arquivo = argv[curr_arg];
						params->flags |= PARAM_FLAG_QTD_LINHAS;
					} else if (err != 1) {
						fprintf(stderr, "erro: valor invalido para o paramentro --qtd-linhas\n");
					} else if (valor <= 0) {
						fprintf(stderr, "erro: a quantidade de linhas deve ser positiva e diferente de zero\n");
						erro |= ERRO_PARAM_INVALIDO | ERRO_PARAM_QTD_LINHAS;
						break;
					} else if (valor > QTD_LINHAS_MAX) {
						fprintf(stderr, "erro: a quantidade de linhas excede o limite maximo\n");
						erro |= ERRO_PARAM_INVALIDO | ERRO_PARAM_QTD_LINHAS;
						break;
					} else {
						params->qtd_linhas = (unsigned int) valor;
						params->flags |= PARAM_FLAG_QTD_LINHAS;
					}
				}
			} else if (strcmp(opcao, "-") == 0) {
				++curr_arg;
				break;
			} else {
				fprintf(stderr, "erro: parametro desconhecido '%s'\n", parametro);
			}
		} else break; // nome de algoritimo
	}
	if (curr_arg < argc) {
		// previne o processamento de mais algoritimos
		// que a quantidade conhecida.
		if (argc - curr_arg > NUM_ALGORITIMOS) {
			fprintf(stderr, "erro: numero de algoritimos passados excede o numero de algoritimos conhecidos\n");
			return erro | ERRO_PARAM_ALGORITIMOS;
		}

		// inicializa o buffer
		unsigned int tamanho_buffer = argc - curr_arg;
		const Algoritimo** algoritimos = malloc(tamanho_buffer * sizeof(Algoritimo));
		if (algoritimos == NULL) {
			fprintf(stderr, "erro: sem memoria\n");
			return erro | ERRO_PARAM_ALGORITIMOS | ERRO_SEM_MEMORIA;
		}

		// procura os algoritimos por nome e preenche o buffer
		unsigned int qtd_algoritimos;
		for (qtd_algoritimos = 0; curr_arg < argc; ++qtd_algoritimos, ++curr_arg) {
			// procura o algoritimo
			const Algoritimo* alg = procurar_algoritimo(argv[curr_arg]);
			if (alg == NULL) {
				fprintf(stderr, "erro: algoritimos desconhecido '%s'\n", argv[curr_arg]);
				for (unsigned int j = 0; j < qtd_algoritimos; ++j) {
					algoritimos[j] = NULL;
				}
				free((void*) algoritimos);
				return erro | ERRO_PARAM_ALGORITIMOS | ERRO_PARAM_INVALIDO;
			}
			algoritimos[qtd_algoritimos] = alg;
		}

		// verifica duplicados usando uma hash table (complexidade O(n))
		// NOTAS:
		//   a quantidade de algoritimos não deve exceder 100,
		//   então pouca memória contigua será alocada (< 1KiB).
		//
		//   o hash será de 4 bits e usará adição dos nibles na orden
		//   LSB para MSB com rotação incremental dos bits modulo 4
		//   para a direita (nible 1 rol 1, nible 2 rol 2, nible 3 rol 3,
		//   ... nible 5 rol 1, nible 6 rol 2), o carry da adição será
		//   adicionado ao hash até ele ser zero
		unsigned int i;
		const Algoritimo** table = (const Algoritimo**) malloc(16 * sizeof(Algoritimo*));
		if (table == NULL) {
			fprintf(stderr, "erro: sem memoria\n");
			return erro | ERRO_SEM_MEMORIA;
		}
		for (i = 0; i < 16; ++i) table[i] = NULL;
		for (i = 0; i < qtd_algoritimos; ++i) {
			// hash
			unsigned int chave = 0;
			unsigned long ptr = (unsigned long) (void*) algoritimos[i];
			for (unsigned int j = 0; ptr != 0; ++j, ptr >>= 4) {
				// left shift 4
				unsigned char nible = (ptr & 15) << (j % 4);
				// left rotate usando o shift anterior
				chave += (nible & 15) | (nible >> 4);
				// carry da adição (como o valor máximo é
				// 0Fh + 0Fh = 1Eh, não é necessário um loop
				// pois o carry da adição nesse caso será 0Fh)
				chave = (chave & 15) + (chave >> 4);
			}

			// verifica se há duplicados, ou salva esse ponteiro
			if (table[chave] != NULL) break;
			table[chave] = algoritimos[i];
		}
		free((void*) table);
		if (i != qtd_algoritimos) {
			// duplicado encontrado
			free((void*) algoritimos);
			fprintf(stderr, "erro: algoritimos duplicados fornecido na posicao %u\n", i);
			return erro | ERRO_PARAM_ALGORITIMOS | ERRO_PARAM_INVALIDO;
		}

		params->algoritimos = algoritimos;
		params->algoritimos_tamanho = qtd_algoritimos;
	}

	/* ---------- checagem de erro ---------- */

	switch (erro & ERRO_PARAM_TIPO_MASK) {
		case ERRO_PARAM_FALTANDO:
			fprintf(stderr, "faltando valor para o parametro '%s'\n", parametro);
			return erro;
		case ERRO_PARAM_INVALIDO:
			fprintf(stderr, "valor invalido para o parametro '%s'\n", parametro);
			return erro;
		case ERRO_PARAM_AJUDA:
			return erro;
		case ERRO_SEM_ERRO:
			break;
		default:
			fprintf(stderr, "erro desconhecido ao processar parametros de linha de comando, cod err: %d\n", erro);
			return erro;
	}

	return erro;
}

int ler_parametros(int argc, char* argv[], Parametros* params) {

	/* ---------- leitura de parâmetros ---------- */

	ParametrosBruto params_bruto;
	int err = ler_parametros_bruto(argc, argv, &params_bruto);
	if (err != 0) return err;

	params->entrada    = params_bruto.entrada;
	params->repeticoes = params_bruto.repeticoes;
	params->algoritimos = params_bruto.algoritimos;
	params->algoritimos_tamanho = params_bruto.algoritimos_tamanho;
	params->flags = params_bruto.flags;

	/* ---------- parâmetro --qtd-linhas ---------- */

	if (params_bruto.qtd_linhas_arquivo == NULL) {
		// ----- qtd-linhas como um número -----
		params->qtd_linhas_tamanho = 1;
		params->qtd_linhas = malloc(1 * sizeof(unsigned int));
		if (params->qtd_linhas == NULL) {
			fprintf(stderr, "erro: sem memoria\n");
			return err | ERRO_SEM_MEMORIA;
		}
		params->qtd_linhas[0] = params_bruto.qtd_linhas;
	} else {
		// ----- qtd-linhas como arquivo -----
		unsigned int* qtd_linhas_vetor;
		unsigned int qtd_linhas_tamanho;
		unsigned int tamanho;
		char** linhas = ler_arquivo(params_bruto.qtd_linhas_arquivo, &tamanho);
		if (linhas == NULL) {
			fprintf(stderr, "erro: --qtd-linhas, falha ao ler o arquivo '%s'\n", params_bruto.qtd_linhas_arquivo);
			return err | ERRO_LEITURA_ARQUIVO;
		}

		// inicializa o buffer
		qtd_linhas_tamanho = 1;
		qtd_linhas_vetor = malloc(qtd_linhas_tamanho * sizeof(unsigned int));
		if (qtd_linhas_vetor == NULL) {
			fprintf(stderr, "erro: sem memoria\n");
			return err | ERRO_PARAM_QTD_LINHAS | ERRO_SEM_MEMORIA;
		}

		// converte cada linha em um número se possível,
		// ignorando linhas vazias ou só com espaços
		unsigned int contador = 0;
		for (unsigned int i = 0; i < tamanho; ++i) {
			long valor;
			int res = str_para_long(linhas[i], &valor);

			// ignora linhas vazias ou só com espaços,
			// verifica se a linha é um número
			if (res == 0) continue;
			if (res == -1) {
				if (linha_isspace(linhas[i]) == 0) {
					fprintf(stderr,
						"erro: --qtd-linhas, arquivo com dados invalidos '%s'\n"
						"linha %d: nao eh numero, linha em branco ou linha vazia\n",
						params_bruto.qtd_linhas_arquivo, i);
					return err | ERRO_PARAM_QTD_LINHAS;
				} else continue;
			}

			// verifica se o número é válido
			if (valor <= 0) {
				fprintf(stderr,
					"erro: --qtd-linhas, arquivo com dados invalidos '%s'\n"
					"linha %d: numero negativo ou zero encontrado\n",
					params_bruto.qtd_linhas_arquivo, i);
				return err | ERRO_PARAM_QTD_LINHAS;
			}
			if (valor > QTD_LINHAS_MAX) {
				fprintf(stderr,
					"erro: --qtd-linhas, arquivo com dados invalidos '%s'\n"
					"linha %d: o valor excede o limite maximo de %d\n",
					params_bruto.qtd_linhas_arquivo, i, QTD_LINHAS_MAX);
				return err | ERRO_PARAM_QTD_LINHAS;
			}

			// previne o excesso de linhas
			if (contador + 1 > QTD_LINHAS_MAX) {
				fprintf(stderr, "erro: --qtd-linhas, a quantidade de linhas excede o limite\n");
				return err | ERRO_PARAM_QTD_LINHAS;
			}

			// aloca mais memoria para o buffer
			if (contador + 1 > qtd_linhas_tamanho) {
				// previne erros relacionados a overflow
				if (qtd_linhas_tamanho >= UINT_MAX / 2 - 1) {
					fprintf(stderr, "erro: falhando alocacao devido a possivel unsigned overflow do comprimento\n");
					return err | ERRO_PARAM_QTD_LINHAS | ERRO_ARRAY_DYN_ALLOC;
				}

				// aloca o dobro de memoria que o atual (faz com que a inserção seja no pior dos casos O(log(n)))
				qtd_linhas_tamanho *= 2;
				unsigned int* buff = malloc(qtd_linhas_tamanho * sizeof(unsigned int));
				if (buff == NULL) {
					fprintf(stderr, "erro: sem memoria\n");
					return err | ERRO_PARAM_QTD_LINHAS | ERRO_SEM_MEMORIA;
				}

				// copia o conteúdo e libera o buffer antigo
				memcpy(buff, qtd_linhas_vetor, qtd_linhas_tamanho * sizeof(unsigned int));
				unsigned int* tmp = qtd_linhas_vetor;
				qtd_linhas_vetor = buff;
				free((void*) tmp);
			}

			// seta o valor lido
			qtd_linhas_vetor[contador++] = (unsigned int) valor;
		}

		// libera o buffer do arquivo e realoca memória para
		// liberar espaço não usado no buffer de quantidade de linhas
		free((void*) linhas);
		params->qtd_linhas = realloc(qtd_linhas_vetor, contador * sizeof(unsigned int));
		params->qtd_linhas_tamanho = contador;
	}

	// retorna o codigo de erro (se tiver)
	return err;
}
