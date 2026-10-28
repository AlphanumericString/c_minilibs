#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

NO_COV void test_idl_copy(void) {
	t_test_dnode  n1, n2;
	t_test_dnode *copy, *copy_next;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 84, 2);
	copy = idl_copy(&n1.dnode, cb_idl_duplicate);

	assert(copy != NULL);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->dnode.next == NULL);
	assert(copy->dnode.prev == NULL);
	free(copy);

	LINK_UP_DNODES(&n1.dnode, &n2.dnode);
	assert(n1.dnode.prev == NULL);
	assert(n2.dnode.next == NULL);

	copy = idl_copy(&n1.dnode, cb_idl_duplicate);
	assert(copy != NULL);
	assert(copy->dnode.offset == n1.dnode.offset);
	copy_next = idl_ntoe(copy->dnode.next);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->dnode.next != NULL);
	assert(copy->dnode.prev == NULL);
	assert(copy_next != NULL);
	assert(copy_next->id == 2);
	assert(copy_next->value == 84);
	assert(copy_next->dnode.next == NULL);
	assert(copy_next->dnode.prev != NULL);
	free(copy);
	free(copy_next);

	assert(idl_copy(&n1.dnode, cb_idl_bad_duplicator) == NULL);
	assert(idl_copy(NULL, cb_idl_duplicate) == NULL);
	assert(idl_copy(&n1.dnode, NULL) == NULL);
}
