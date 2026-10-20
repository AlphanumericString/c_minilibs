#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_insert(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 1;
	n2.value = 2;
	n3.value = 3;
	n4.value = 4;

	assert(idl_push(&head, &n2.dnode) != NULL);
	assert(head == &n2.dnode);
	assert(idl_push(NULL, &n2.dnode) == NULL);
	assert(idl_push(&head, NULL) == NULL);

	idl_push(&head, &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n2.dnode.prev == &n1.dnode);

	assert(idl_push_back(&head, &n4.dnode) != NULL);
	// State assert: [n1, n2, n4]
	assert(n2.dnode.next == &n4.dnode);
	assert(n4.dnode.prev == &n2.dnode);
	assert(n4.dnode.next == NULL);

	assert(idl_push_back(NULL, &n2.dnode) == NULL);
	assert(idl_push_back(&head, NULL) == NULL);

	idl_append(&n2.dnode, &n3.dnode);
	// State assert: [n1, n2, n3, n4]
	assert(n2.dnode.next == &n3.dnode);
	assert(n3.dnode.prev == &n2.dnode);
	assert(n3.dnode.next == &n4.dnode);
	assert(n4.dnode.prev == &n3.dnode);

	idl_append(NULL, &n3.dnode);
	idl_append(&n2.dnode, NULL);

	t_test_dnode n5, n6;
	idl_init(&n5.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n6.dnode, offsetof(t_test_dnode, dnode));
	idl_append(&n5.dnode, &n6.dnode);
	// State assert: [n1, n2, n3, n4] [n5, n6]
	idl_nappend(&n4.dnode, &n5.dnode);
	// State assert: [n1, n2, n3, n4, n5, n6]
	assert(n4.dnode.next == &n5.dnode && n5.dnode.prev == &n4.dnode);
	assert(n5.dnode.next == &n6.dnode && n6.dnode.prev == &n5.dnode);
	assert(n6.dnode.next == NULL);
	idl_nappend(&n4.dnode, NULL);
	idl_nappend(NULL, &n5.dnode);
	t_test_dnode n_tmp1;
	idl_init(&n_tmp1.dnode, offsetof(t_test_dnode, dnode));
	idl_nappend(&n1.dnode, &n_tmp1.dnode);
	assert(n1.dnode.next == &n_tmp1.dnode && n1.dnode.prev == NULL);
	assert(n_tmp1.dnode.next == &n2.dnode && n_tmp1.dnode.prev == &n1.dnode);
	n1.dnode.next = &n2.dnode;
	n2.dnode.prev = &n1.dnode;

	t_test_dnode n7, n8;
	idl_init(&n7.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n8.dnode, offsetof(t_test_dnode, dnode));
	n7.dnode.next = &n8.dnode;
	n8.dnode.prev = &n7.dnode;

	// State assert: [.. n5, n6] [n7, n8]
	assert(idl_npush(&head, &n7.dnode) != NULL);
	// State assert: [.. n5, n6, n7, n8]
	assert(head == &n7.dnode);
	assert(n8.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n8.dnode);
	assert(idl_npush(&head, NULL) == NULL);
	assert(idl_npush(NULL, &n7.dnode) == NULL);

	t_test_dnode n0;
	idl_init(&n0.dnode, offsetof(t_test_dnode, dnode));
	// State assert: [n0] [ n1, n2, ..]
	idl_prepend(&n1.dnode, &n0.dnode);
	assert(n8.dnode.next == &n0.dnode);
	assert(n0.dnode.prev == &n8.dnode);
	assert(n0.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n0.dnode);
	idl_prepend(&n1.dnode, NULL);
	idl_prepend(NULL, &n0.dnode);

	t_test_dnode n_neg1, n_neg2;
	t_idl       *n0_before_op_prv;
	n0_before_op_prv = n0.dnode.prev;
	idl_init(&n_neg1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n_neg2.dnode, offsetof(t_test_dnode, dnode));
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
	    (t_idl){.prev = NULL, .next = NULL, .offset = n_neg1.dnode.offset};
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
