#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_is_isolated(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_test_node isolated_node;

	INIT_STD_NODES(n1, n2, n3, isolated_node);

	assert(ill_is_isolated(&n1.node) == true);

	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);

	assert(ill_is_isolated(&n1.node) == false);
	assert(ill_is_isolated(&n3.node) == true);
	assert(ill_is_isolated(&isolated_node.node) == true);
	assert(ill_is_isolated(NULL) == false);
}
