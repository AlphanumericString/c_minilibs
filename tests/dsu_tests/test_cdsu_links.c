#include <assert.h>

#include "cdsu_tests.h"

NO_COV void test_cm_cdsu_union(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);
	t_cm_cdsu  no_rank, no_parents, no_size;

	assert(cdsu_nb_components(cdsu) == 5); // start state
	assert(cdsu->rank[0] == 0 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 1);
	assert(cdsu->rank[2] == 0 && cdsu->parent[2] == 2);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 3);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	assert(cdsu_union(cdsu, 0, 1) == true); // simple union
	assert(cdsu_strict_find(cdsu, 0) == cdsu_strict_find(cdsu, 1));
	assert(cdsu_nb_components(cdsu) == 4);
	assert(cdsu->rank[0] == 1 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 0 && cdsu->parent[2] == 2);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 3);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	assert(cdsu_union(cdsu, 3, 2) ==
	       true); // reversed order to show always lower taken
	assert(cdsu_strict_find(cdsu, 0) == cdsu_strict_find(cdsu, 1));
	assert(cdsu_strict_find(cdsu, 2) == cdsu_strict_find(cdsu, 3));
	assert(cdsu_nb_components(cdsu) == 3);
	assert(cdsu->rank[0] == 1 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 1 && cdsu->parent[2] == 2);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	assert(cdsu_union(cdsu, 1, 3) == true); // merging two nets
	assert(cdsu_strict_find(cdsu, 0) == cdsu_strict_find(cdsu, 1));
	assert(cdsu_strict_find(cdsu, 0) == cdsu_strict_find(cdsu, 2));
	assert(cdsu_strict_find(cdsu, 0) == cdsu_strict_find(cdsu, 3));
	assert(cdsu->rank[0] == 2 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 1 && cdsu->parent[2] == 0);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	assert(cdsu_nb_components(cdsu) == 2);
	assert(cdsu_union(cdsu, 0, 4) == true); // merging single elem to a net
	assert(cdsu->rank[0] == 2 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 1 && cdsu->parent[2] == 0);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 0);
	assert(cdsu_nb_components(cdsu) == 1);

	// net:
	//      0-\-\  : rank 2
	//      | | |
	//      2 | |  : rank 1
	//      | | |
	//      3 1 4  : rank 0

	assert(cdsu_union(NULL, 1, 3) == false);
	assert(cdsu_union(cdsu, 32, 3) == false);
	assert(cdsu_union(cdsu, 1, 32) == false);
	assert(cdsu_union(cdsu, 1, 1) == false);

	no_rank    = *cdsu;
	no_size    = *cdsu;
	no_parents = *cdsu;

	no_rank.rank      = NULL;
	no_parents.parent = NULL;
	no_size.size      = 0;
	assert(cdsu_union(&no_rank, 1, 2) == false);
	assert(cdsu_union(&no_parents, 1, 2) == false);
	assert(cdsu_union(&no_size, 1, 2) == false);

	cdsu_free(&cdsu);
}

NO_COV void test_cm_cdsu_find(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);
	t_cm_cdsu  intact_copy;

	assert(cdsu_find(cdsu, 0) == 0);
	assert(cdsu_find(cdsu, 1) == 1);
	assert(cdsu_find(cdsu, 2) == 2);
	assert(cdsu_find(cdsu, 3) == 3);
	assert(cdsu_find(cdsu, 4) == 4);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_find(cdsu, 0) == 0);
	assert(cdsu_find(cdsu, 1) == 0);
	assert(cdsu_find(cdsu, 2) == 2);
	assert(cdsu_find(cdsu, 3) == 3);
	assert(cdsu_find(cdsu, 4) == 4);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 1));
	assert(cdsu->rank[0] == 1 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 0 && cdsu->parent[2] == 2);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 3);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	cdsu->parent[0] = 0;
	cdsu->parent[1] = 0;
	cdsu->parent[2] = 1;
	cdsu->parent[3] = 2;
	cdsu->rank[2]   = 99;
	cdsu->rank[0]   = 24;
	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 1);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);
	assert(cdsu_find(cdsu, 0) == 0);
	assert(cdsu_find(cdsu, 1) == 0);
	assert(cdsu_find(cdsu, 2) == 0);
	assert(cdsu_find(cdsu, 3) == 0);
	assert(cdsu_find(cdsu, 4) == 4);
	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 0);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 0);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);

	assert(cdsu_find(cdsu, 99) == (size_t)-1);
	intact_copy  = *cdsu;
	cdsu->parent = NULL;
	assert(cdsu_find(cdsu, 0) == (size_t)-1);
	*cdsu      = intact_copy;
	cdsu->size = 0;
	assert(cdsu_find(cdsu, 0) == (size_t)-1);
	assert(cdsu_find(NULL, 99) == (size_t)-1);
	*cdsu = intact_copy;
	cdsu_free(&cdsu);
}

NO_COV void test_cm_cdsu_strict_find(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);
	t_cm_cdsu  intact_copy;

	assert(cdsu_strict_find(cdsu, 0) == 0);
	assert(cdsu_strict_find(cdsu, 1) == 1);
	assert(cdsu_strict_find(cdsu, 2) == 2);
	assert(cdsu_strict_find(cdsu, 3) == 3);
	assert(cdsu_strict_find(cdsu, 4) == 4);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_strict_find(cdsu, 0) == 0);
	assert(cdsu_strict_find(cdsu, 1) == 0);
	assert(cdsu_strict_find(cdsu, 2) == 2);
	assert(cdsu_strict_find(cdsu, 3) == 3);
	assert(cdsu_strict_find(cdsu, 4) == 4);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 1));
	assert(cdsu->rank[0] == 1 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 0 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 0 && cdsu->parent[2] == 2);
	assert(cdsu->rank[3] == 0 && cdsu->parent[3] == 3);
	assert(cdsu->rank[4] == 0 && cdsu->parent[4] == 4);
	cdsu->parent[0] = 0;
	cdsu->parent[1] = 0;
	cdsu->parent[2] = 1;
	cdsu->parent[3] = 2;
	cdsu->rank[2]   = 99;
	cdsu->rank[0]   = 24;
	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 1);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);
	assert(cdsu_strict_find(cdsu, 0) == 0);
	assert(cdsu_strict_find(cdsu, 1) == 0);
	assert(cdsu_strict_find(cdsu, 2) == 0);
	assert(cdsu_strict_find(cdsu, 3) == 0);
	assert(cdsu_strict_find(cdsu, 4) == 4);
	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 1);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);

	assert(cdsu_strict_find(cdsu, 99) == (size_t)-1);
	intact_copy  = *cdsu;
	cdsu->parent = NULL;
	assert(cdsu_strict_find(cdsu, 0) == (size_t)-1);
	*cdsu      = intact_copy;
	cdsu->size = 0;
	assert(cdsu_strict_find(cdsu, 0) == (size_t)-1);
	*cdsu = intact_copy;
	cdsu_free(&cdsu);
}

NO_COV void test_cm_cdsu_compress_all(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);
	t_cm_cdsu  intact_copy;

	cdsu->parent[0] = 0;
	cdsu->parent[1] = 0;
	cdsu->parent[2] = 1;
	cdsu->parent[3] = 2;
	cdsu->rank[2]   = 99;
	cdsu->rank[0]   = 24;

	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 1);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 2);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);

	assert(cdsu_compress_all(cdsu) == cdsu_size(cdsu));

	assert(cdsu->rank[0] == 24 && cdsu->parent[0] == 0);
	assert(cdsu->rank[1] == 00 && cdsu->parent[1] == 0);
	assert(cdsu->rank[2] == 99 && cdsu->parent[2] == 0);
	assert(cdsu->rank[3] == 00 && cdsu->parent[3] == 0);
	assert(cdsu->rank[4] == 00 && cdsu->parent[4] == 4);

	assert(cdsu_compress_all(NULL) == 0);
	intact_copy  = *cdsu;
	cdsu->parent = NULL;
	assert(cdsu_compress_all(cdsu) == 0);
	*cdsu      = intact_copy;
	cdsu->size = 0;
	assert(cdsu_compress_all(cdsu) == 0);
	*cdsu = intact_copy;

	cdsu_free(&cdsu);
}
