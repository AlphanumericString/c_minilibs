#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_is_circular(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_test_node isolated_node;

	INIT_STD_NODES(n1, n2, n3, isolated_node);

	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);

	assert(ill_is_circular(&n1.node) == false);
	n3.node.next = &n1.node;
	assert(ill_is_circular(&n1.node) == true);
	assert(ill_is_circular(&n2.node) == true);
	assert(ill_is_circular(&n3.node) == true);
	isolated_node.node.next = &isolated_node.node;
	assert(ill_is_circular(&isolated_node.node) == true);
	isolated_node.node.next = NULL;
	assert(ill_is_circular(&isolated_node.node) == false);
	assert(ill_is_circular(NULL) == false);
	n3.node.next = NULL;
}
