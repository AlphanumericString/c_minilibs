#include <stdlib.h>

#include "./dsu/disjointed_set_union.h"
#define DISP_DSU(dsu)                                                          \
	do {                                                                       \
		printf("DSU INFO\n"                                                    \
		       "\t.size:%zu\n"                                                 \
		       "\t.single_alloc:%s\n"                                          \
		       "\t.parent:%p\n"                                                \
		       "\t.rank:%p\n"                                                  \
		       "\t.nb_set:%zu\n",                                              \
		       (dsu).size, (dsu).rp_single_alloc ? "true" : "no", (dsu).parent,   \
		       (dsu).rank, (dsu).components);                                  \
		if (!(dsu).parent || !(dsu).rank) {                                    \
			printf("\tparents - or - rank missing\n");                         \
			break;                                                             \
		}                                                                      \
		printf("\t%8s | %-8s\n", "parent", "rank");                            \
		for (size_t i = 0; i < (dsu).size; i++)                                \
			printf("\t%8d | %-8d\n", (dsu).parent[i], (dsu).rank[i]);          \
	} while (0)

void base_tests(void) {
	{
		t_dsu dsu = (t_dsu){0};

		printf("base:\n");
		DISP_DSU(dsu);

		printf("init:\n");
		dsu_init(&dsu, 5);
		DISP_DSU(dsu);

		printf("union ({0-1-2}{3}{4}):\n");
		dsu_union(&dsu, 0, 1);
		dsu_union(&dsu, 1, 2);
		DISP_DSU(dsu);

		printf("union ({0-1-2}{3-4}):\n");
		dsu_union(&dsu, 3, 4);
		DISP_DSU(dsu);

		printf("union on already linked (0 - 1 - 2 permutations):\n");
		dsu_union(&dsu, 1, 0);
		dsu_union(&dsu, 0, 2);
		dsu_union(&dsu, 1, 2);
		DISP_DSU(dsu);

		printf("destroy:\n");
		dsu_destroy(&dsu);
		DISP_DSU(dsu);
	}
	printf("===\n");
	printf("local buffer\n");
	printf("===\n");
	{
		t_dsu dsu = {0};
		int   buffer[32];

		printf("base:\n");
		DISP_DSU(dsu);

		printf("from arrays:\n");
		dsu_init_from_arrays(&dsu, 5, buffer, &buffer[5]);
		DISP_DSU(dsu);

		printf("union ({0-1-2}{3-4}):\n");
		dsu_union(&dsu, 0, 1);
		dsu_union(&dsu, 1, 2);
		dsu_union(&dsu, 3, 4);
		DISP_DSU(dsu);
	}
	printf("===\n");
	printf("grow / new\n");
	printf("===\n");
	{
		t_dsu *dsu = dsu_new(5);

		DISP_DSU(*dsu);
		dsu_union(dsu, 0, 1);
		dsu_union(dsu, 1, 2);
		dsu_union(dsu, 3, 4);
		DISP_DSU(*dsu);
		dsu_grow(dsu, 3);
		DISP_DSU(*dsu);
	}
}

int main(void) {
	base_tests();
	return (EXIT_SUCCESS);
}
