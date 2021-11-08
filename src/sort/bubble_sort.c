
#include "bubble_sort.h"

#include <string.h>

const Algoritimo alg_bubble_sort = {
	.nome = "Bubble Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = bubble_sort
};

int bubble_sort(char** linhas, unsigned int qtd_linhas) {
	int repetir;
	do {
		repetir = 0;
		for (unsigned int i = 0; i < qtd_linhas - 1; ++i) {
			int res = strcmp(linhas[i], linhas[i+1]);
			if (res > 0) {
				char* tmp = linhas[i];
				linhas[i] = linhas[i+1];
				linhas[i+1] = tmp;
				repetir |= 1;
			}
		}
	} while(repetir);

	return 0;
}
