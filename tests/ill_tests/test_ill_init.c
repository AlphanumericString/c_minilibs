#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_init(void) {
	t_cm_test_node node;

	ill_init(&node.node, offsetof(t_cm_test_node, node));
	ill_init(NULL, offsetof(t_cm_test_node, node));

	assert(node.node.offset == offsetof(t_cm_test_node, node));
	assert(node.node.next == NULL);
}
