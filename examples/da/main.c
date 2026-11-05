#include <stdio.h>

#include "da/dynamic_array.h"

static int cmp_int(const void *a, const void *b) {
	return (*(const int *)a - *(const int *)b);
}

static bool is_even(void *elem, void *arg) {
	(void)arg;
	return (*(int *)elem % 2 == 0);
}

static bool print_int(void *elem, void *arg) {
	(void)arg;
	printf(" %d", *(int *)elem);
	return (true);
}

static void *sum_int(void *elem, void *acc) {
	*(int *)acc += *(int *)elem;
	return (acc);
}

int main(void) {
	t_da  da;
	int   seed[] = {5, 3, 8, 1, 9, 2, 7, 4};
	int   needle = 8;
	int   found  = 0;
	void *pos;

	da_init(&da, sizeof(int));
	da_cat_array(&da, seed, sizeof(seed) / sizeof(seed[0]), sizeof(int));

	printf("initial  (%zu elements):", da.nb_e);
	da_apply(&da, print_int, NULL);
	printf("\n");

	da_sort(&da, cmp_int);
	printf("sorted   (%zu elements):", da.nb_e);
	da_apply(&da, print_int, NULL);
	printf("\n");

	pos = da_find(&da, cmp_int, &needle);
	if (pos)
		printf("found %d at index %zu\n", *(int *)pos,
		       (size_t)((int *)pos - (int *)da.array));

	da_fold(&da, sum_int, &found);
	printf("sum = %d\n", found);

	da_reverse(&da);
	printf("reversed (all)     :");
	da_apply(&da, print_int, NULL);
	printf("\n");

	da_nreverse(&da, 3);
	printf("reversed (first 3) :");
	da_apply(&da, print_int, NULL);
	printf("\n");

	da_filter(&da, is_even, NULL, NULL);
	printf("even only          :");
	da_apply(&da, print_int, NULL);
	printf(" (%zu left)\n", da.nb_e);

	da_destroy(&da);
	return (0);
}
