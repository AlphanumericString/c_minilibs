#include <assert.h>

#include "ill_tests.h"

NO_COV static void check_sorted(t_ill *head) {
	while (head && head->next) {
		assert(cb_ill_cmp_min_max(ill_ntoe(head), ill_ntoe(head->next)) <= 0);
		head = head->next;
	}
}

NO_COV static void check_values(t_ill *head, const int *exp, size_t n) {
	size_t i = 0;

	while (head && i < n) {
		assert(((t_test_node *)ill_ntoe(head))->value == exp[i]);
		head = head->next;
		i++;
	}
	assert(i == n);
	assert(head == NULL);
}

NO_COV void test_ill_sort(void) {
	t_test_node nodes[5];
	t_test_node a[3];
	t_test_node b[3];
	t_test_node solo;
	t_ill      *head;
	t_ill      *la        = NULL;
	t_ill      *lb        = NULL;
	int         values[5] = {5, 1, 4, 2, 3};
	int         exp[5]    = {1, 2, 3, 4, 5};
	int         expm[6]   = {1, 2, 3, 4, 5, 6};
	size_t      i;

	head = NULL;
	for (i = 0; i < 5; i++) {
		INIT_NODE(nodes[i], values[i], (int)i);
		ill_push_back(&head, &nodes[i].node);
	}
	ill_sort(&head, cb_ill_cmp_min_max);
	assert(ill_length(head) == 5);
	check_sorted(head);
	check_values(head, exp, 5);

	// already sorted / single element
	ill_sort(&head, cb_ill_cmp_min_max);
	check_values(head, exp, 5);
	INIT_NODE(solo, 7, 9);
	{
		t_ill *sh = &solo.node;
		ill_sort(&sh, cb_ill_cmp_min_max);
		assert(sh == &solo.node);
	}
	assert(solo.node.next == NULL);

	// bad args
	ill_sort(NULL, cb_ill_cmp_min_max);
	ill_sort(&head, NULL);
	head = NULL;
	ill_sort(&head, cb_idl_cmp_min_max);
	ill_sort(NULL, NULL);

	// sorted merge
	for (i = 0; i < 3; i++) {
		INIT_NODE(a[i], (int)(i * 2 + 1), (int)i);       // 1 3 5
		INIT_NODE(b[i], (int)(i * 2 + 2), (int)(i + 3)); // 2 4 6
		ill_push_back(&la, &a[i].node);
		ill_push_back(&lb, &b[i].node);
	}
	head = ill_sorted_merge(la, lb, cb_ill_cmp_min_max);
	check_values(head, expm, 6);
	check_sorted(head);

	assert(ill_sorted_merge(NULL, NULL, cb_ill_cmp_min_max) == NULL);
	assert(ill_sorted_merge(la, lb, NULL) == NULL);
}
