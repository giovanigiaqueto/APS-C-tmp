#ifndef __LISTA_LINHAS_H
#define __LISTA_LINHAS_H

typedef struct ListaLinhas_No {
	char*                  linha;
	struct ListaLinhas_No* proximo;
} ListaLinhas_No;

typedef struct ListaLinhas {
	ListaLinhas_No* base;
} ListaLinhas;

#endif
