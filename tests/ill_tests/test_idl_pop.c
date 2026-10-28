#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_pop(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;
	t_idl       *pop_res;
	t_idl       *null_head = NULL;

	INIT_STD_DNODES(n1, n2, n3, n4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	pop_res = idl_pop(&head);
	assert(pop_res == &n1.dnode);
	assert(head == &n2.dnode);

	assert(n2.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	pop_res = idl_pop(&head);
	assert(pop_res == &n2.dnode);
	assert(head == &n3.dnode);

	assert(n3.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	pop_res = idl_pop(&head);
	assert(pop_res == &n3.dnode);
	assert(head == &n4.dnode);
	assert(n4.dnode.prev == NULL);
	assert(n4.dnode.next == NULL);

	pop_res = idl_pop(&head);
	assert(pop_res == &n4.dnode);
	assert(head == NULL);

	assert(idl_pop(&null_head) == NULL);
	assert(idl_pop(NULL) == NULL);
}
