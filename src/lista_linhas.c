
#include "lista_linhas.h"

#include <stdlib.h>

ListaLinhas* criar_lista_linhas() {
	ListaLinhas* lista = malloc(sizeof(ListaLinhas));
	if (lista != NULL) {
		lista->base = NULL;
	}
	return lista;
}

void deletar_lista_linhas(ListaLinhas* lista) {
	ListaLinhas_No* no = lista->base;
	while (no != NULL) {
		ListaLinhas* proximo = no->proximo;
		no->proximo = NULL;
		free((void*) no);
		no = proximo;
	}
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
	free((void*) no);
	return 0;
}

int mover_lista_linhas(ListaLinhas_No** dest, ListaLinhas_No** orig) {
	if (dest == NULL || *dest == NULL) return -1;
	if (orig == NULL || *orig == NULL) return -1;

	ListaLinhas_No* no = *orig;

	// remoção do no *orig
	*orig = no->proximo;

	// inserção do no em *dest
	no->proximo = *dest;
	*dest = no;

	return 0;
}