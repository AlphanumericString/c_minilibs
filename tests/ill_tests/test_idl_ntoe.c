#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_ntoe(void) {
	t_test_dnode node;

	idl_init(&node.dnode, offsetof(t_test_dnode, dnode));

	t_test_dnode *elem = idl_ntoe(&node.dnode);

	assert(elem == &node);
	assert(idl_ntoe(NULL) == NULL);
}
