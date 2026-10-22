#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "xarray_tests.h"

NO_COV int main(void) {
	intptr_t tests2;
	void    *tests = (void *)0xBEADDEADFEEDCAFE;
	short    chunks;

	tests = malloc(42);
	CM_DBG("%p", tests);
	tests2  = (intptr_t)tests;
	tests2 |= 0xCAFE000000000000;
	for (size_t i = 0; i < (sizeof(tests) / sizeof(chunks)); i++) {
		chunks = (typeof(chunks))tests2;
		tests2 = tests2 >> sizeof(chunks) * 8;
		CM_DBG("chunk: %hx t:%p", chunks, (void *)tests2);
	}
	free(tests);
	return (0);
}
