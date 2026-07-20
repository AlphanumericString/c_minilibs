#include <assert.h>

#include "ill_tests.h"

void test_ill_gettail(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	assert(ill_get_tail(head) == NULL);

	ill_push_back(&head, &n1.node);
	assert(ill_get_tail(head) == &n1.node);

	ill_push_back(&head, &n2.node);
	assert(ill_get_tail(head) == &n2.node);

	ill_push_back(&head, &n3.node);
	assert(ill_get_tail(head) == &n3.node);
}
