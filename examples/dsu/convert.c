#include <stdio.h>

#include "dsu/dsu_convert/dsu_convertions.h"

static void dump_cdsu(const t_cm_cdsu *d) {
	printf("cdsu components=%zu :", cdsu_nb_components(d));
	for (size_t i = 0; i < cdsu_size(d); i++)
		printf(" %zu", cdsu_strict_find(d, i));
	printf("\n");
}

static void dump_rdsu(const t_cm_rdsu *d) {
	printf("rdsu components=%zu :", rdsu_nb_components(d));
	for (size_t i = 0; i < rdsu_size(d); i++)
		printf(" %u", rdsu_find(d, i));
	printf("\n");
}

int main(void) {
	t_cm_cdsu *cdsu = cdsu_new(6);

	cdsu_union(cdsu, 0, 1);
	cdsu_union(cdsu, 2, 3);
	cdsu_union(cdsu, 3, 4);
	printf("source compact dsu\n");
	dump_cdsu(cdsu);

	t_cm_rdsu *rdsu = cdsu_to_rdsu(&cdsu);
	printf("converted to rewindable dsu (source consumed: %s)\n",
	       cdsu == NULL ? "yes" : "no");
	dump_rdsu(rdsu);

	rdsu_union(rdsu, 1, 5);
	printf("one more link on the rewindable side\n");
	dump_rdsu(rdsu);

	cdsu = rdsu_to_cdsu(&rdsu);
	printf("converted back to compact (source consumed: %s)\n",
	       rdsu == NULL ? "yes" : "no");
	dump_cdsu(cdsu);

	cdsu_free(&cdsu);
	return (0);
}
