#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_cm_rdsu_union(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);
	t_cm_rdsu  no_rank, no_parent, no_size;

	assert(rdsu_nb_components(rdsu) == 5); // start state
	assert(rdsu->rank[0] == 0 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 1);
	assert(rdsu->rank[2] == 0 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 3);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	assert(rdsu_union(rdsu, 0, 1) == true); // simple union
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu_nb_components(rdsu) == 4);
	assert(rdsu->rank[0] == 1 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 0 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 3);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	assert(rdsu_union(rdsu, 3, 2) == true); // reversed order, take lower
	assert(rdsu_find(rdsu, 2) == rdsu_find(rdsu, 3));
	assert(rdsu_nb_components(rdsu) == 3);
	assert(rdsu->rank[0] == 1 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	assert(rdsu_union(rdsu, 1, 3) == true); // merging two nets
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 2));
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 3));
	assert(rdsu->rank[0] == 2 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 0);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	assert(rdsu_nb_components(rdsu) == 2);
	assert(rdsu_union(rdsu, 0, 4) == true); // merging single elem to a net
	assert(rdsu->rank[0] == 2 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 0);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 0);
	assert(rdsu_nb_components(rdsu) == 1);

	// net:
	//      0-\-\  : rank 2
	//      | | |
	//      2 | |  : rank 1
	//      | | |
	//      3 1 4  : rank 0

	rdsu_clr(rdsu);
	no_parent = *rdsu;
	no_rank   = *rdsu;
	no_size   = *rdsu;

	no_parent.parent = NULL;
	no_size.size     = 0;
	no_rank.rank     = NULL;
	assert(rdsu_union(&no_parent, 0, 1) == false);
	assert(rdsu_nb_components(&no_parent) == rdsu_size(&no_parent));

	assert(rdsu_union(&no_rank, 0, 1) == false);
	assert(rdsu_nb_components(&no_rank) == rdsu_size(&no_rank));

	assert(rdsu_union(&no_size, 0, 1) == false);
	assert(no_size.components == rdsu->size);

	assert(rdsu_union(NULL, 1, 3) == false);
	assert(rdsu_union(rdsu, 32, 3) == false);
	assert(rdsu_union(rdsu, 1, 32) == false);
	assert(rdsu_union(rdsu, 1, 1) == false);

	alloc_set_call_limit(0);
	assert(rdsu_union(rdsu, 0, 1) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	rdsu_free(&rdsu);
}

NO_COV void test_cm_rdsu_find(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);
	t_cm_rdsu  intact_copy;

	assert(rdsu_find(rdsu, 0) == 0);
	assert(rdsu_find(rdsu, 1) == 1);
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	rdsu_rewind(rdsu);
	assert(rdsu_find(rdsu, 0) != rdsu_find(rdsu, 1));

	intact_copy  = *rdsu;
	rdsu->parent = NULL;
	assert(rdsu_find(rdsu, 0) == (uint16_t)-1);
	*rdsu      = intact_copy;
	rdsu->rank = NULL;
	assert(rdsu_find(rdsu, 0) == (uint16_t)-1);
	*rdsu      = intact_copy;
	rdsu->size = 0;
	assert(rdsu_find(rdsu, 0) == (uint16_t)-1);
	*rdsu = intact_copy;
	assert(rdsu_find(NULL, 0) == (uint16_t)-1);
	assert(rdsu_find(rdsu, 99) == (uint16_t)-1);

	rdsu_free(&rdsu);
}

NO_COV void test_cm_rdsu_compress_all(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);
	t_cm_rdsu  intact_copy;

	rdsu->rank[0]   = 42;
	rdsu->parent[0] = 1;
	rdsu->parent[1] = 2;
	rdsu->parent[2] = 3;
	rdsu->parent[3] = 3;
	rdsu_union(rdsu, 3, 4);
	assert(rdsu->change_log != NULL);
	assert(rdsu_compress_all(rdsu) == 5);
	assert(rdsu->change_log == NULL);
	assert(rdsu->rank[0] == 42);
	assert(rdsu->parent[0] == 2);
	assert(rdsu->parent[1] == 3);
	assert(rdsu->parent[2] == 3);
	assert(rdsu->parent[3] == 3);
	assert(rdsu->parent[4] == 3);
	assert(rdsu_compress_all(rdsu) == 5);
	assert(rdsu->rank[0] == 42);
	assert(rdsu->parent[0] == 3);
	assert(rdsu->parent[1] == 3);
	assert(rdsu->parent[2] == 3);
	assert(rdsu->parent[3] == 3);
	assert(rdsu->parent[4] == 3);

	intact_copy = *rdsu;

	rdsu->size = 0;
	assert(rdsu_compress_all(rdsu) == 0);
	*rdsu        = intact_copy;
	rdsu->parent = NULL;
	assert(rdsu_compress_all(rdsu) == 0);
	assert(rdsu_compress_all(NULL) == 0);
	*rdsu = intact_copy;
	assert(rdsu_compress_all(rdsu) == 5);

	rdsu_free(&rdsu);
}
