
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
	ListaLinhas* no = lista->base;
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

	no->proximo = (*base)->proximo;
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
