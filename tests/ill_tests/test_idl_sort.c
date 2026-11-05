#include <assert.h>

#include "ill_tests.h"

NO_COV static void check_sorted(t_cm_idl *head) {
	t_cm_idl *prev = NULL;

	while (head) {
		assert(head->prev == prev);
		if (head->next) assert(head->next->prev == head);
		if (head->next)
			assert(cb_idl_cmp_min_max(idl_ntoe(head), idl_ntoe(head->next)) <=
			       0);
		prev = head;
		head = head->next;
	}
}

NO_COV static void check_values(t_cm_idl *head, const int *exp, size_t n) {
	size_t i = 0;

	while (head && i < n) {
		assert(((t_cm_test_dnode *)idl_ntoe(head))->value == exp[i]);
		head = head->next;
		i++;
	}
	assert(i == n);
	assert(head == NULL);
}

NO_COV void test_cm_idl_sort(void) {
	t_cm_test_dnode nodes[5];
	t_cm_test_dnode a[3];
	t_cm_test_dnode b[3];
	t_cm_test_dnode solo;
	t_cm_idl       *head      = NULL;
	t_cm_idl       *la        = NULL;
	t_cm_idl       *lb        = NULL;
	int             values[5] = {5, 1, 4, 2, 3};
	int             exp[5]    = {1, 2, 3, 4, 5};
	int             expm[6]   = {1, 2, 3, 4, 5, 6};
	size_t          i;

	head = NULL;
	for (i = 0; i < 5; i++) {
		INIT_DNODE(nodes[i], values[i], (int)i);
		idl_push_back(&head, &nodes[i].dnode);
	}
	idl_sort(&head, cb_idl_cmp_min_max);
	assert(idl_length(head) == 5);
	check_sorted(head);
	check_values(head, exp, 5);

	// already sorted / single element
	idl_sort(&head, cb_idl_cmp_min_max);
	check_values(head, exp, 5);
	INIT_DNODE(solo, 7, 9);
	{
		t_cm_idl *sh = &solo.dnode;
		idl_sort(&sh, cb_idl_cmp_min_max);
		assert(sh == &solo.dnode);
	}
	assert(solo.dnode.next == NULL);
	assert(solo.dnode.prev == NULL);

	// bad args
	idl_sort(NULL, cb_idl_cmp_min_max);
	idl_sort(&head, NULL);
	head = NULL;
	idl_sort(&head, cb_idl_cmp_min_max);
	idl_sort(NULL, NULL);

	// sorted merge
	for (i = 0; i < 3; i++) {
		INIT_DNODE(a[i], (int)(i * 2 + 1), (int)i);       // 1 3 5
		INIT_DNODE(b[i], (int)(i * 2 + 2), (int)(i + 3)); // 2 4 6
		idl_push_back(&la, &a[i].dnode);
		idl_push_back(&lb, &b[i].dnode);
	}
	head = idl_sorted_merge(la, lb, cb_idl_cmp_min_max);
	check_values(head, expm, 6);
	check_sorted(head);

	assert(idl_sorted_merge(NULL, NULL, cb_idl_cmp_min_max) == NULL);
	assert(idl_sorted_merge(la, lb, NULL) == NULL);
}
