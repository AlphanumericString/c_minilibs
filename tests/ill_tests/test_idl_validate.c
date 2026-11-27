#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_validate(void) {
	t_cm_test_dnode n1, n2, n3, iso;
	t_cm_idl       *head = NULL;
	size_t          off  = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	INIT_DNODE(iso, 40, 4);

	assert(idl_validate(NULL) == true);

	idl_push_back_node(&head, &n1.dnode);
	assert(idl_validate(head) == true);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);
	assert(idl_validate(head) == true);

	// mixed offsets are valid by design
	n2.dnode.offset = 0;
	assert(idl_validate(head) == true);
	n2.dnode.offset = off;

	// the head must not have a predecessor
	n1.dnode.prev = &iso.dnode;
	assert(idl_validate(head) == false);
	n1.dnode.prev = NULL;

	// a broken next->prev link is rejected
	n2.dnode.prev = NULL;
	assert(idl_validate(head) == false);
	n2.dnode.prev = &n1.dnode;
	assert(idl_validate(head) == true);

	// a cycle is rejected
	n3.dnode.next = &n1.dnode;
	n1.dnode.prev = &n3.dnode;
	assert(idl_validate(head) == false);
	n3.dnode.next = NULL;
	n1.dnode.prev = NULL;
	assert(idl_validate(head) == true);
}
