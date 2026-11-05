#include <stdio.h>

#include "dsu/cdsu/compact_disjointed_set_union.h"

static void dump(const t_cm_cdsu *d) {
	printf("components=%zu depth=%d :", cdsu_nb_components(d),
	       cdsu_max_depth(d));
	for (size_t i = 0; i < cdsu_size(d); i++)
		printf(" %zu", cdsu_strict_find(d, i));
	printf("\n");
}

int main(void) {
	enum { N = 8 };
	t_cm_cdsu *d = cdsu_new(N);

	printf("fresh dsu\n");
	dump(d);

	cdsu_union(d, 0, 1);
	cdsu_union(d, 2, 3);
	cdsu_union(d, 4, 5);
	printf("after three unions\n");
	dump(d);

	printf("0 and 1 related: %s\n", cdsu_is_related(d, 0, 1) ? "yes" : "no");
	printf("0 and 5 related: %s\n", cdsu_is_related(d, 0, 5) ? "yes" : "no");

	cdsu_union(d, 3, 4);
	cdsu_union(d, 1, 5);
	printf("after merging the groups\n");
	dump(d);

	cdsu_compress_all(d);
	printf("after path compression\n");
	dump(d);

	printf("everything related: %s\n",
	       cdsu_is_related(d, 0, 5) && cdsu_is_related(d, 0, 2) ? "yes" : "no");

	cdsu_free(&d);
	return (0);
}
