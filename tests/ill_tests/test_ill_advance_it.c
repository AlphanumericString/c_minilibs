#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_advance_it(void) {
	t_test_node n1, n2, n3;
	t_ill      *it;
	void       *e;
	size_t      off = offsetof(t_test_node, node);

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);
	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);

	e = ill_advance_it(&n1, off);
	assert(e == &n2);
	e = ill_advance_it(e, off);
	assert(e == &n3);
	e = ill_advance_it(e, off);
	assert(e == NULL);
	assert(ill_advance_it(NULL, off) == NULL);

	it = ill_advance_it_node(&n1.node);
	assert(it == &n2.node);
	it = ill_advance_it_node(it);
	assert(it == &n3.node);
	it = ill_advance_it_node(it);
	assert(it == NULL);
	assert(ill_advance_it_node(NULL) == NULL);
}
