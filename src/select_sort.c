
#include "select_sort.h"

#include <string.h>

void ordenar_select_sort(char** linhas, unsigned int qtd_linhas){
  unsigned int menor;
	char* aux;

  for (int i = 0; i < qtd_linhas; i++) {
    menor = i;
    for (int j = i + 1; j < qtd_linhas; j++) {
      if (strcmp(linhas[menor], linhas[j]) > 0)
        menor = j;
    }
    if (i != menor) {
      aux = linhas[i];
      linhas[i] = linhas[menor];
      linhas[menor] = aux;
    }
  }
}