
#include "bucket_sort.h"
#include "insertion_sort.h"

#include <stdlib.h>
#include <string.h>

// constantes
#define TAMANHO_BALDE 100
#define TAMANHO_CHAVE 2

// estrutura usada como
// balde para o algoritimo
typedef struct Balde {
	ListaLinhas  linhas;
	unsigned int qtd_linhas;
} Balde;

/**
 * calcula o comprimento da chave até o tamanho
 * máximo permitido para uma chave (retorna no
 * máximo TAMANHO_CHAVE)
 *
 * AVISO: resultado indefinido se 'chave' for NULL
 */
unsigned int compr_chave(char* chave) {
	unsigned int compr = 0;
	for (; compr < TAMANHO_CHAVE; ++compr) {
		if (chave[compr] == '\00') break;
	}
	return compr;
}

/**
 * preenche a chave 'orig' com \NUL no final, copiando-a para o vetor 'dest'
 *
 * AVISO: resultado indefinido se 'orig' ou 'dest' forem NULL
 */
unsigned int zfill_chave(char dest[TAMANHO_CHAVE], const char* orig) {
	// copia os caracteres existentes
	unsigned int idx = 0;
	for (; idx < TAMANHO_CHAVE; ++idx) {
		if (orig[idx] == '\0') break;
		dest[idx] = orig[idx];
	}
	// preenche com NUL (ASCII \0)
	for (unsigned int i = idx; i < TAMANHO_CHAVE; ++i) {
		dest[i] = '\0';
	}
	// retorna o numero de caracteres preenchidos
	return TAMANHO_CHAVE - idx;
}

/**
 * retorna a distância de uma chave até a chave zero
 * (\00, \00, \00 ... TAMANHO_CHAVE vezes), usada
 * para converter para um número
 *
 * AVISO: resultado indefinido quando 'chs' é NULL
 */
unsigned int chave_para_uint(const char chave[TAMANHO_CHAVE]) {
	unsigned int uint_v = 0;
	for (unsigned int i = 0; i < TAMANHO_CHAVE; ++i) {
		uint_v <<= 8;
		uint_v |= (unsigned char) chave[i];
	}
	return uint_v;
}

/**
 * converte a distância de uma chave até a chave zero
 * para a chave original.
 *
 * AVISO: resultado indefinido se 'chave' for NULL
 */
void uint_para_chave(char chave[TAMANHO_CHAVE], unsigned int dist) {
	for (unsigned int i = TAMANHO_CHAVE - 1; i > 0; --i) {
		chave[i] = (unsigned char) (dist & 255u);
		dist >>= 8;
	}
}

/**
 * algoritimo de ordenação bucket sort
 *
 * @param linhas as linhas a serem ordenadas
 * @param qtd_linhas a quantidade de linhas fornecidas
 *
 * @returns codigo de erro diferente de zero (se existir)
 *
 * AVISO: o uso de goto é intencional (liberação de memória
 *   de forma centralizada, para reduzir o tamanho do código
 *   e simplificar as partes do código que param a execução
 *   caso um erro seja encontrado)
 */
int bucket_sort(ListaLinhas* linhas, unsigned int qtd_linhas) {

	// codigo de erro retornado
	int codigo_erro = 0;

	// procura o maior e menor valor da lista de linhas
	char* maior = NULL;
	char* menor = NULL;
	for (ListaLinhas_No* no = linhas->base; no != NULL; no = no->proximo) {
		if (maior == NULL || strcmp(no->linha, maior) > 0) maior = no->linha;
		if (menor == NULL || strcmp(no->linha, menor) < 0) menor = no->linha;
	}

	// evita erros catastróficos (e.g. seg. fault)
	if (maior == NULL || menor == NULL) return -1;
	if (maior[0] == 0 || menor[0] == 0) return -1;

	// extrai as chaves da maior e menor linha
	unsigned int chave_menor;
	unsigned int chave_maior;
	{
		char zfill_tmp[TAMANHO_CHAVE];

		// chave menor
		zfill_chave(zfill_tmp, menor);
		chave_menor = chave_para_uint(zfill_tmp);

		// chave maior
		zfill_chave(zfill_tmp, maior);
		chave_maior = chave_para_uint(zfill_tmp);
	}

	// calcula o número de baldes e a maior diferença entre chaves
	unsigned int chave_max_diff = chave_maior - chave_menor;
	unsigned int qtd_baldes     = (chave_max_diff / TAMANHO_BALDE) + 1;

	// cria o número de baldes necessarios
	Balde* baldes = malloc(qtd_baldes * sizeof(Balde));
	if (baldes == NULL) return -1;

	// inicializa os baldes
	for (unsigned int i = 0; i < qtd_baldes; ++i) {
		inicia_lista_linhas(&baldes[i].linhas);
		baldes[i].qtd_linhas = 0;
	}

	// insere as linhas nos baldes
	ListaLinhas_No** base_ptr = &(linhas->base);
	char zfill_tmp[TAMANHO_CHAVE];
	while (linhas->base != NULL) {
		// calcula a chave da linha
		zfill_chave(zfill_tmp, (*base_ptr)->linha);
		unsigned int chave = chave_para_uint(zfill_tmp);

		// move a linha para o balde
		Balde* balde_ptr = &baldes[(chave - chave_menor) / TAMANHO_BALDE];
		int err = mover_lista_linhas(&(balde_ptr->linhas.base), base_ptr);
		if (err != 0) {
			codigo_erro = err;
			goto dealoc_baldes;
		}

		++(balde_ptr->qtd_linhas);
	}

	// ordena os baldes
	for (unsigned int i = 0; i < qtd_baldes; ++i) {
		int err = insertion_sort(&baldes[i].linhas, baldes[i].qtd_linhas);
		if (err != 0) {
			codigo_erro = err;
			goto dealoc_baldes;
		}
		// configura o fim da lista do balde
		if (baldes[i].qtd_linhas > 0) {
			ListaLinhas_No* iter;
			for (iter = baldes[i].linhas.base; iter->proximo != NULL; iter = iter->proximo) {}
			baldes[i].linhas.fim = iter;
		}
	}

	// juntar os baldes
	for (unsigned int i = 0; i < qtd_baldes; ++i) {
		int err = concat_lista_linhas(linhas, &baldes[i].linhas);
		if (err != 0) {
			codigo_erro = err;
			goto dealoc_baldes;
		}
	}

dealoc_baldes:
	// liberação de memória [1/2] (baldes individuais)
	for (unsigned int i = 0; i < qtd_baldes; ++i) {
		finaliza_lista_linhas(&baldes[i].linhas);
	}

	// liberação de memória [2/2] (vetor de baldes)
	free((void*) baldes);
	baldes = NULL;

	// retorno do código de erro (se existir)
	return codigo_erro;
}

// informações sobre o algoritimo para
// execução pelo módulo de temporização
Algoritimo alg_bucket_sort = { ALG_LISTA, .func.alg_lista = bucket_sort };

