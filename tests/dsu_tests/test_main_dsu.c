#include "dsu_tests.h"

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
	// base_tests();
	test_dsu_new();
	test_dsu_new_from_array();
	test_dsu_init();
	test_dsu_init_from_arrays();
	test_dsu_grow();
	test_dsu_clr();
	test_dsu_destroy();
	test_dsu_max_depth();
	test_dsu_size();
	test_dsu_nb_components();
	test_dsu_is_related();
	test_dsu_strict_is_related();
	test_dsu_validate();
	test_dsu_union();
	test_dsu_find();
	test_dsu_strict_find();
	test_dsu_compress_all();
	printf("All da tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
