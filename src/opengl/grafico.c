
#define __STDC_WANT_IEC_60559_BFP_EXT__ // strfromf
#include <stdlib.h> // malloc & free
#include <string.h> // strlen & NULL
#include <limits.h> // UINT_MAX
#include <ctype.h>  // isspace
#include <stdio.h>
#include <math.h>

#define __GRAFICO_IMPL
	#include "grafico.h"
	#include "opengl-pch.h"
#undef  __GRAFICO_IMPL

#include "../aleatorio.h"
#include "../temporizacao.h"

// tamanho da tela
extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;

// posição e tamanho do gráfico
extern const unsigned int X_PADDING;
extern const unsigned int Y_PADDING;
extern const unsigned int GRAFICO_LARGURA;
extern const unsigned int GRAFICO_ALTURA;
extern const unsigned int GRAFICO_POSX;
extern const unsigned int GRAFICO_POSY;

// dados da grade
typedef struct Grade {
	// coordenadas da grade
	float x0, x1, y0, y1;

	// coordenadas da origen
	float origx, origy;

	// coordenadas dos eixos
	unsigned int eixos_nx, eixos_ny; // quatidade de eixos
	float        eixos_dx, eixos_dy; // (valor / id_eixo)
	float        eixos_px, eixos_py; // posição do primeiro eixo

	// valores dos eixos
	float escala_x, escala_y; // escala (pixel / valor)
} Grade;

// limites dos dados
typedef struct LimitesDados {
	float min_x, max_x;
	float min_y, max_y;
} LimitesDados;

// cores
const float CORES[][3] = {
	{ 0.8f, 0.0f, 0.0f }, 	// vermelho
	{ 0.0f, 0.8f, 0.0f }, 	// verde
	{ 0.0f, 0.0f, 0.8f }, 	// azul
	{ 0.7f, 0.0f, 0.7f }, 	// magenta
	{ 0.0f, 0.8f, 0.8f }, 	// ciano
	{ 0.5f, 0.5f, 0.5f }, 	// cinza
	{ 1.0f, 0.5f, 0.0f }, 	// laranja
	{ 0.0f, 0.6f, 0.0f }, 	// verde escuro
};
const unsigned int NUM_CORES = sizeof(CORES) / sizeof(float[3]);

Grade*        glob_grade   = NULL;
LimitesDados* glob_limites = NULL;
unsigned char texto_mouse[28];
void renderizar_mouse(int x, int y) {
	float ux = 2 * x / (float) WIDTH  - 1.0f;
	float uy = 1.0f - 2 * y / (float) HEIGHT;
	if (ux >= glob_grade->x0 && ux <= glob_grade->x1 &&
			uy >= glob_grade->y0 && uy <= glob_grade->y1) {

		float rx = ux + 24 / (float) WIDTH;
		float ry = uy + 12 / (float) HEIGHT;

		// 8 digitos + nova linha + 8 digitos com uma virgula + 
		// 's' ou 'ms' ou 'ns' + \0
		char buff_x[8];
		char buff_y[9];
		char texto[28] = {
			' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
			' ', 'l', 'i', 'n', 'h', 'a', 's', '\n',
			' ', ' ', ' ', ' ', ' ', ' ', 
			' ', ' ', ' ', 'm', 's', '\0'
		};

		float escala_x = 2 * glob_grade->escala_x / (float) WIDTH;
		float escala_y = 2 * glob_grade->escala_y / (float) HEIGHT;
		float vx = glob_limites->min_x + (ux - glob_grade->origx) / escala_x;
		float vy = glob_limites->min_y + (uy - glob_grade->origy) / escala_y;

		// casa decimal do primeiro digito
		if (vy > 0) {
			unsigned int casa_dec = ceilf(-log10f(vy));

			unsigned int mult;
			if (casa_dec < 2) {
				mult = 1;
				texto[25] = 's';
				texto[26] = ' ';
			} else if (casa_dec < 5) {
				mult = 1000;
			} else {
				mult = 1000000;
				texto[25] = 'n';
			}

			snprintf(buff_y, 9, "%8.2f", vy * mult);
		} else {
			strncpy(buff_y, "     NaN", 9);
			texto[25] = 's';
			texto[26] = ' ';
		}

		snprintf(buff_x, 8, "%7.0f", vx);
		strncpy(&texto[1], buff_x, 7);  // o alerta nessa linha pode ser ignorado
		strncpy(&texto[16], buff_y, 8); // o alerta nessa linha pode ser ignorado
		for (unsigned int i = 0; i < sizeof(texto_mouse); ++i) {
			texto_mouse[i] = (unsigned char) texto[i];
		}
	
		glColor3f(0.8f, 0.25f, 0.0f);
		glRasterPos3f(rx, ry, -0.1f);
		glutBitmapString(GLUT_BITMAP_8_BY_13, texto_mouse);
	}
}

unsigned char* cstr_para_ucstr(const char* str) {
	size_t tamanho = strlen(str);
	unsigned char* ustr = malloc(tamanho + 1);
	if (ustr != NULL) {
		for (unsigned int i = 0; i < tamanho + 1; ++i) {
			ustr[i] = (unsigned char) str[i];
		}
	}
	return ustr;
}

const char* fmt_eixo_grade(float valor, float passo, int* expoente) {
	int expo = 0;
	float valor_eixo = passo * floor(valor / passo);
	if (passo != 0) {
		expo = -3 * (((int) floorf(log10f(passo))) / 3);
	}
	if (expoente != NULL) *expoente = -expo;

	char str[32];
	snprintf(str, 32, "%9.2f", valor_eixo * pow(10, expo));
	str[31] = '\0';
	
	// procura os caracteres que não são espaços
	size_t idx_l = 0, idx_r = strlen(str);
	for (; idx_l < idx_r; ++idx_l) {
		if (!isspace(str[idx_l])) break;
	}
	while (idx_r > idx_l) {
		if (!isspace(str[idx_r - 1])) break;
		--idx_r;
	}

	// copia o texto removendo os espaços
	char* texto = malloc((idx_r - idx_l) + 1);
	if (texto != NULL) {
		for (size_t i = 0; i < idx_r - idx_l; ++i) {
			texto[i] = str[i + idx_l];
		}
		texto[idx_r - idx_l] = 0;
	}
	return texto;
}

RenderableData* processar_camadas(Camada* camadas, unsigned int qtd_camadas) {

	// aloca memória
	RenderableData* renderable = (RenderableData*) malloc(sizeof(RenderableData));
	if (renderable == NULL) return NULL;

	// calcula o tamanho do buffer
	unsigned int cnt_triangulos = 0;
	unsigned int cnt_linhas = 0;
	unsigned int cnt_textos = 0;
	for (unsigned int i = 0; i < qtd_camadas; ++i) {
		cnt_triangulos += camadas[i].triangulos_cnt;
		cnt_linhas += camadas[i].linhas_cnt;
		cnt_textos += camadas[i].textos_cnt;
	}

	// aloca memoria para o buffer
	const unsigned int cnt_vertices = (3 * cnt_triangulos + 2 * cnt_linhas);
	Vertice* vert_buff = (Vertice*) malloc(sizeof(Vertice) * cnt_vertices);
	if (vert_buff == NULL) {
		free((void*) renderable);
		fprintf(
			stderr,
			"erro: nao foi possivel alocar memoria o suficiente para a renderizacao\n"
		);
		return NULL;
	}
	UTexto* textos_buff = (UTexto*) malloc(sizeof(Texto) * cnt_textos);
	if (textos_buff == NULL) {
		free((void*) renderable);
		free((void*) vert_buff);
		fprintf(
			stderr,
			"erro: nao foi possivel alocar memoria o suficiente para a renderizacao\n"
		);
		return NULL;
	}

	// vertices dos triangulos
	unsigned int offset = 0;
	for (unsigned int i = 0; i < qtd_camadas; ++i) {
		unsigned int tamanho = camadas[i].triangulos_cnt * 3 * sizeof(Vertice);
		memcpy(((void*) vert_buff) + offset, camadas[i].triangulos, tamanho);
		offset += tamanho;
	}

	// vertices das linhas
	for (unsigned int i = 0; i < qtd_camadas; ++i) {
		unsigned int tamanho = camadas[i].linhas_cnt * 2 * sizeof(Vertice);
		memcpy(((void*) vert_buff) + offset, camadas[i].linhas, tamanho);
		offset += tamanho;
	}

	// textos
	unsigned int id_texto = 0;
	for (unsigned int i = 0; i < qtd_camadas; ++i) {
		Texto* textos = camadas[i].textos;
		unsigned int j;
		for (j = 0; j < camadas[i].textos_cnt; ++j, ++id_texto) {
			Texto* texto = &textos[j];
			unsigned char* ustr = cstr_para_ucstr(texto->texto);
			if (ustr == NULL) break;

			textos_buff[id_texto].texto = ustr;
			textos_buff[id_texto].alinhamento = texto->alinhamento;
			Vertice* vsrc = &texto->vert;
			Vertice* vdst = &textos_buff[id_texto].vert;
			vdst->cor[0] = vsrc->cor[0];
			vdst->cor[1] = vsrc->cor[1];
			vdst->cor[2] = vsrc->cor[2];
			vdst->pos[0] = vsrc->pos[0];
			vdst->pos[1] = vsrc->pos[1];
			vdst->pos[2] = vsrc->pos[2];
		}
		if (j < camadas[i].textos_cnt) break;
	}
	if (id_texto < cnt_textos) {
		free((void*) renderable);
		free((void*) vert_buff);
		for (unsigned int i = 0; i < id_texto; ++i) {
			free((void*) textos_buff[i].texto);
		}
		free((void*) textos_buff);
		vert_buff   = NULL;
		textos_buff = NULL;
		return NULL;
	}

	renderable->vertices     = vert_buff;
	renderable->vertices_cnt = cnt_vertices;
	renderable->textos       = textos_buff;
	renderable->textos_cnt   = cnt_textos;

	renderable->triangulos_offset  = 0;
	renderable->triangulos_tamanho = 3 * cnt_triangulos;
	renderable->linhas_offset      = 3 * cnt_triangulos;
	renderable->linhas_tamanho     = 2 * cnt_linhas;

	return renderable;
}

int minmax_qtd_linhas(const unsigned int* qtd_linhas,
	unsigned int qtd_linhas_tamanho, unsigned int* min,
	unsigned int* max) {
	
	if (qtd_linhas == NULL || qtd_linhas_tamanho == 0) return -1;

	unsigned int min_qtd_linhas = UINT_MAX;
	unsigned int max_qtd_linhas = 0;
	for (unsigned int i = 0; i < qtd_linhas_tamanho; ++i) {
		if (qtd_linhas[i] > max_qtd_linhas) {
			max_qtd_linhas = qtd_linhas[i];
		}
		if (qtd_linhas[i] < min_qtd_linhas) {
			min_qtd_linhas = qtd_linhas[i];
		}
	}

	*min = min_qtd_linhas;
	*max = max_qtd_linhas;
	return 0;
}
int minmax_tempo(ResultadoAlgoritimo* resultados,
		unsigned int resultados_tamanho, tempo_t* min,
		tempo_t* max) {

	if (resultados == NULL || resultados_tamanho == 0) return -1;
	
	tempo_t min_tempo = resultados[0].tempos[0];
	tempo_t max_tempo = resultados[0].tempos[0];
	for (unsigned int i = 0; i < resultados_tamanho; ++i) {
		tempo_t* tempos = resultados[i].tempos;
		for (unsigned int j = 0; j < resultados[i].tempos_tamanho; ++j) {
			if (tempos[j] > max_tempo) {
				max_tempo = tempos[j];
			} else if (tempos[j] < min_tempo) {
				min_tempo = tempos[j];
			}
		}
	}

	*min = min_tempo;
	*max = max_tempo;
	return 0;
}

int limites_dados(LimitesDados* limites,
		ResultadoAlgoritimo* resultados, const unsigned int* qtd_linhas,
		unsigned int resultados_tamanho, unsigned int qtd_linhas_tamanho) {

	unsigned int min_x, max_x;
	tempo_t      min_y, max_y;

	int err_x = minmax_qtd_linhas(qtd_linhas, qtd_linhas_tamanho, &min_x, &max_x);
	int err_y = minmax_tempo(resultados, resultados_tamanho, &min_y, &max_y);
	if (err_x != 0 || err_y != 0) return -1;

	limites->min_x = (float) min_x;
	limites->max_x = (float) max_x;
	limites->min_y = (float) min_y;
	limites->max_y = (float) max_y;
	return 0;
}

int passo_grade(float* passo, const float escala) {
	if (escala < 1e-9 && escala > -1e-9) return -1;

	int paridade = 0;
	float valor = 1.0f;
	while (1) {
		float res = valor * escala;
		if (res < 100.0f) {
			valor *= 2 + paridade * 3;
		} else if (res > 250.0f) {
			valor /= 2 + paridade * 3;
		} else break;
		paridade = (paridade + 1) % 2;
	}

	*passo = valor;
	return 0;
}

int gerar_grade(Grade* grade, LimitesDados* limites,
	unsigned int padx, unsigned int pady) {

	if (grade == NULL || limites == NULL) return -1;

	// coordenadas
	grade->x0 = 2 * GRAFICO_POSX / (float) WIDTH  - 1.0f;
	grade->y0 = 2 * GRAFICO_POSY / (float) HEIGHT - 1.0f;
	grade->x1 = 1.0f - 2 * GRAFICO_POSX / (float) WIDTH;
	grade->y1 = 1.0f - 2 * GRAFICO_POSY / (float) HEIGHT;

	// origen
	grade->origx = grade->x0 + 2 * padx / (float) WIDTH;
	grade->origy = grade->y0 + 2 * pady / (float) HEIGHT;

	// constantes
	const unsigned int LARGURA = GRAFICO_LARGURA - 2 * padx;
	const unsigned int ALTURA  = GRAFICO_ALTURA  - 2 * pady;

	// eixos
	grade->escala_x = LARGURA / (limites->max_x - limites->min_x);
	grade->escala_y = ALTURA  / (limites->max_y - limites->min_y);

	// distancias entre os eixos (passo (dx,dy))
	int err_x = passo_grade(&grade->eixos_dx, 2 * grade->escala_x);
	int err_y = passo_grade(&grade->eixos_dy, 2 * grade->escala_y);
	if (err_x != 0 || err_y != 0) return -1;

	// escala para converter entre coordenadas de pixel
	// e coordenadas normalizadas de tela:
	//
	//  SUPERIOR ESQUERDO = (-1, 1)
	//  SUPERIOR DIREITO  = ( 1, 1)
	//  INFERIOR ESQUERDO = (-1,-1)
	//  INFERIOR DIREITO  = ( 1,-1)
	//	
	float escala_x = 2 * grade->escala_x / WIDTH;
	float escala_y = 2 * grade->escala_y / HEIGHT;

	// distancias entre a origen e (x1,y1)
	float dist_origx_x1 = fmaxf(grade->x0, grade->x1) - grade->origx;
	float dist_origy_y1 = fmaxf(grade->y0, grade->y1) - grade->origy;

	// valores dos eixos na origen e (x1,y1)
	float val_origx = grade->eixos_dx * floorf(limites->min_x / grade->eixos_dx);
	float val_origy = grade->eixos_dy * floorf(limites->min_y / grade->eixos_dy);
	float val_x1 = grade->eixos_dx * ceilf(
		(limites->min_x + dist_origx_x1 / escala_x) / grade->eixos_dx
	);
	float val_y1 = grade->eixos_dy * ceilf(
		(limites->min_y + dist_origy_y1 / escala_y) / grade->eixos_dy
	);

	// quantidade de eixos
	grade->eixos_nx = floorf((val_x1 - val_origx) / grade->eixos_dx);
	grade->eixos_ny = floorf((val_y1 - val_origy) / grade->eixos_dy);

	// posição do primeiro eixo
	unsigned int ix0 = floorf(limites->min_x / grade->eixos_dx);
	unsigned int iy0 = floorf(limites->min_y / grade->eixos_dy);
	float i0_orig_vx = ix0 * grade->eixos_dx - limites->min_x;
	float i0_orig_vy = iy0 * grade->eixos_dy - limites->min_y;
	grade->eixos_px = grade->origx + i0_orig_vx * escala_x;
	grade->eixos_py = grade->origy + i0_orig_vy * escala_y;

	return 0;
};

int glut_bitmap_lenght_signed(void* font, const char* str) {
	size_t tamanho = strlen(str);
	unsigned char* ustr = malloc(tamanho + 1);
	if (ustr == NULL) return -1;

	for (unsigned int k = 0; k < tamanho + 1; ++k) {
		ustr[k] = str[k];
	}
	int bitmap_len = glutBitmapLength(font, ustr);
	free((void*) ustr);
	return bitmap_len;
}

RenderableData* gerar_grafico(ResultadoAlgoritimo* resultados,
		const unsigned int* qtd_linhas, unsigned int resultados_tamanho,
		unsigned int qtd_linhas_tamanho) {

	if (resultados == NULL || qtd_linhas == NULL) return NULL;
	if (resultados_tamanho == 0 || qtd_linhas_tamanho == 0) return NULL;

	const unsigned int PADX = 32;
	const unsigned int PADY = 32;

	RenderableData* renderable = NULL;
	LimitesDados*   limites    = NULL;
	Grade*          grade      = NULL;
	{
		limites = malloc(sizeof(LimitesDados));
		if (limites == NULL) return NULL;
		if (limites_dados(limites, resultados, qtd_linhas,
				resultados_tamanho, qtd_linhas_tamanho) != 0) return NULL;

		grade = malloc(sizeof(Grade));
		if (grade == NULL) goto cleanup;
		if (gerar_grade(grade, limites, PADX, PADY) != 0) {
			fprintf(stderr, "erro: falha na geracao da grade do grafico\n");
			goto cleanup;
		}
	}

	Camada* camadas = malloc(2 * sizeof(Camada));
	if (camadas == NULL) return NULL;

	Linha* linhas = (Linha*) malloc((4 +
		resultados_tamanho * qtd_linhas_tamanho) * sizeof(Linha));
	if (linhas == NULL) goto cleanup;

	Triangulo* triangulos = (Triangulo*) malloc(2 * sizeof(Triangulo));
	if (triangulos == NULL) goto cleanup;

	Texto* textos = (Texto*) malloc(resultados_tamanho * sizeof(Texto));
	if (textos == NULL) goto cleanup;

	Texto* textos_grade = NULL;

	/* ---------- linhas dos resultados ---------- */
	float grafico_x = 2 * (X_PADDING + PADX) / (float) WIDTH  - 1.0f;
	float grafico_y = 2 * (Y_PADDING + PADY) / (float) HEIGHT - 1.0f;
	{
		// --- cores --
		unsigned int* cores_idx = malloc(NUM_CORES * sizeof(unsigned int));
		if (cores_idx == NULL) goto cleanup;

		for (unsigned int i = 0; i < NUM_CORES; ++i) cores_idx[i] = i;
		embaralhar(cores_idx, sizeof(unsigned int), NUM_CORES);

		float escala_x = 2 * grade->escala_x / (float) WIDTH;
		float escala_y = 2 * grade->escala_y / (float) HEIGHT;

		float legenda_spacing_y = 16.0f / (float) HEIGHT;
		float legenda_stride_y = \
			-((float) glutBitmapHeight(GLUT_BITMAP_8_BY_13) / HEIGHT + legenda_spacing_y);

		unsigned int max_tamanho_ustr = 0;
		for (unsigned int i = 0; i < resultados_tamanho; ++i) {
			const float (*cor)[3] = &CORES[cores_idx[i]];
			ResultadoAlgoritimo* res = &resultados[i];

			int bitmap_len = glut_bitmap_lenght_signed(
					GLUT_BITMAP_8_BY_13, res->algoritimo->nome
			);
			if (bitmap_len == -1) goto cleanup;
			if (bitmap_len > max_tamanho_ustr) {
				max_tamanho_ustr = bitmap_len;
			}

			textos[i].texto = res->algoritimo->nome;
			textos[i].alinhamento = RIGHT_ALIGN;
			textos[i].vert.pos[0] = +grafico_x;
			textos[i].vert.pos[1] = -grafico_y + (i + 1) * legenda_stride_y;
			textos[i].vert.pos[2] = 0.0f;
			textos[i].vert.cor[0] = (*cor)[0];
			textos[i].vert.cor[1] = (*cor)[1];
			textos[i].vert.cor[2] = (*cor)[2];

			for (unsigned int j = 0; j < qtd_linhas_tamanho - 1; ++j) {
				Linha* linha = &linhas[j + i*qtd_linhas_tamanho];
				Vertice* v0 = &linha->vertices[0];
				Vertice* v1 = &linha->vertices[1];

				v0->pos[0] = grafico_x + escala_x * (qtd_linhas[j]  - limites->min_x);
				v0->pos[1] = grafico_y + escala_y * (res->tempos[j] - limites->min_y);
				v0->pos[2] = 0.2f;
				v1->pos[0] = grafico_x + escala_x * (qtd_linhas[j+1]  - limites->min_x);
				v1->pos[1] = grafico_y + escala_y * (res->tempos[j+1] - limites->min_y);
				v1->pos[2] = 0.2f;

				v0->cor[0] = (*cor)[0]; v1->cor[0] = (*cor)[0];
				v0->cor[1] = (*cor)[1]; v1->cor[1] = (*cor)[1];
				v0->cor[2] = (*cor)[2]; v1->cor[2] = (*cor)[2];
			}
		}

		// caixa legendas
		{
			float dx = 16.0f / WIDTH;
			float dy = 16.0f / HEIGHT;

			float px0 =  grafico_x - dx;
			float py0 = -grafico_y + dy;
			float px1 =  grafico_x + dx + 2 * max_tamanho_ustr / (float) WIDTH;
			float py1 = -grafico_y - dy - 
					legenda_spacing_y + legenda_stride_y * resultados_tamanho;

			Vertice verts[4] = {
				{ .cor = { 0.85f, 0.85f, 0.85f }, .pos = { px0, py0, 0.1f } },
				{ .cor = { 0.85f, 0.85f, 0.85f }, .pos = { px1, py0, 0.1f } },
				{ .cor = { 0.85f, 0.85f, 0.85f }, .pos = { px0, py1, 0.1f } },
				{ .cor = { 0.85f, 0.85f, 0.85f }, .pos = { px1, py1, 0.1f } },
			};

			triangulos[0].vertices[0] = verts[0];
			triangulos[0].vertices[1] = verts[2];
			triangulos[0].vertices[2] = verts[3];
			triangulos[1].vertices[0] = verts[0];
			triangulos[1].vertices[1] = verts[3];
			triangulos[1].vertices[2] = verts[1];
		}

		free((void*) cores_idx);
	}

	/* --------------- grade --------------- */
	Linha* linhas_grade = NULL;
	unsigned int qtd_linhas_grade = 0;
	{
		// escala para converter entre coordenadas de pixel
		// e coordenadas normalizadas de tela:
		//
		//  SUPERIOR ESQUERDO = (-1, 1)
		//  SUPERIOR DIREITO  = ( 1, 1)
		//  INFERIOR ESQUERDO = (-1,-1)
		//  INFERIOR DIREITO  = ( 1,-1)
		//
		float escala_x = 2 * grade->escala_x / (float) WIDTH;
		float escala_y = 2 * grade->escala_y / (float) HEIGHT;

		qtd_linhas_grade = grade->eixos_nx + grade->eixos_ny;
		if (qtd_linhas_grade != 0) {
			linhas_grade = malloc(qtd_linhas_grade * sizeof(Linha));
			if (linhas_grade == NULL) goto cleanup;

			textos_grade = malloc(qtd_linhas_grade * sizeof(Texto));
			if (textos_grade == NULL) goto cleanup;

			// horizontal
			unsigned int id_linha = 0;
			for (unsigned int i = 0; i < grade->eixos_ny; ++i, ++id_linha) {
				Vertice* v0 = &linhas_grade[id_linha].vertices[0];
				Vertice* v1 = &linhas_grade[id_linha].vertices[1];

				// aviso: NÃO RETIRE O PARENTESES (a equação quebra por causa de cast)
				float dy = (i * grade->eixos_dy) * escala_y;
				v0->pos[0] = grade->x0;
				v1->pos[0] = grade->x1;
				v0->pos[1] = grade->eixos_py + dy;
				v1->pos[1] = grade->eixos_py + dy;
				v0->pos[2] = 0.3f;
				v1->pos[2] = 0.3f;

				v0->cor[0] = 0.70f; v1->cor[0] = 0.70f;
				v0->cor[1] = 0.70f; v1->cor[1] = 0.70f;
				v0->cor[2] = 0.70f; v1->cor[2] = 0.70f;

				// texto do eixo
				int expoente;
				const char* str = fmt_eixo_grade(
					limites->min_y + i * grade->eixos_dy,
					grade->eixos_dy, &expoente
				);
				if (str == NULL) break;

				if (expoente > -2) {

					size_t len = strlen(str);
					char* newstr = realloc((void*) str, len + 2);
					if (newstr == NULL) {
						free((void*) str);
						break;
					}
					str = strcat(newstr, " s");
					
				} else if (expoente > -5) {

					size_t len = strlen(str);
					char* newstr = realloc((void*) str, len + 3);
					if (newstr == NULL) {
						free((void*) str);
						break;
					}
					str = strcat(newstr, " ms");

				} else {

					size_t len = strlen(str);
					char* newstr = realloc((void*) str, len + 3);
					if (newstr == NULL) {
						free((void*) str);
						break;
					}
					str = strcat(newstr, " ns");

				}

				textos_grade[id_linha].texto = str;
				textos_grade[id_linha].alinhamento = LEFT_ALIGN;

				Vertice* vert = &textos_grade[id_linha].vert;
				vert->cor[0] = 0.2f; vert->pos[0] = grade->x0 - 32 / (float) WIDTH;
				vert->cor[1] = 0.2f; vert->pos[1] = grade->eixos_py + dy;
				vert->cor[2] = 0.2f; vert->pos[2] = -0.2f;
			}
			if (id_linha != grade->eixos_ny) {
				for (unsigned int i = 0; i < id_linha; ++i) {
					free((void*) textos_grade[i].texto);
					textos_grade[i].texto = NULL;
				}
				free((void*) textos_grade);
				textos_grade = NULL;
			}

			// vertical
			for (unsigned int i = 0; i < grade->eixos_nx; ++i, ++id_linha) {
				Vertice* v0 = &linhas_grade[id_linha].vertices[0];
				Vertice* v1 = &linhas_grade[id_linha].vertices[1];

				// aviso: NÃO RETIRE O PARENTESES (a equação quebra por causa de cast)
				float dx = (i * grade->eixos_dx) * escala_x;
				v0->pos[0] = grade->eixos_px + dx;
				v1->pos[0] = grade->eixos_px + dx;
				v0->pos[1] = grade->y0;
				v1->pos[1] = grade->y1;
				v0->pos[2] = 0.3f;
				v1->pos[2] = 0.3f;

				v0->cor[0] = 0.70f; v1->cor[0] = 0.70f;
				v0->cor[1] = 0.70f; v1->cor[1] = 0.70f;
				v0->cor[2] = 0.70f; v1->cor[2] = 0.70f;

				// texto do eixo
				int expoente;
				const char* str = fmt_eixo_grade(
					limites->min_x + i * grade->eixos_dx,
					grade->eixos_dx, &expoente
				);
				if (str == NULL) break;

				textos_grade[id_linha].texto = str;
				textos_grade[id_linha].alinhamento = CENTER_ALIGN;

				Vertice* vert = &textos_grade[id_linha].vert;
				vert->cor[0] = 0.2f; vert->pos[0] = grade->eixos_px + dx;

				vert->cor[1] = 0.2f; vert->pos[1] = \
					grade->y0 - (32 + glutBitmapHeight(GLUT_BITMAP_8_BY_13)) / (float) WIDTH;

				vert->cor[2] = 0.2f; vert->pos[2] = -0.2f;
			}
			if (id_linha != qtd_linhas_grade) {
				for (unsigned int i = 0; i < id_linha; ++i) {
					free((void*) textos_grade[i].texto);
					textos_grade[i].texto = NULL;
				}
				free((void*) textos_grade);
				textos_grade = NULL;
			}
		}
	}

	/* ---------- grade de gráfico ---------- */
	{
		float posx = 2 * X_PADDING / (float) WIDTH  - 1.0f;
		float posy = 2 * Y_PADDING / (float) HEIGHT - 1.0f;

		Vertice verts[4] = {
			{ .cor = { 0.0f, 0.0f, 0.0f }, .pos = { +posx, -posy, 0.0f } },
			{ .cor = { 0.0f, 0.0f, 0.0f }, .pos = { -posx, -posy, 0.0f } },
			{ .cor = { 0.0f, 0.0f, 0.0f }, .pos = { +posx, +posy, 0.0f } },
			{ .cor = { 0.0f, 0.0f, 0.0f }, .pos = { -posx, +posy, 0.0f } }
		};
		
		unsigned int offset = resultados_tamanho * qtd_linhas_tamanho;
		Linha* linhas_rel = &linhas[offset];

		linhas_rel[0].vertices[0] = verts[0];
		linhas_rel[0].vertices[1] = verts[1];
		linhas_rel[1].vertices[0] = verts[1];
		linhas_rel[1].vertices[1] = verts[3];
		linhas_rel[2].vertices[0] = verts[3];
		linhas_rel[2].vertices[1] = verts[2];
		linhas_rel[3].vertices[0] = verts[0];
		linhas_rel[3].vertices[1] = verts[2];
	}

	camadas[0].triangulos     = NULL;
	camadas[0].triangulos_cnt = 0;
	camadas[0].linhas         = linhas_grade;
	camadas[0].linhas_cnt     = qtd_linhas_grade;
	camadas[0].textos         = textos_grade;
	camadas[0].textos_cnt     = textos_grade != NULL ? qtd_linhas_grade : 0;

	camadas[1].triangulos     = triangulos;
	camadas[1].triangulos_cnt = 2;
	camadas[1].linhas         = linhas;
	camadas[1].linhas_cnt     = 4 + qtd_linhas_tamanho * resultados_tamanho;
	camadas[1].textos         = textos;
	camadas[1].textos_cnt     = resultados_tamanho;

	renderable = processar_camadas(camadas, 2);

	free((void*) glob_grade);
	free((void*) glob_limites);

	glob_grade   = grade;
	glob_limites = limites;

	if (textos_grade != NULL) {
		unsigned int qtd_eixos = grade->eixos_nx + grade->eixos_ny;
		for (unsigned int i = 0; i < qtd_eixos; ++i) {
			free((void*) textos_grade[i].texto);
			textos_grade[i].texto = NULL;
		}
	}	

	grade   = NULL;
	limites = NULL;

cleanup:
	free((void*) triangulos);
	free((void*) linhas);
	free((void*) textos);

	free((void*) textos_grade);

	free((void*) grade);
	free((void*) limites);
	
	if (camadas != NULL) {
		camadas[0].triangulos     = NULL;
		camadas[0].linhas         = NULL;
		camadas[0].textos         = NULL;
		camadas[0].triangulos_cnt = 0;
		camadas[0].linhas_cnt     = 0;
		camadas[0].textos_cnt     = 0;
		camadas[1].triangulos     = NULL;
		camadas[1].linhas         = NULL;
		camadas[1].textos         = NULL;
		camadas[1].triangulos_cnt = 0;
		camadas[1].linhas_cnt     = 0;
		camadas[1].textos_cnt     = 0;
	}

	free((void*) camadas);
	return renderable;
}
