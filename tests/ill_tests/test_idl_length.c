#include <assert.h>

#include "ill_tests.h"

static bool is_even(void *elem, void *arg) {
	t_test_dnode *n = (t_test_dnode *)elem;
	(void)arg;
	return n->value % 2 == 0;
}

void test_idl_length(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;
	size_t       matches;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 1;
	n2.value = 2;
	n3.value = 3;
	n4.value = 4;

	assert(idl_length(head) == 0);
	assert(idl_nlength(head, 10) == 0);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	assert(idl_length(head) == 4);
	assert(idl_nlength(head, 2) == 2);
	assert(idl_nlength(head, 10) == 4);

	matches = idl_match_nb(head, is_even, NULL);
	assert(matches == 2);
	matches = idl_match_nb(head, NULL, NULL);
	assert(matches == 0);
	matches = idl_match_nb(NULL, is_even, NULL);
	assert(matches == 0);
}
