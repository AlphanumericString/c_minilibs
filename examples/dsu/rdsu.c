#include <stdio.h>

#include "dsu/rdsu/rewindable_disjointed_set_union.h"

static void dump(const t_rdsu *d) {
	printf("components=%zu :", rdsu_nb_components(d));
	for (size_t i = 0; i < rdsu_size(d); i++)
		printf(" %u", rdsu_find(d, i));
	printf("\n");
}

int main(void) {
	t_rdsu     *d = rdsu_new(6);
	t_rdsu_log *checkpoint;

	rdsu_union(d, 0, 1);
	rdsu_union(d, 1, 2);
	printf("0-1-2 linked\n");
	dump(d);

	checkpoint = rdsu_save_state(d);
	printf("checkpoint saved\n");

	rdsu_union(d, 3, 4);
	rdsu_union(d, 2, 4);
	printf("more links (0..4 now one group)\n");
	dump(d);

	rdsu_rewind_to_state(d, checkpoint);
	printf("rewound to checkpoint\n");
	dump(d);

	rdsu_union(d, 5, 0);
	printf("linked 5 to the group\n");
	dump(d);

	rdsu_rewind(d);
	printf("rewound one step\n");
	dump(d);

	rdsu_free(&d);
	return (0);
}
