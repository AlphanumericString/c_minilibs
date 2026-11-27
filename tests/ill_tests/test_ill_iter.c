#include <assert.h>

#include "ill_tests.h"

typedef struct s_cm_test_node_pad {
	char     pad[16];
	t_cm_ill node;
} t_cm_test_node_pad;

NO_COV static void iter_mixed_offsets(void) {
	t_cm_test_node     n1, n3;
	t_cm_test_node_pad n2;
	size_t             off = offsetof(t_cm_test_node, node);
	void              *it  = NULL;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n3, 30, 3);
	ill_init_node(&n2.node, offsetof(t_cm_test_node_pad, node));
	n1.node.next = &n2.node;
	n2.node.next = &n3.node;
	n3.node.next = NULL;

	assert(ill_iter(&n1.node, &it, off) == true);
	assert(it == &n1);
	assert(ill_iter(&n1.node, &it, off) == true);
	assert(it == &n2);
	off = offsetof(t_cm_test_node_pad, node);
	assert(ill_iter(&n1.node, &it, off) == true);
	assert(it == &n3);
	off = offsetof(t_cm_test_node, node);
	assert(ill_iter(&n1.node, &it, off) == false);
	assert(it == NULL);
}

NO_COV void test_cm_ill_iter(void) {
	t_cm_test_node  n1, n2, n3;
	t_cm_ill       *head  = NULL;
	t_cm_test_node *typed = NULL;
	void           *it    = NULL;
	size_t          off   = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	assert(ill_iter(head, &it, off) == false);
	assert(it == NULL);
	assert(ill_iter(NULL, &it, off) == false);
	assert(ill_iter(NULL, NULL, off) == false);

	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);

	assert(ill_iter(head, NULL, off) == false);

	it = NULL;
	assert(ill_iter(head, &it, off) == true);
	assert(it == &n1);
	assert(ill_iter(head, &it, off) == true);
	assert(it == &n2);
	assert(ill_iter(head, &it, off) == true);
	assert(it == &n3);
	assert(ill_iter(head, &it, off) == false);
	assert(it == NULL);

	// exhausted handles restart from the head on the next call
	assert(ill_iter(head, &it, off) == true);
	assert(it == &n1);

	// typed iterator usage
	assert(ill_iter(head, (void **)&typed, off) == true);
	assert(typed == &n1);
	assert(ill_iter(head, (void **)&typed, off) == true);
	assert(typed == &n2);

	// start from an arbitrary element
	it = &n2;
	assert(ill_iter(head, &it, off) == true);
	assert(it == &n3);
	assert(ill_iter(head, &it, off) == false);
	assert(it == NULL);

	iter_mixed_offsets();
}
