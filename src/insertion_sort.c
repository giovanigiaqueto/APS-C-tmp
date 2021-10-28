#include "insertion_sort.h"

int insertion_sort(ListaLinhas* lista, unsigned int qtd_int){
    ListaLinhas_No* no_linha = lista->base;
    while  (no_linha != NULL)
    {
		ListaLinhas_No* proximo = no_linha->proximo;
		remover_lista_linhas(&no_linha);
		ListaLinhas_No* no_linha2 = lista->base;
		do
		{
			if (strcmp (no_linha->linha, no_linha2->proximo->linha) < 0) 
			{
				inserir_lista_linhas(&no_linha2, no_linha->linha);
				free ((void*) no_linha);
				break;
			}
		}
		while (no_linha2->proximo != proximo);
		no_linha = proximo;
    }
    
}

