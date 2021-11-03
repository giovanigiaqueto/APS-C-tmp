
#include "binary_insertion_sort.h"

#include <string.h>

int binary_insertion_sort(char** linhas, unsigned int qtd_linhas) {

	// checa se a lista já está ordenada
	if (qtd_linhas == 1) return 0;

	for (unsigned int indice = 1; indice < qtd_linhas; ++indice) {

		// procura binária
		unsigned int head = 0;
		unsigned int tail = indice - 1;
		unsigned int iter = (head + tail) / 2;
		do {
			int res = strcmp(linhas[indice], linhas[iter]);
			if (res > 0) {
				head = iter + 1;
			} else if (res < 0) {
				tail = iter;
			} else break;
			iter = (head + tail) / 2;
		} while (head < tail);

		// pula se uma inserção não é necessária
		if (iter == indice - 1 && strcmp(linhas[indice], linhas[iter]) > 0) {
			continue;
		}

		// inserção
		char* linha = linhas[indice];
		for (unsigned int i = indice; i > iter; --i) {
			linhas[i] = linhas[i - 1];
		}
		linhas[iter] = linha;
	}

	return 0;
}

const Algoritimo alg_binary_insertion_sort = {
	.nome = "Binary Insertion Sort",
	.tipo = ALG_VETOR,
	.func.alg_vetor = binary_insertion_sort
};
