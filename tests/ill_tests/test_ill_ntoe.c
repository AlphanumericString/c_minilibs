#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_ntoe(void) {
	t_cm_test_node node;

	ill_init(&node.node, offsetof(t_cm_test_node, node));

	assert(ill_ntoe(&node.node) == &node);
	assert(ill_ntoe(NULL) == NULL);
}
