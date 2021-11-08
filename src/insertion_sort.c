#include "insertion_sort.h"

#include <string.h>
#include <stdio.h>

int insertion_sort(ListaLinhas* lista, unsigned int qtd_int){
	ListaLinhas_No** no_linha = &(lista->base);
	while  (*no_linha != NULL)
	{
		ListaLinhas_No** no_linha2 = &(lista->base);
		int movido = 0;
		while (no_linha2 != no_linha)
		{
			if (strcmp ((*no_linha)->linha, (*no_linha2)->linha) < 0) 
			{
				if (mover_lista_linhas(no_linha2, no_linha) != 0) return -1;
				movido = 1;
				break;
			}
			no_linha2 = &((*no_linha2)->proximo);
		}
		if (!movido) no_linha = &((*no_linha)->proximo);
	}

	return 0;
}

const Algoritimo alg_insertion_sort = {
	.nome = "Insertion Sort",
	.tipo = ALG_LISTA,
	.func.alg_lista = insertion_sort
};
