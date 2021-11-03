
#include "lista_linhas.h"

#include <stdlib.h>

void inicia_lista_linhas(ListaLinhas* lista) {
	lista->base = NULL;
	lista->fim  = NULL;
}

void finaliza_lista_linhas(ListaLinhas* lista) {
	ListaLinhas_No* no = lista->base;
	while (no != NULL) {
		ListaLinhas_No* proximo = no->proximo;
		no->proximo = NULL;
		free((void*) no);
		no = proximo;
	}
	lista->base = NULL;
	lista->fim  = NULL;
}

ListaLinhas* criar_lista_linhas() {
	ListaLinhas* lista = malloc(sizeof(ListaLinhas));
	if (lista != NULL) inicia_lista_linhas(lista);
	return lista;
}

void deletar_lista_linhas(ListaLinhas* lista) {
	finaliza_lista_linhas(lista);
	free((void*) lista);
}

int inserir_lista_linhas(ListaLinhas_No** base, char* linha) {
	ListaLinhas_No* no = malloc(sizeof(ListaLinhas_No));
	if (no == NULL) return -1;

	no->proximo = *base;
	no->linha   = linha;

	*base = no;

	return 0;
}

int remover_lista_linhas(ListaLinhas_No** base) {
	ListaLinhas_No* no = *base;
	if (no == NULL) return -1;

	*base = no->proximo;
	no->proximo = NULL;
	free((void*) no);
	return 0;
}

int mover_lista_linhas(ListaLinhas_No** dest, ListaLinhas_No** orig) {
	if (dest == NULL || orig == NULL || *orig == NULL) return -1;

	ListaLinhas_No* no = *orig;

	// remoção do no *orig
	*orig = no->proximo;

	// inserção do no em *dest
	no->proximo = *dest;
	*dest = no;

	return 0;
}

int concat_lista_linhas(ListaLinhas* restrict dest, ListaLinhas* restrict orig) {
	if (dest == NULL || orig == NULL) return -1;

	// concatenação de lista vazia
	if (orig->base == NULL && orig->fim == NULL) return 0;

	if (dest->fim == NULL) {
		// transeferência simples
		dest->base = orig->base;
		dest->fim  = orig->fim;
	} else {
		// inserção no fim
		if (dest->fim->proximo != NULL) return -2;
		dest->fim->proximo = orig->base;
		dest->fim = orig->fim;
	}

	// 'limpa' a segunda lista
	orig->base = NULL;
	orig->fim = NULL;

	return 0;
}

