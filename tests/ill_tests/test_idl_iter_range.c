#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_iter_range(void) {
	t_cm_test_dnode n1, n2, n3, n4, iso;
	t_cm_idl       *head = NULL;
	void           *it   = NULL;
	size_t          off  = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	INIT_DNODE(n4, 40, 4);
	INIT_DNODE(iso, 50, 5);
	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);
	idl_push_back_node(&head, &n4.dnode);

	assert(idl_iter_range(NULL, NULL, &it, off) == false);
	assert(it == NULL);
	assert(idl_iter_range(&n1, NULL, NULL, off) == false);

	// empty range
	it = NULL;
	assert(idl_iter_range(&n1, &n1, &it, off) == false);
	assert(it == NULL);

	// full range
	it = NULL;
	assert(idl_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n1);
	assert(idl_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n2);
	assert(idl_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n3);
	assert(idl_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n4);
	assert(idl_iter_range(&n1, NULL, &it, off) == false);
	assert(it == NULL);

	// exhausted iterators restart from the head
	assert(idl_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n1);

	// exclusive end
	it = NULL;
	assert(idl_iter_range(&n1, &n3, &it, off) == true);
	assert(it == &n1);
	assert(idl_iter_range(&n1, &n3, &it, off) == true);
	assert(it == &n2);
	assert(idl_iter_range(&n1, &n3, &it, off) == false);
	assert(it == NULL);

	// arbitrary start
	it = &n2;
	assert(idl_iter_range(&n1, &n4, &it, off) == true);
	assert(it == &n3);
	assert(idl_iter_range(&n1, &n4, &it, off) == false);

	// end reached on the very next step
	it = &n2;
	assert(idl_iter_range(&n1, &n3, &it, off) == false);
	assert(it == NULL);

	// an end outside the list behaves like the tail
	it = NULL;
	assert(idl_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n1);
	assert(idl_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n2);
	assert(idl_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n3);
	assert(idl_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n4);
	assert(idl_iter_range(&n1, &iso, &it, off) == false);
	assert(it == NULL);
}
