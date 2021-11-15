
#ifndef __GRAFICO_IMPL
	#error renderer_defs.h so deve ser incluido por renderer.c
#else
#ifndef __GRAFICO_DEFS_H
#define __GRAFICO_DEFS_H

// vertices e cores
typedef struct Vertice {
	float cor[3];
	float pos[3];
} Vertice;

// formas primitivas
typedef struct Triangulo {
	Vertice vertices[3];
} Triangulo;
typedef struct Linha {
	Vertice vertices[2];
} Linha;

// textos
typedef enum AlinhamentoTexto {
	LEFT_ALIGN, CENTER_ALIGN, RIGHT_ALIGN
} AlinhamentoTexto;
typedef struct Texto {
	const char*       texto;
	Vertice           vert;
	AlinhamentoTexto  alinhamento;
} Texto;
typedef struct UTexto {
	const unsigned char* texto;
	Vertice              vert;
	AlinhamentoTexto     alinhamento;
} UTexto;

// camada
typedef struct Camada {
	Triangulo*   triangulos;
	Linha*       linhas;
	Texto*       textos;
	unsigned int triangulos_cnt;
	unsigned int linhas_cnt;
	unsigned int textos_cnt;
} Camada;

// renderer data
typedef struct RenderableData {
	// buffer de vertices
	Vertice*     vertices;
	unsigned int vertices_cnt;

	// buffer de textos
	UTexto*      textos;
	unsigned int textos_cnt;

	// sub-buffer de triangulos
	unsigned int triangulos_offset;
	unsigned int triangulos_tamanho;

	// sub-buffer de linhas
	unsigned int linhas_offset;
	unsigned int linhas_tamanho;

} RenderableData;

#endif
#endif
