#include <stdio.h>

#include "hp/heap.h"

static int cmp_int(const void *a, const void *b) {
	return (*(const int *)a - *(const int *)b);
}

static bool print_int(void *elem, void *arg) {
	(void)arg;
	printf(" %d", *(int *)elem);
	return (true);
}

int main(void) {
	int   values[] = {42, 17, 99, 3, 56, 8, 71, 24};
	t_hp *min      = hp_create(16, sizeof(int), cmp_int);
	t_hp *max      = hp_create(16, sizeof(int), cmp_int);
	int   out;

	for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
		hp_min_push(min, &values[i]);
		hp_max_push(max, &values[i]);
	}

	printf("min-heap storage:");
	hp_foreach(min, print_int, NULL);
	printf("\n");
	printf("max-heap storage:");
	hp_foreach(max, print_int, NULL);
	printf("\n");

	printf("popping ascending :");
	while (hp_min_pop(min, &out))
		printf(" %d", out);
	printf("\n");

	printf("popping descending:");
	while (hp_max_pop(max, &out))
		printf(" %d", out);
	printf("\n");

	hp_free(&min);
	hp_free(&max);
	return (0);
}
