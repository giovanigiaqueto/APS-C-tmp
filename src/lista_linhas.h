#ifndef __LISTA_LINHAS_H
#define __LISTA_LINHAS_H

typedef struct ListaLinhas_No {
	char*                  linha;
	struct ListaLinhas_No* proximo;
} ListaLinhas_No;

typedef struct ListaLinhas {
	ListaLinhas_No* base;
} ListaLinhas;

ListaLinhas* criar_lista_linhas();

void deletar_lista_linhas(ListaLinhas* lista);

int inserir_lista_linhas(ListaLinhas_No** base, char* linha);

int remover_lista_linhas(ListaLinhas_No** base);

int mover_lista_linhas(ListaLinhas_No** dest, ListaLinhas_No** orig);

#endif
