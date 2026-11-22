#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_init_node(void) {
	t_cm_test_dnode node;

	idl_init_node(NULL, offsetof(t_cm_test_dnode, dnode));
	idl_init_node(&node.dnode, offsetof(t_cm_test_dnode, dnode));

	assert(node.dnode.offset == offsetof(t_cm_test_dnode, dnode));
	assert(node.dnode.next == NULL);
	assert(node.dnode.prev == NULL);
}
