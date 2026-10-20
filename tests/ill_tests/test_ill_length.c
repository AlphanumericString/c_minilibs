#include <assert.h>

#include "ill_tests.h"

static bool is_even(void *elem, void *arg) {
	t_test_node *n = (t_test_node *)elem;
	(void)arg;
	return n->value % 2 == 0;
}

NO_COV void test_ill_length(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));
	ill_init(&n4.node, offsetof(t_test_node, node));

	n1.value = 1;
	n2.value = 2;
	n3.value = 3;
	n4.value = 4;

	assert(ill_length(head) == 0);
	assert(ill_length(NULL) == 0);
	assert(ill_nlength(head, 10) == 0);
	assert(ill_nlength(NULL, 10) == 0);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	assert(ill_length(head) == 4);
	assert(ill_nlength(head, 2) == 2);
	assert(ill_nlength(head, 10) == 4);

	assert(ill_match_nb(head, is_even, NULL) == 2);
	assert(ill_match_nb(head, NULL, NULL) == 0);
	assert(ill_match_nb(NULL, is_even, NULL) == 0);
}
