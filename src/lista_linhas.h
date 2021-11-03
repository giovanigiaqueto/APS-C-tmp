#ifndef __LISTA_LINHAS_H
#define __LISTA_LINHAS_H

typedef struct ListaLinhas_No {
	char*                  linha;
	struct ListaLinhas_No* proximo;
} ListaLinhas_No;

typedef struct ListaLinhas {
	ListaLinhas_No* base;
	ListaLinhas_No* fim;
} ListaLinhas;

/* ----- ----- ----- ----- ----- ----- ----- ----- //

AVISO:
	devido a adição de um ponteiro para o fim da lista
	de linhas, e a complexidade de implementar funções
	que manipulem esse ponteiro corretamente em alguns
	casos sem prejudicar a performance, todo código
	que manipular esse ponteiro deve fazê-lo também
	para cada uma das funções desse módulo que irão
	ser utilizadas. Além disso, cada módulo que use
	esse módulo deve ter documentado se esse ponteiro
	é usado (a ausência de um aviso será considerada
	como não utilizado).

// ----- ----- ----- ----- ----- ----- ----- ----- */

// inicia uma lista de linhas previamente alocada
void inicia_lista_linhas(ListaLinhas* lista);

// finaliza uma lista de linhas previamente alocada
void finaliza_lista_linhas(ListaLinhas* lista);

// aloca e inicializa uma lista de linhas na heap
ListaLinhas* criar_lista_linhas();

// finaliza e libera uma lista de linhas alocada na heap
void deletar_lista_linhas(ListaLinhas** lista);

// insere uma linha na lista de linhas
int inserir_lista_linhas(ListaLinhas_No** base, char* linha);

// remove uma linha da lista de linhas
int remover_lista_linhas(ListaLinhas_No** base);

// move uma linha dentro de uma lista de
// linhas ou entre duas lista de linhas
int mover_lista_linhas(ListaLinhas_No** dest, ListaLinhas_No** orig);

// concatena uma lista de linhas no final de uma lista de linhas
int concat_lista_linhas(ListaLinhas* dest, ListaLinhas* orig);

#endif
