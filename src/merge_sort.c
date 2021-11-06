
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

typedef struct ListaSpan {
	Span_No* base;
	Span_No* fim;
} ListaSpan;

int push_pilha(ListaSpan* lista, Span* span) {
	if (lista == NULL || span == NULL) return -1;

	Span_No* no = malloc(sizeof(Span_No));
	if (no == NULL) return -1;

	no->span = *span;
	no->prox = lista->base;
	if (lista->base == NULL) {
		lista->fim = no;
	}
	lista->base = no;
	return 0;
}
int push_fila(ListaSpan* lista, Span* span) {
	if (lista == NULL || span == NULL) return -1;

	Span_No* no = malloc(sizeof(Span_No));
	if (no == NULL) return -1;

	no->span = *span;
	no->prox = NULL;

	if (lista->fim == NULL) {
		lista->base = no;
		lista->fim = no;
	} else {
		lista->fim->prox = no;
		lista->fim = no;
	}

	return 0;
}
int pop(ListaSpan* lista, Span* span) {
	if (lista == NULL || span == NULL) return -1;
	if (lista->base == NULL) return 0;

	Span_No* no = lista->base;
	*span = no->span;

	if (no->prox == NULL) {
		lista->fim = NULL;
	}
	lista->base = no->prox;
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

	ListaSpan fila_span  = { NULL, NULL };
	ListaSpan pilha_span = { NULL, NULL };

	Span span = { 0, qtd_linhas };
	if ((cod_err = push_fila(&fila_span, &span)))   return cod_err;
	if ((cod_err = push_pilha(&pilha_span, &span))) return cod_err;

	while (1) {
		int res = pop(&fila_span, &span);
		if (res != 1) {
			if (res < 0) cod_err = res;
			break;
		}

		unsigned int compr_head = span.compr / 2;
		unsigned int compr_tail = span.compr - compr_head;
		if (compr_head > 1 && compr_tail > 1) {
			span.compr = compr_head;
			if ((cod_err = push_pilha(&pilha_span, &span))) {
				break;
			}
			if (compr_head > 3) {
				if ((cod_err = push_fila(&fila_span, &span))) {
					break;
				}
			}
			span.indice += compr_head;
			span.compr = compr_tail;
			if ((cod_err = push_pilha(&pilha_span, &span))) {
				break;
			}
			if (compr_tail > 3) {
				if ((cod_err = push_fila(&fila_span, &span))) {
					break;
				}
			}
		}
	}

	Span sub[2];
	while (1) {
		int res = pop(&pilha_span, &span);
		if (res != 1) {
			if (res < 0) cod_err = res;
			break;
		}

		if (span.compr > 3) {
			res = pop(&fila_span, &sub[1]);
			if (res != 1) {
				if (res < 0) cod_err = res;
				break;
			}
			res = pop(&fila_span, &sub[0]);
			if (res != 1) {
				if (res < 0) cod_err = res;
				break;
			}

			// checagem de erro
			if (span.indice != sub[0].indice ||
					sub[0].indice + sub[0].compr != sub[1].indice ||
					sub[0].compr  + sub[1].compr != span.compr) {
				cod_err = -42;
				break;
			}

			// ========== merge ordenado ==========

			char** arr_esq = &linhas[sub[0].indice];
			char** arr_dir = &linhas[sub[1].indice];
			char** buff = malloc(sub[1].compr * sizeof(char*));
			if (buff == NULL) {
				// NOTA: ERRO DE ALOCACAO
				cod_err = -1;
				break;
			}
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
			free((void*) buff);

			if ((cod_err = push_fila(&fila_span, &span))) {
				break;
			}

		} else {
			// ordenação de 2 ou 3 elementos
			unsigned int i = span.indice;
			if (span.compr == 2) {
				if (strcmp(linhas[i], linhas[i+1]) > 0) {
					troca(&linhas[i], &linhas[i+1]);
				}
			} else {
				if (strcmp(linhas[i], linhas[i+1]) > 0) {
					// a > b
					if (strcmp(linhas[i+1], linhas[i+2]) > 0) {
						// a > b > c, (a,b,c) -> (c,b,a)
						char* tmp   = linhas[i];
						linhas[i]   = linhas[i+2];
						linhas[i+2] = tmp;
					} else if (strcmp(linhas[i+2], linhas[i]) > 0){
						// c > a > b, (a,b,c) -> (b,a,c)
						char* tmp   = linhas[i];
						linhas[i]   = linhas[i+1];
						linhas[i+1] = tmp;
					} else {
						// a > b == c, (a,b,c) -> (b,c,a)
						char* tmp   = linhas[i];
						linhas[i]   = linhas[i+1];
						linhas[i+1] = linhas[i+2];
						linhas[i+2] = tmp;
					}
				} else if (strcmp(linhas[i], linhas[i+2]) > 0) {
					// c < a <= b, (a,b,c) -> (c,a,b)
					char* tmp = linhas[i];
					linhas[i]   = linhas[i+2];
					linhas[i+2] = linhas[i+1];
					linhas[i+1] = tmp;
				} else if (strcmp(linhas[i+1], linhas[i+2]) > 0) {
					// a <= c < b, (a,b,c) -> (a,c,b)
					char* tmp   = linhas[i+1];
					linhas[i+1] = linhas[i+2];
					linhas[i+2] = tmp;
				}
			}

			if ((cod_err = push_fila(&fila_span, &span))) {
				break;
			}
		}
	}

	// dealocação de memória e retorno
	while (pop(&fila_span, &span)  > 1) {}
	while (pop(&pilha_span, &span) > 1) {}
	return cod_err;
}

const Algoritimo alg_merge_sort = {
	.nome = "Merge Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = merge_sort
};
