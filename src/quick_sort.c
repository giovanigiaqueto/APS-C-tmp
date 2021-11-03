
#include "quick_sort.h"

const Algoritimo alg_quick_sort = {
	.nome = "Quick Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = ordenar_quick_sort
};

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct ValorPilha {
	unsigned int       esquerda;
	unsigned int       direita;
	struct ValorPilha* proximo;
};
typedef struct ValorPilha ValorPilha;

typedef struct {
	struct ValorPilha* valores;
} Pilha;

int __ordenar_quick_sort(char** linhas, unsigned int esquerda, unsigned int direita) {
	Pilha pilha;

	ValorPilha* _valor = malloc(sizeof(ValorPilha));
	if (_valor == NULL) return -1;

	_valor->esquerda = esquerda;
	_valor->direita  = direita;
	_valor->proximo  = NULL;

	pilha.valores = _valor;

	while (pilha.valores) {
		ValorPilha* valor = pilha.valores;

		esquerda = valor->esquerda;
		direita  = valor->direita;

		pilha.valores = valor->proximo;
		free((void*) valor);

		char* x = linhas[(esquerda + direita) / 2];

		unsigned int i = esquerda;
		unsigned int j = direita;

		while (i <= j) {
			while (strcmp(linhas[i], x) < 0 && i < direita) {
				i++;
			}
			while (strcmp(linhas[j], x) > 0 && j > esquerda) {
				j--;
			}
			if (i <= j) {
				char* tmp = linhas[i];
				linhas[i] = linhas[j];
				linhas[j] = tmp;
				i++;
				j--;
			}
		}

		if (j > esquerda) {
			valor = malloc(sizeof(ValorPilha));
			if (valor == NULL) {
				valor = pilha.valores;
				while (valor != NULL) {
					ValorPilha* proximo = valor->proximo;
					free((void*) valor);
					valor = proximo;
				}

				return -1;
			}

			valor->esquerda = esquerda;
			valor->direita  = j;
			valor->proximo  = pilha.valores;

			pilha.valores = valor;
		}
		if (i < direita) {
			valor = malloc(sizeof(ValorPilha));
			if (valor == NULL) {
				valor = pilha.valores;
				while (valor != NULL) {
					ValorPilha* proximo = valor->proximo;
					free((void*) valor);
					valor = proximo;
				}

				return -1;
			}

			valor->esquerda = i;
			valor->direita  = direita;
			valor->proximo  = pilha.valores;

			pilha.valores = valor;
		}
	}

	return 0;
}

int ordenar_quick_sort(char** linhas, unsigned int qtd_linhas) {
	return __ordenar_quick_sort(linhas, 0, qtd_linhas - 1);
}
