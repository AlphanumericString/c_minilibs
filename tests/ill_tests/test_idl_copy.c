#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

static int loc_count = 0;

NO_COV static void *foo(const void *e) {
	static t_test_dnode no_leak = {0};

	(void)e;
	if (loc_count++ < 1) return (&no_leak);
	return (NULL);
}

NO_COV static void copy_cleanup_on_failure(void) {
	t_test_dnode n1, n2, n3;
	void        *head;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 84, 2);
	INIT_DNODE(n3, 126, 3);
	LINK_UP_DNODES(&n1.dnode, &n2.dnode);
	LINK_UP_DNODES(&n2.dnode, &n3.dnode);

	g_dup_hits    = 0;
	g_dup_fail_at = 2;
	head          = idl_copy(&n1.dnode, cb_idl_duplicate_fail_at, cb_free_elem);
	assert(head == NULL);
	g_dup_fail_at = -1;
	g_dup_hits    = 0;

	head = idl_copy(&n1.dnode, foo, NULL);
	assert(head == NULL);
}

NO_COV void test_idl_copy(void) {
	t_test_dnode  n1, n2;
	t_test_dnode *copy, *copy_next;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 84, 2);
	copy = idl_copy(&n1.dnode, cb_idl_duplicate, cb_free_elem);

	assert(copy != NULL);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->dnode.next == NULL);
	assert(copy->dnode.prev == NULL);
	free(copy);

	LINK_UP_DNODES(&n1.dnode, &n2.dnode);
	assert(n1.dnode.prev == NULL);
	assert(n2.dnode.next == NULL);

	copy = idl_copy(&n1.dnode, cb_idl_duplicate, cb_free_elem);
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

	assert(idl_copy(&n1.dnode, cb_bad_duplicator, cb_free_elem) == NULL);
	assert(idl_copy(NULL, cb_idl_duplicate, cb_free_elem) == NULL);
	assert(idl_copy(&n1.dnode, NULL, cb_free_elem) == NULL);

	copy = idl_copy(&n1.dnode, cb_idl_duplicate, NULL);
	assert(copy != NULL);
	free(idl_ntoe(copy->dnode.next));
	free(copy);

	copy_cleanup_on_failure();
}
