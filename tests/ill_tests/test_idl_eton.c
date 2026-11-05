#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_eton(void) {
	t_cm_test_dnode node;

	idl_init(&node.dnode, offsetof(t_cm_test_dnode, dnode));

	assert(idl_eton(&node, offsetof(t_cm_test_dnode, dnode)) == &node.dnode);
	assert(idl_eton(NULL, 99) == NULL);
}
