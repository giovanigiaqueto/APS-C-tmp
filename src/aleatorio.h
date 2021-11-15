
#include <stdlib.h>
#include <limits.h>

#if ULONG_MAX > UINT_MAX
static inline long randlong() {
	return (((long) rand()) & ((long) ~0)) | (((long) rand()) << (8 * sizeof(int)));
}
#define __MY_LONG_RAND_MAX ((((long) RAND_MAX) & ((long) ~0)) | (((long) RAND_MAX) << (8 * sizeof(int))))
#else

static inline long randlong() {
	return (long) rand();
}
#define __MY_LONG_RAND_MAX ((long) RAND_MAX)
#endif

int gerar_randint(int min, int max);
void embaralhar(void* buff, size_t tamanho, size_t tamanho_buff);

void inic_aleatorio();
