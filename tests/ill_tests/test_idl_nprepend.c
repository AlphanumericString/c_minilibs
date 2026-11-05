#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_nprepend(void) {
	t_cm_test_dnode n0, n1, n2, n3, n4;
	t_cm_test_dnode n7, n8;
	t_cm_test_dnode n_neg1, n_neg2;
	t_cm_idl       *head = NULL;
	t_cm_idl       *n0_before_op_prv;

	INIT_DNODE(n0, 0, 0);
	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	INIT_DNODE(n7, 46, 7);
	INIT_DNODE(n8, 47, 8);
	LINK_UP_DNODES(&n7.dnode, &n8.dnode);
	idl_npush(&head, &n7.dnode);

	idl_prepend(&n1.dnode, &n0.dnode);

	n0_before_op_prv = n0.dnode.prev;
	INIT_DNODE(n_neg1, 48, -1);
	INIT_DNODE(n_neg2, 49, -2);
	idl_prepend(&n_neg1.dnode, &n_neg2.dnode);

	// State assert: [n-2 , n-1] [.. n0_before_op, n0, n1 ..]
	assert(n_neg2.dnode.next == &n_neg1.dnode && n_neg1.dnode.next == NULL);
	assert(n_neg2.dnode.prev == NULL && n_neg1.dnode.prev == &n_neg2.dnode);
	assert(n0.dnode.prev == n0_before_op_prv && n0.dnode.next == &n1.dnode);
	idl_nprepend(&n0.dnode, &n_neg1.dnode);
	// State assert: [ .. n0_before, n-2, n-1, n0, n1 ..]
	assert(n0_before_op_prv->next == &n_neg2.dnode &&
	       n_neg2.dnode.next == &n_neg1.dnode &&
	       n_neg1.dnode.next == &n0.dnode && n0.dnode.next == &n1.dnode);
	assert(n_neg2.dnode.prev == n0_before_op_prv &&
	       n_neg1.dnode.prev == &n_neg2.dnode &&
	       n0.dnode.prev == &n_neg1.dnode && n1.dnode.prev == &n0.dnode);
	n0.dnode.prev = NULL;
	n_neg1.dnode =
	    (t_cm_idl){.prev = NULL, .next = NULL, .offset = n_neg1.dnode.offset};
	// State assert: [NULL, n0, n1 ..] [n-1]
	assert(n0.dnode.prev == NULL && n0.dnode.next == &n1.dnode);
	assert(n_neg1.dnode.prev == NULL && n_neg1.dnode.next == NULL);
	idl_nprepend(&n0.dnode, &n_neg1.dnode);
	// State assert: [NULL, n-1, n0, n1 ..]
	assert(n0.dnode.prev == &n_neg1.dnode && n0.dnode.next == &n1.dnode);
	assert(n_neg1.dnode.prev == NULL && n_neg1.dnode.next == &n0.dnode);

	idl_nprepend(NULL, &n_neg2.dnode);
	idl_nprepend(&n_neg1.dnode, NULL);
}
