#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_advance_it(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;
	t_idl       *it;
	void        *e;
	size_t       off = offsetof(t_test_dnode, dnode);

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	e = idl_advance_it(&n1, off);
	assert(e == &n2);
	e = idl_advance_it(e, off);
	assert(e == &n3);
	e = idl_advance_it(e, off);
	assert(e == NULL);
	assert(idl_advance_it(NULL, off) == NULL);

	e = idl_recede_it(&n3, off);
	assert(e == &n2);
	e = idl_recede_it(e, off);
	assert(e == &n1);
	e = idl_recede_it(e, off);
	assert(e == NULL);
	assert(idl_recede_it(NULL, off) == NULL);

	it = idl_advance_it_node(&n1.dnode);
	assert(it == &n2.dnode);
	it = idl_advance_it_node(it);
	assert(it == &n3.dnode);
	it = idl_advance_it_node(it);
	assert(it == NULL);
	assert(idl_advance_it_node(NULL) == NULL);

	it = idl_recede_it_node(&n3.dnode);
	assert(it == &n2.dnode);
	it = idl_recede_it_node(it);
	assert(it == &n1.dnode);
	it = idl_recede_it_node(it);
	assert(it == NULL);
	assert(idl_recede_it_node(NULL) == NULL);
}
