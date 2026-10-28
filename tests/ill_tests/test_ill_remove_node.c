#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_remove_node(void) {
	t_test_node n2, n3;
	t_ill      *head = NULL;

	INIT_NODE(n2, 42, 2);
	INIT_NODE(n3, 43, 3);

	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_remove_node(NULL, &n3.node) == false);
	assert(ill_remove_node(&head, NULL) == false);
	assert(ill_remove_node(&head, (void *)0xDEAD) == false);
	assert(ill_remove_node(&head, &n3.node) == true);
	assert(head == &n2.node);
	assert(n2.node.next == NULL);

	assert(ill_remove_node(&head, &n2.node) == true);
	assert(head == NULL);
	assert(ill_remove_node(&head, (void *)0xDEAD) == false);
}
