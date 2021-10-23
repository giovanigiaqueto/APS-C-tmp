#ifndef __TEMPORIZACAO_H
#define __TEMPORIZACAO_H

#include "algoritimo_defs.h"

typedef float tempo_t;

tempo_t temporizar(char** linhas, unsigned int qtd_linhas,
		Algoritimo alg, tempo_t* tempo);

#endif
