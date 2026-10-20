#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_find(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);

	assert(ill_find(head, &n2.node) == &n2.node);
	assert(ill_find(head, &n3.node) == NULL);
	assert(ill_find(head, NULL) == NULL);
	assert(ill_find(NULL, &n2.node) == NULL);

	assert(ill_find_prev(head, &n2.node) == &n1.node);
	assert(ill_find_prev(head, head) == NULL);
	assert(ill_find_prev(head, &n3.node) == NULL);
	assert(ill_find_prev(head, NULL) == NULL);
	assert(ill_find_prev(NULL, &n2.node) == NULL);
}
