#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_remove_after(void) {
	t_test_dnode n2, n3;
	t_idl       *head = NULL;
	t_idl       *rm_after_res;

	INIT_DNODE(n2, 42, 2);
	INIT_DNODE(n3, 43, 3);

	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	rm_after_res = idl_remove_after(&n2.dnode);
	assert(rm_after_res == &n3.dnode);
	assert(n2.dnode.next == NULL);
	assert(head == &n2.dnode);
	assert(idl_remove_after(idl_get_tail(&n2.dnode)) == NULL);
	assert(idl_remove_after(NULL) == NULL);
}
