#ifndef __GRAFICO_RENDERER_H
#define __GRAFICO_RENDERER_H

// includes
#include "../resultados.h"

// tamanho da tela
extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;

int opengl_resultados_algoritimos(ResultadoAlgoritimo* resultados,
		const unsigned int* qtd_linhas, unsigned int resultados_tamanho,
		unsigned int qtd_linhas_tamanho);

int iniciar_opengl(int* argc, char** argv);
void mainloop_opengl();

#endif