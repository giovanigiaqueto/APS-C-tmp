
#include "merge_sort.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Span {
	unsigned int indice;
	unsigned int compr;
} Span;

typedef struct Span_No {
	struct Span     span;
	struct Span_No* prox;
} Span_No;

typedef struct PilhaSpan {
	Span_No*     base;
	unsigned int compr;
} PilhaSpan;

int push(PilhaSpan* pilha, const Span* span) {
	if (pilha == NULL || span == NULL) return -1;

	Span_No* no = malloc(sizeof(Span_No));
	if (no == NULL) return -1;

	if (++pilha->compr == 0) {
		--pilha->compr;
		free((void*) no);
		return -1;
	}

	no->span = *span;
	no->prox = pilha->base;
	pilha->base = no;
	return 0;
}
int pop(PilhaSpan* pilha, Span* span) {
	if (pilha == NULL || span == NULL) return -1;
	if (pilha->base == NULL) return 0;

	Span_No* no = pilha->base;
	*span = no->span;

	pilha->base = no->prox;
	--pilha->compr;
	no->prox = NULL;
	free((void*) no);
	return 1;
}

static inline void troca(char** a, char** b) {
	char* tmp = *a;
	*a = *b;
	*b = tmp;
}

int merge_sort(char** linhas, unsigned int qtd_linhas) {

	int cod_err = 0;

	PilhaSpan pilha_fork = { NULL, 0 };
	PilhaSpan pilha_work = { NULL, 0 };

	Span span = { 0, qtd_linhas };
	if ((cod_err = push(&pilha_fork, &span))) return cod_err;

	char** buff = malloc((qtd_linhas / 2 + 1) * sizeof(char*));
	if (buff == NULL) return -1;

	while (pilha_fork.compr > 0 || pilha_work.compr > 0) {

		int cadeia = 0;
		if (pilha_fork.compr) {
			int res = pop(&pilha_fork, &span);
			if (res != 1) {
				if (res < 0) cod_err = res;
				break;
			}
			if (span.compr > 1) {
				while (span.compr > 1) {
					if ((cod_err = push(&pilha_work, &span))) break;
					unsigned int la = span.compr / 2;
					const Span span_tmp = { span.indice + la, span.compr - la };
					if ((cod_err = push(&pilha_fork, &span_tmp))) break;
					span.compr = la;
				}
				if (cod_err) break;
			} else cadeia = 1;
		}

		int contador = (cadeia ? pilha_work.compr - pilha_fork.compr : 1);
		for (; contador > 0; --contador) {
			int res = pop(&pilha_work, &span);
			if (res != 1) {
				if (res < 0) cod_err = res;
				break;
			}

			if (span.compr > 3) {
				unsigned int la = span.compr / 2;
				Span sub[2] = {
					{ span.indice, la },
					{ span.indice + la, span.compr - la }
				};

				// ========== merge ordenado ==========

				char** arr_esq = &linhas[sub[0].indice];
				char** arr_dir = &linhas[sub[1].indice];
				for (unsigned int i = 0; i < sub[1].compr; ++i) {
					buff[i] = arr_dir[i];
				}

				int i = sub[0].compr - 1;
				int j = sub[1].compr - 1;
				int k = i + j + 1;
				while (i >= 0 && j >= 0) {
					if (strcmp(arr_esq[i], buff[j]) > 0) {
						// array esquerda -> array ordenado
						troca(&arr_esq[k--], &arr_esq[i--]);
					} else {
						// array direita -> array ordenado
						troca(&arr_esq[k--], &buff[j--]);
					}
				}
				while (i >= 0) troca(&arr_esq[k--], &arr_esq[i--]);
				while (j >= 0) troca(&arr_esq[k--], &buff[j--]);
			}
		}
		if (cod_err) break;
	}

	// dealocação de memória e retorno
	while (pop(&pilha_work, &span) > 1) {}
	while (pop(&pilha_fork, &span) > 1) {}
	free((void*) buff);

	return cod_err;
}

const Algoritimo alg_merge_sort = {
	.nome = "Merge Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = merge_sort
};
