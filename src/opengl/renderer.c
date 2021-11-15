
#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <string.h> // memcpy, strlen

#define __GRAFICO_IMPL
	#include "opengl-pch.h"
	#include "renderer_defs.h"
	#include "grafico.h"
#undef __GRAFICO_IMPL

#include "../resultados.h"

// janela glut principal
int WINDOW;

// tamanho da tela
const unsigned int WIDTH  = 850;
const unsigned int HEIGHT = 600;

// posição e tamanho do gráfico
const unsigned int X_PADDING = 115;
const unsigned int Y_PADDING = 90;
const unsigned int GRAFICO_LARGURA = WIDTH  - 2 * X_PADDING;
const unsigned int GRAFICO_ALTURA  = HEIGHT - 2 * Y_PADDING;
const unsigned int GRAFICO_POSX    = X_PADDING;
const unsigned int GRAFICO_POSY    = Y_PADDING;

// dados de renderização
RenderableData* glob_render_data;

// coordenadas do mouse
int mouse_x, mouse_y;
void ler_mouse(int x, int y) {
	mouse_x = x;
	mouse_y = y;
}

void renderizar() {
	// limpa a tela
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// renderiza linhas e triangulos
	glInterleavedArrays(GL_C3F_V3F, sizeof(Vertice), glob_render_data->vertices);
	glDrawArrays(GL_TRIANGLES,
		glob_render_data->triangulos_offset,
		glob_render_data->triangulos_tamanho
	);
	glDrawArrays(GL_LINES,
		glob_render_data->linhas_offset,
		glob_render_data->linhas_tamanho
	);

	// renderiza os textos
	const UTexto* textos = glob_render_data->textos;
	for (unsigned int i = 0; i < glob_render_data->textos_cnt; ++i) {
		float offset = 0;
		unsigned int factor = 1;
		const float (*pos)[3] = &textos[i].vert.pos;
		
		switch (textos[i].alinhamento) {
			case LEFT_ALIGN:
				factor = 2;
			case CENTER_ALIGN:
				offset = -((float) factor) * glutBitmapLength(
					GLUT_BITMAP_8_BY_13, textos[i].texto
				) / (float) WIDTH;
			case RIGHT_ALIGN:
				glColor3fv(textos[i].vert.cor);
				glRasterPos3f((*pos)[0] + offset, (*pos)[1], (*pos)[2]);
				glutBitmapString(GLUT_BITMAP_8_BY_13, textos[i].texto);
				break;
		}
	}

	// renderiza o mouse
	renderizar_mouse(mouse_x, mouse_y);

	// troca os buffers internos (double buffering)
	// e sinaliza que é preciso renderizar a tela
	// novamente
	glutSwapBuffers();
	glutPostRedisplay();
}

int opengl_resultados_algoritimos(ResultadoAlgoritimo* resultados,
		const unsigned int* qtd_linhas, unsigned int resultados_tamanho,
		unsigned int qtd_linhas_tamanho) {
		
	glob_render_data = gerar_grafico(resultados,
			qtd_linhas, resultados_tamanho, qtd_linhas_tamanho);

	// retorna erro se houver
	return (glob_render_data != NULL) ? 0 : -1;
}

int iniciar_opengl(int* argc, char** argv) {

	// ==================== GLUT INIT ====================

	// init glut
	glutInit(argc, argv);

	// parametros do glut (double buffering + cor RGB + profundidade)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);

	// cria a janela principal
	WINDOW = glutCreateWindow("benchmark");

	// cofigura a funcao de desenho
	glutDisplayFunc(renderizar);
	glutMotionFunc(ler_mouse);
	glutPassiveMotionFunc(ler_mouse);

	// ==================== GLEW INIT ====================

	// inicia o glew
	int error = glewInit();
	if (error != GLEW_NO_ERROR) {
		const unsigned char* msg = glewGetErrorString(error);
		fprintf(stderr, "erro: falha na inicializacao do GLEW\n");
		fprintf(stderr, "GLEW error: %s\n", msg);
		return -1;
	}

	// -------------- CONFIGURAÇÃO DO OPENGL -------------

	// abilita profundidade
	// e linhas suaves
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LINE_SMOOTH);

	// espessura das linhas
	glLineWidth(1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glInterleavedArrays(GL_C3F_V3F, sizeof(Vertice), NULL);

	return 0;
}

void mainloop_opengl() {
	// main loop
	glutMainLoop();
}
