#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_validate(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;
	size_t         off  = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	assert(ill_validate(NULL) == true);

	ill_push_back_node(&head, &n1.node);
	assert(ill_validate(head) == true);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);
	assert(ill_validate(head) == true);

	// mixed offsets are valid by design
	n2.node.offset = 0;
	assert(ill_validate(head) == true);
	n2.node.offset = off;

	// a cycle is rejected
	n3.node.next = &n1.node;
	assert(ill_validate(head) == false);
	n3.node.next = NULL;
	assert(ill_validate(head) == true);
}
