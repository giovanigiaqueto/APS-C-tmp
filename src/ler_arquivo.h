
#ifndef __LER_ARQUIVO_H
#define __LER_ARQUIVO_H

/**
 * lê as linhas de um arquivo em um vetor,
 * e retorna a quantidade de linhas lidas.
 *
 * @param arquivo nome do arquivo a ser lido
 * @param qtd_linhas ponteiro para retornar
 *      a quantidade de linhas lidas.
 *
 * @returns as linhas lidas
 */
char** ler_arquivo(const char* arquivo, unsigned int* qtd_linhas);

#endif
