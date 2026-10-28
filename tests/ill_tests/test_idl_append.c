#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_append(void) {
	t_test_dnode n1, n2, n3, n4;

	INIT_STD_DNODES(n1, n2, n3, n4);

	idl_append(&n1.dnode, &n3.dnode);
	assert(n1.value == 42 && n3.value == 44);
	assert(n1.id == 1 && n3.id == 3);
	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n3.dnode);
	assert(n3.dnode.next == NULL);

	idl_append(&n1.dnode, &n2.dnode);
	assert(n1.value == 42 && n2.value == 43 && n3.value == 44);
	assert(n1.id == 1 && n2.id == 2 && n3.id == 3);
	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	assert(n3.dnode.next == NULL);

	idl_append(&n1.dnode, NULL);
	idl_append(NULL, &n4.dnode);

	idl_append(&n3.dnode, &n4.dnode);
	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);
}
