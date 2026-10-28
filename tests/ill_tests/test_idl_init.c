#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_init(void) {
	t_test_dnode node;

	idl_init(NULL, offsetof(t_test_dnode, dnode));
	idl_init(&node.dnode, offsetof(t_test_dnode, dnode));

	assert(node.dnode.offset == offsetof(t_test_dnode, dnode));
	assert(node.dnode.next == NULL);
	assert(node.dnode.prev == NULL);
}
