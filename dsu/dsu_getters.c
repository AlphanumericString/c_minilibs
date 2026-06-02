#include "./disjointed_set_union.h"

int dsu_max_depth(t_dsu dsu) {
	unsigned int max = 0;
	size_t       i   = 0;

	while (i < dsu.size) {
		if (dsu.rank[i++] > max)
			max = dsu.rank[i - 1];
	}
	return (max);
}
size_t dsu_size(t_dsu dsu) {
	return (dsu.size);
}
size_t dsu_nb_components(t_dsu dsu) {
	return (dsu.components);
}
