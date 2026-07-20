#include <assert.h>

#include "ill_tests.h"

void test_ill_find(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);

	t_ill *found = ill_find(head, &n2.node);
	assert(found == &n2.node);

	t_ill *not_found = ill_find(head, &n3.node);
	assert(not_found == NULL);

	t_ill *prev = ill_find_prev(head, &n2.node);
	assert(prev == &n1.node);

	t_ill *prev_head = ill_find_prev(head, &n1.node);
	assert(prev_head == NULL);

	t_ill *prev_not_found = ill_find_prev(head, &n3.node);
	assert(prev_not_found == NULL);
}
