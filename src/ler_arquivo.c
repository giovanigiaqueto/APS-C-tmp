
#include "ler_arquivo.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** ler_arquivo(const char* arquivo, unsigned int* qtd_linhas) {

	FILE* fp = fopen(arquivo, "r");
	if (fp == NULL) {
		*qtd_linhas = 0;
		fprintf(stderr, "erro: nao foi possivel abrir o arquivo %s\n", arquivo);
		return NULL;
	}

	// le o tamanho do arquivo
	fseek(fp, 0, SEEK_END);
	size_t tamanho = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// aloca memoria para ler o arquivo
	char* buffer = malloc(tamanho);
	if (buffer == NULL) {
		*qtd_linhas = 0;
		fprintf(stderr, "erro: sem memoria o suficiente para ler o arquivo %s\n", arquivo);
		return NULL;
	}

	// lê o arquivo
	if (fread(buffer, 1, tamanho, fp) < tamanho) {
		*qtd_linhas = 0;
		free(buffer);
		fprintf(stderr, "erro: não foi possível ler o arquivo %s\n", arquivo);
		return NULL;
	}

	// fecha o arquivo
	if (fclose(fp) != 0) {
		fprintf(stderr, "erro: não foi possível fechar o arquivo %s\n", arquivo);
	}

	// conta a quantidade de linhas
	unsigned int _qtd_linhas = 0;
	for (unsigned int i = 0; i < tamanho; ++i) {
		if (buffer[i] == '\n') ++_qtd_linhas;
	}

	char** linhas = malloc(_qtd_linhas * sizeof(char*));
	if (linhas == NULL) {
		*qtd_linhas = 0;
		free(buffer);
		fprintf(stderr, "erro: sem memória o suficiente para ler o arquivo %s\n", tamanho);
		return NULL;
	}

	size_t pos_atual = 0;
	size_t pos_nova  = 0;
	unsigned int linha_atual = 0;

	while (pos_atual < tamanho) {
		/* calcula o tamanho e posição da linha atual */
		char* chr = strchr(&buffer[pos_atual], '\n');
		if (chr == NULL) {
			pos_nova = tamanho - 1;
		} else {
			pos_nova = chr - buffer;
		}
		size_t comp_linha = pos_nova - pos_atual;

		// copia a linha
		char* linha = malloc(comp_linha + 1);
		unsigned int i = linha_atual;
		if (linha == NULL) {
			do {
				free(linhas[i]);
			} while (i --> 0);
			*qtd_linhas = 0;
			return NULL;
		}
		linha[comp_linha] = '\0';
		memcpy(linha, &buffer[pos_atual], comp_linha);

		// insere a linha
		linhas[linha_atual] = linha;

		pos_atual = pos_nova + 1;
		++linha_atual;
	}

	free(buffer);

	*qtd_linhas = _qtd_linhas;
	return linhas;
}
