#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_iter_range(void) {
	t_cm_test_node n1, n2, n3, n4, iso;
	t_cm_ill      *head = NULL;
	void          *it   = NULL;
	size_t         off  = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);
	INIT_NODE(n4, 40, 4);
	INIT_NODE(iso, 50, 5);
	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);
	ill_push_back_node(&head, &n4.node);

	assert(ill_iter_range(NULL, NULL, &it, off) == false);
	assert(it == NULL);
	assert(ill_iter_range(&n1, NULL, NULL, off) == false);

	// empty range
	it = NULL;
	assert(ill_iter_range(&n1, &n1, &it, off) == false);
	assert(it == NULL);

	// full range
	it = NULL;
	assert(ill_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n1);
	assert(ill_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n2);
	assert(ill_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n3);
	assert(ill_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n4);
	assert(ill_iter_range(&n1, NULL, &it, off) == false);
	assert(it == NULL);

	// exhausted iterators restart from the head
	assert(ill_iter_range(&n1, NULL, &it, off) == true);
	assert(it == &n1);

	// exclusive end
	it = NULL;
	assert(ill_iter_range(&n1, &n3, &it, off) == true);
	assert(it == &n1);
	assert(ill_iter_range(&n1, &n3, &it, off) == true);
	assert(it == &n2);
	assert(ill_iter_range(&n1, &n3, &it, off) == false);
	assert(it == NULL);

	// arbitrary start
	it = &n2;
	assert(ill_iter_range(&n1, &n4, &it, off) == true);
	assert(it == &n3);
	assert(ill_iter_range(&n1, &n4, &it, off) == false);

	// end reached on the very next step
	it = &n2;
	assert(ill_iter_range(&n1, &n3, &it, off) == false);
	assert(it == NULL);

	// an end outside the list behaves like the tail
	it = NULL;
	assert(ill_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n1);
	assert(ill_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n2);
	assert(ill_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n3);
	assert(ill_iter_range(&n1, &iso, &it, off) == true);
	assert(it == &n4);
	assert(ill_iter_range(&n1, &iso, &it, off) == false);
	assert(it == NULL);
}
