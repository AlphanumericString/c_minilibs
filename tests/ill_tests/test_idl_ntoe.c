#include <assert.h>

#include "ill_tests.h"

void test_idl_ntoe(void) {
	t_test_dnode node;

	idl_init(&node.dnode, offsetof(t_test_dnode, dnode));

	t_test_dnode *elem = idl_ntoe(&node.dnode);

	assert(elem == &node);

	t_idl *node_ptr = idl_eton(&node, offsetof(t_test_dnode, dnode));

	assert(node_ptr == &node.dnode);
}
