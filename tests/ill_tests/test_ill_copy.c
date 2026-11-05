#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

static int loc_count = 0;

NO_COV static void *foo(const void *e) {
	static t_cm_test_dnode no_leak = {0};

	(void)e;
	if (loc_count++ < 1) return (&no_leak);
	return (NULL);
}

NO_COV static void copy_cleanup_on_failure(void) {
	t_cm_test_node n1, n2, n3;
	void          *head;

	INIT_NODE(n1, 42, 1);
	INIT_NODE(n2, 84, 2);
	INIT_NODE(n3, 126, 3);
	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);

	g_dup_hits    = 0;
	g_dup_fail_at = 2;
	head          = ill_copy(&n1.node, cb_ill_duplicate_fail_at, cb_free_elem);
	assert(head == NULL);
	g_dup_fail_at = -1;
	g_dup_hits    = 0;

	head = ill_copy(&n1.node, foo, NULL);
	assert(head == NULL);
}

NO_COV void test_cm_ill_copy(void) {
	t_cm_test_node  elem_1;
	t_cm_test_node  elem_2;
	t_cm_test_node *copy_1;
	t_cm_test_node *copy_2;

	INIT_NODE(elem_1, 42, 1);
	INIT_NODE(elem_2, 84, 2);
	LINK_UP_NODES(&elem_1.node, &elem_2.node);

	copy_1 = ill_copy(&elem_1.node, cb_ill_duplicate, cb_free_elem);

	assert(copy_1 != NULL);
	assert(copy_1->id == elem_1.id);
	assert(copy_1->value == elem_1.value);
	assert(copy_1->node.next != NULL);

	copy_2 = (t_cm_test_node *)ill_ntoe(copy_1->node.next);
	assert(copy_2->id == elem_2.id);
	assert(copy_2->value == elem_2.value);
	assert(copy_2->node.next == NULL);

	free(copy_2);
	free(copy_1);

	assert(ill_copy(&elem_1.node, NULL, cb_free_elem) == NULL);
	assert(ill_copy(NULL, cb_ill_duplicate, cb_free_elem) == NULL);
	assert(ill_copy(&elem_1.node, cb_bad_duplicator, cb_free_elem) == NULL);

	copy_1 = ill_copy(&elem_1.node, cb_ill_duplicate, NULL);
	assert(copy_1 != NULL);
	free(ill_ntoe(copy_1->node.next));
	free(copy_1);

	copy_cleanup_on_failure();
}
