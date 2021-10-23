#ifndef __TEMPORIZACAO_H
#define __TEMPORIZACAO_H

#include "algoritimo.h"

typedef float tempo_t;

int temporizar(char** linhas, unsigned int qtd_linhas,
		Algoritimo alg, tempo_t* tempo);

#endif
