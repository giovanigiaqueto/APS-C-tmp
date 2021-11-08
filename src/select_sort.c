
#include "select_sort.h"

#include <string.h>

const Algoritimo alg_select_sort = {
	.nome = "Select Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = select_sort
};

int select_sort(char** linhas, unsigned int qtd_linhas){

	for (int i = 0; i < qtd_linhas; ++i) {
		unsigned int menor = i;

		for (int j = i + 1; j < qtd_linhas; ++j) {
			if (strcmp(linhas[menor], linhas[j]) > 0)
				menor = j;
		}
		if (i != menor) {
			char* tmp = linhas[i];
			linhas[i] = linhas[menor];
			linhas[menor] = tmp;
		}
	}

	return 0;
}
