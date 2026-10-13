#include <stdio.h>
#include <stdlib.h>

#include "./ss/sparse_set.h"

#define DUMP_SS(ss)                                                            \
	do {                                                                       \
		printf("ss (%p):\n"                                                    \
		       "\tmax cap: %zu\n\tcurrent nb: %zu\n"                           \
		       "\tsize elements: %zu\n\t.data loc: %p\n"                       \
		       "\t.indexes loc: %p - current:\n",                              \
		       (ss), (ss)->cappacity, (ss)->nb_elem, (ss)->sz_elem,            \
		       (ss)->data, (ss)->indexes);                                     \
		if ((ss)->indexes) printf("\t");                                       \
		for (size_t i = 0; (ss)->indexes && i < (ss)->cappacity; i++)          \
			printf("[%u]", (ss)->indexes[i]);                                  \
		if ((ss)->indexes) printf("\n");                                       \
		printf("\n");                                                          \
	} while (0);

void display_function_it(void *data, size_t pos, bool last) {
	if (pos == 0) printf("{");
	printf(" %d", *(int *)data);
	if (!last) printf(",");
	else printf(" }\n");
	fflush(stdout);
}
void display_function(void *data) {
	printf("%d ", *(int *)data);
	fflush(stdout);
}

int __attribute__((no_profile_instrument_function)) main(void) {
	{
		size_t i  = 0;
		t_ss   ss = {0};

		DUMP_SS(&ss);
		ss_ninit(&ss, 16, sizeof(int));
		DUMP_SS(&ss);
		while (i != ss.cappacity) {
			ss_add(&ss, &i);
			i++;
		}
		DUMP_SS(&ss);
		ss_apply(&ss, display_function);
		printf("\n");
		ss_apply_it(&ss, display_function_it);
		srand(42);
		size_t nb = rand() % ss.cappacity;
		printf("removing %zu\n", nb);
		ss_remove(&ss, nb);
		ss_apply_it(&ss, display_function_it);
		ss_destroy(&ss);
	}
	return (EXIT_SUCCESS);
}
