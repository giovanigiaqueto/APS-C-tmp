
#ifndef __GRAFICO_IMPL
	#error opengl-pch.h so deve ser incluido por grafico.c
#endif

#ifndef __OPENGL_PRECOMPILED_HEADER
#define __OPENGL_PRECOMPILED_HEADER

// --- windows ---
#include "so_defs.h"
#ifdef WINDOWS_OS
	#include <windows.h>
#endif

// --- OpenGL ---
#include <GL/glew.h>
#include <GL/freeglut.h>

#endif
