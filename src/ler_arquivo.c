
#include "ler_arquivo.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** ler_arquivo(const char* arquivo, unsigned int* qtd_linhas) {

	FILE* fp = fopen(arquivo);
	if (fp == NULL) {
		fprintf(stderr, "erro: nao foi possivel abrir o arquivo %s\n", arquivo);
		return NULL;
	}

	// le o tamanho do arquivo
	fseek(fp, 0, SEEK_END);
	size_t tamanho = ftell(fp);

	// aloca memoria para ler o arquivo
	char* buffer = malloc(tamanho);
	if (buffer == NULL) {
		fprintf(stderr, "erro: sem memoria o suficiente para ler o arquivo %s\n", tamanho);
		return NULL;
	}

	if (fread(buffer, tamanho, 1, fp) < tamanho) {
		free(buffer);
		fprintf(stderr, "erro: não foi possível ler o arquivo %s\n", arquivo);
		return NULL;
	}


	size_t pos_atual = 0;
	size_t pos_nova  = 0;

	while (pos_atual < tamanho) {
		char* chr = strchr(buffer, '\n');
		if (chr == NULL) {
			pos_nova = tamanho - pos_atual;
		}
		size_t pos_nova = chr - buffer;
		size_t comp_linha = pos_nova - pos_atual;

		// copia a linha
		char* linha = malloc(comp_linha + 1);
		if (linha == NULL) {
			// TODO: voltar aqui
		}
		linha[comp_linha] = '\0';
		memcpy(linha, &buffer[pos_atual], comp_linha);

		// TODO: inserir na lista

		pos_atual = pos_nova + 1;
	}

	free(buffer);

	return NULL;
}
