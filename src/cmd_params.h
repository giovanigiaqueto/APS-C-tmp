#ifndef __CMD_PARAMS_H
#define __CMD_PARAMS_H

#include <limits.h>

#define PARAM_FLAG_ENTRADA_GERADA      0x01
#define PARAM_FLAG_ENTRADA_EMBARALHADA 0x02

#define PARAM_FLAG_REPETICOES  0x04
#define PARAM_FLAG_QTD_LINHAS  0x08
#define PARAM_FLAG_ENTRADA     0x10

#define ERRO_SEM_ERRO         0
#define ERRO_SEM_MEMORIA      1
#define ERRO_ARRAY_DYN_ALLOC  2
#define ERRO_LEITURA_ARQUIVO  4

#define ERRO_PARAM_AJUDA        0x0008
#define ERRO_PARAM_FALTANDO     0x0010
#define ERRO_PARAM_INVALIDO     0x0020

#define ERRO_PARAM_TIPO_MASK    0x0038
#define ERRO_PARAM_SUBTIPO_MASK 0x03B0

#define ERRO_PARAM_REPETICOES   0x0040
#define ERRO_PARAM_QTD_LINHAS   0x0080
#define ERRO_PARAM_ENTRADA      0x0100
#define ERRO_PARAM_ALGORITIMOS  0x0200

#define NUM_REPETICOES_MAX 5000
#define REPETICOES_PADRAO  2000
#define QTD_LINHAS_MAX     UINT_MAX

#if (NUM_REPETICOES_MAX > UINT_MAX)
	#error NUM_REPETICOES_MAX eh maior que UINT_MAX
#endif
#if (NUM_REPETICOES_PADRAO > UINT_MAX)
	#error NUM_REPETICOES_PADRAO eh maior que UINT_MAX
#endif
#if (QTD_LINHAS_MAX > UINT_MAX)
	#error QTD_LINHAS_MAX eh maior que UINT_MAX
#endif

#include "algoritimo.h"

typedef struct Parametros {
	unsigned int repeticoes;

	unsigned int* qtd_linhas;
	unsigned int  qtd_linhas_tamanho;

	const char*  entrada;

	const Algoritimo** algoritimos;
	unsigned int       algoritimos_tamanho;

	unsigned int flags;
} Parametros;

int ler_parametros(int argc, char* argv[], Parametros* params);

#endif
