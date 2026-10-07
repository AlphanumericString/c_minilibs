#include <assert.h>
#include <dsu/rdsu/rewindable_disjointed_set_union.h>

#include "rdsu_tests.h"

void test_rdsu_union(void) {
	t_rdsu *rdsu = rdsu_new(5);
	t_rdsu  intact_copy;

	assert(rdsu_nb_components(*rdsu) == 5); // start state
	assert(rdsu->rank[0] == 0 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 1);
	assert(rdsu->rank[2] == 0 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 3);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	rdsu_union(rdsu, 0, 1); // simple union
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu_nb_components(*rdsu) == 4);
	assert(rdsu->rank[0] == 1 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 0 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 3);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	rdsu_union(rdsu, 3, 2); // reversed order to show always lower taken
	assert(rdsu_find(rdsu, 2) == rdsu_find(rdsu, 3));
	assert(rdsu_nb_components(*rdsu) == 3);
	assert(rdsu->rank[0] == 1 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 2);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	rdsu_union(rdsu, 1, 3); // merging two nets
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 2));
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 3));
	assert(rdsu->rank[0] == 2 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 0);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 4);
	assert(rdsu_nb_components(*rdsu) == 2);
	rdsu_union(rdsu, 0, 4); // merging single elem to a net
	assert(rdsu->rank[0] == 2 && rdsu->parent[0] == 0);
	assert(rdsu->rank[1] == 0 && rdsu->parent[1] == 0);
	assert(rdsu->rank[2] == 1 && rdsu->parent[2] == 0);
	assert(rdsu->rank[3] == 0 && rdsu->parent[3] == 2);
	assert(rdsu->rank[4] == 0 && rdsu->parent[4] == 0);
	assert(rdsu_nb_components(*rdsu) == 1);
	rdsu_clr(rdsu);

	// net:
	//      0-\-\  : rank 2
	//      | | |
	//      2 | |  : rank 1
	//      | | |
	//      3 1 4  : rank 0

	intact_copy  = *rdsu;
	rdsu->parent = NULL;
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(*rdsu) == rdsu_size(*rdsu));
	*rdsu      = intact_copy;
	rdsu->rank = NULL;
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(*rdsu) == rdsu_size(*rdsu));
	*rdsu      = intact_copy;
	rdsu->size = 0;
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(*rdsu) == intact_copy.size);
	*rdsu = intact_copy;

	rdsu_union(NULL, 1, 3);
	rdsu_union(rdsu, 32, 3);
	rdsu_union(rdsu, 1, 32);
	rdsu_union(rdsu, 1, 1);

	rdsu_free(&rdsu);
}

void test_rdsu_find(void) {
	t_rdsu *rdsu = rdsu_new(5);
	t_rdsu  intact_copy;

	assert(rdsu_find(rdsu, 0) == 0);
	assert(rdsu_find(rdsu, 1) == 1);
	rdsu_union(rdsu, 0, 1);
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

void test_rdsu_compress_all(void) {
	t_rdsu *rdsu = rdsu_new(5);
	t_rdsu  intact_copy;

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
