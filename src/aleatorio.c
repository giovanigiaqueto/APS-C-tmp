
#include <time.h>
#include <string.h>

#include "aleatorio.h"

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
void embaralhar(void* buff, size_t tamanho, size_t tamanho_buff) {
	if (buff == NULL || tamanho <= 1 || tamanho_buff <= 1) return;

	void* saved = malloc(tamanho);
	if (saved == NULL) return;

	for (unsigned int i = 0; i < tamanho_buff - 1; ++i) {
		unsigned int idx = gerar_randint(i, tamanho_buff);
		// basicamente um memswap, se existisse
		if (idx == i) continue;
		memcpy(saved, buff + i * tamanho, tamanho);
		memcpy(buff + i * tamanho, buff + idx * tamanho, tamanho);
		memcpy(buff + idx * tamanho, saved, tamanho);
	}

	free(saved);
}

int iniciado = 0;
void inic_aleatorio() {
	if (!iniciado) {
		srand(time(NULL));
		iniciado = 1;
	}
}
