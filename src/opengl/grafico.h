
#ifndef __GRAFICO_IMPL
	#error grafico.h nao deve ser incluido diretamente
#else
#ifndef __GRAFICO_H
#define __GRAFICO_H

#include "renderer_defs.h"
#include "../resultados.h"

RenderableData* gerar_grafico(ResultadoAlgoritimo* resultados,
		const unsigned int* qtd_linhas, unsigned int resultados_tamanho,
		unsigned int qtd_linhas_tamanho);

void renderizar_mouse(int x, int y);

#endif
#endif
