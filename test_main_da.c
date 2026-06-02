#include <stdio.h>

#include "./da/dynamic_array.h"

#define DISP_VECT_INFOS(v)                                                     \
	printf(".data\t%p\n.sz_e\t%zu\n.capp_e\t%zu\n.nb_e\t%zu\n\n", v.array,     \
	       v.sz_e, v.cap_e, v.nb_e)

void display_function_it(void *data, size_t pos, bool last) {
	int *data_as_int = (int *)data;

	if (pos == 0) printf("{");
	printf(" %d", *data_as_int);
	if (!last) printf(",");
	else printf(" }\n");
	fflush(stdout);
}
void display_function(void *data) {
	int *data_as_int = (int *)data;

	printf("%d ", *data_as_int);
	fflush(stdout);
}

int test_rm(void) {
	printf("====\n");
	printf("====\n");
	printf("RM_!ORDER\n");
	printf("====\n");
	printf("====\n");
	t_da vect2;
	int arr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
	int arr2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

	da_init(&vect2, sizeof(int));
	da_cat_array(&vect2, arr2, sizeof(arr2) / sizeof(arr2[0]), sizeof(arr2[0]));
	da_apply_it(&vect2, display_function_it);

	printf("====\n");
	for (size_t pos = rand() % vect2.nb_e; vect2.nb_e > 2;
	     pos        = rand() % vect2.nb_e) {
		printf("rm @ pos %zu\n", pos);
		da_rm(&vect2, pos);
		da_apply_it(&vect2, display_function_it);
	}
	printf("====\n");
	da_cat_array(&vect2, arr2, sizeof(arr2) / sizeof(arr2[0]), sizeof(arr2[0]));
	da_apply_it(&vect2, display_function_it);
	printf("rm from @ %zu to %zu\n", 4ul, 6ul);
	da_rms(&vect2, 4, 6);
	da_apply_it(&vect2, display_function_it);

	da_destroy(&vect2);
	DISP_VECT_INFOS(vect2);

	da_init(&vect2, sizeof(int));
	da_cat_array(&vect2, arr1, sizeof(arr1) / sizeof(arr1[0]), sizeof(arr1[0]));
	DISP_VECT_INFOS(vect2);
	da_apply_it(&vect2, display_function_it);
	printf("rm from @ %zu to %zu\n", vect2.nb_e - 4, vect2.nb_e - 1);
	da_rms(&vect2, vect2.nb_e - 4, vect2.nb_e - 1);
	DISP_VECT_INFOS(vect2);
	da_apply_it(&vect2, display_function_it);
	da_destroy(&vect2);
	return (0);
}

int test_rm_order(void) {
	printf("====\n");
	printf("====\n");
	printf("RM_ORDER\n");
	printf("====\n");
	printf("====\n");
	t_da dyn_arr3;
	int arr2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

	da_init(&dyn_arr3, sizeof(int));
	da_cat_array(&dyn_arr3, arr2, sizeof(arr2) / sizeof(arr2[0]),
	             sizeof(arr2[0]));
	da_apply_it(&dyn_arr3, display_function_it);

	printf("====\n");
	for (size_t pos = rand() % dyn_arr3.nb_e; dyn_arr3.nb_e > 2;
	     pos        = rand() % dyn_arr3.nb_e) {
		printf("rm @ pos %zu\n", pos);
		da_rm_order(&dyn_arr3, pos);
		da_apply_it(&dyn_arr3, display_function_it);
	}
	printf("====\n");
	da_cat_array(&dyn_arr3, arr2, sizeof(arr2) / sizeof(arr2[0]),
	             sizeof(arr2[0]));
	da_apply_it(&dyn_arr3, display_function_it);
	printf("rm from @ %zu to %zu\n", 4ul, 6ul);
	da_rms_order(&dyn_arr3, 4, 6);
	da_apply_it(&dyn_arr3, display_function_it);
	da_destroy(&dyn_arr3);
	DISP_VECT_INFOS(dyn_arr3);
	return (0);
}

void tests_basics(void) {
	t_da vect  = {0};
	int  y     = -1;
	int  x     = 324;
	int  arr[] = {243, 432523, 2234523};

	printf("base:\n");
	DISP_VECT_INFOS(vect);

	printf("init:\n");
	da_init(&vect, sizeof(int));
	DISP_VECT_INFOS(vect);

	printf("add:\n");
	da_add(&vect, &x);
	DISP_VECT_INFOS(vect);

	printf("y = %d\n", y);
	printf("get:\n");
	da_get(&vect, 0, &y);
	printf("y = %d\n", y);
	DISP_VECT_INFOS(vect);

	{
		printf("clr:\n");
		da_clr(&vect);
		DISP_VECT_INFOS(vect);

		y = -1;
		printf("y = %d\n", y);
		printf("get:\n");
		da_get(&vect, 0, &y);
		printf("y = %d\n", y);
		DISP_VECT_INFOS(vect);

		printf("destroy:\n");
		da_destroy(&vect);
		DISP_VECT_INFOS(vect);
	}

	printf("cat_array:\n");
	da_cat_array(&vect, arr, sizeof(arr) / sizeof(arr[0]), sizeof(arr[0]));
	DISP_VECT_INFOS(vect);

	printf("apply:\n");
	da_apply(&vect, display_function);
	printf("\n");
	DISP_VECT_INFOS(vect);

	printf("apply_it:\n");
	da_apply_it(&vect, display_function_it);
	DISP_VECT_INFOS(vect);

	printf("destroy:\n");
	da_destroy(&vect);
	DISP_VECT_INFOS(vect);
}

int main(void) {
	test_rm();
	test_rm_order();
}
