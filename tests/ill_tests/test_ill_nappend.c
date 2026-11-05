#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_nappend(void) {
	t_cm_test_node n1, n2, n3, n4;
	t_cm_test_node n5, n6;
	t_cm_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	INIT_NODE(n5, 46, 5);
	INIT_NODE(n6, 47, 6);
	LINK_UP_NODES(&n5.node, &n6.node);

	ill_nappend(&n4.node, &n5.node);
	ASSERT_ORDER_ILL(&n4.node, &n5.node);
	ASSERT_ORDER_ILL(&n5.node, &n6.node);
	assert(n6.node.next == NULL);
	ill_nappend(&n4.node, NULL);
	ill_nappend(NULL, &n5.node);
}
