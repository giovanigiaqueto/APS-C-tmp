
#include "lista_linhas.h"

typedef enum { ALG_VETOR, ALG_LISTA } TipoAlgoritimo;

typedef int (*alg_vetor_fptr)(char**,       unsigned int);
typedef int (*alg_lista_fptr)(ListaLinhas*, unsigned int);

typedef union FuncAlgoritimo {
	alg_vetor_fptr alg_vetor;
	alg_lista_fptr alg_lista;
} FuncAlgoritimo;

typedef struct Algoritimo {
	TipoAlgoritimo tipo;
	FuncAlgoritimo func;
} Algoritimo;
