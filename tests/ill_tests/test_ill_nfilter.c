#include <assert.h>

#include "ill_tests.h"

NO_COV static size_t collect(t_cm_ill *head, int *out, size_t cap) {
	size_t n = 0;

	while (head && n < cap) {
		out[n++] = ((t_cm_test_node *)ill_ntoe(head))->value;
		head     = head->next;
	}
	return n;
}

NO_COV static void build_list(t_cm_ill **head, t_cm_test_node *nodes, size_t n,
                              const int *values) {
	size_t i;

	*head = NULL;
	for (i = 0; i < n; i++) {
		INIT_NODE(nodes[i], values[i], (int)i);
		ill_push_back_node(head, &nodes[i].node);
	}
}

NO_COV void test_cm_ill_nfilter(void) {
	static const int vals[]  = {1, 2, 3, 4, 5, 6};
	static const int odds[]  = {1, 3, 5};
	static const int evens[] = {2, 4, 5};
	t_cm_test_node   nodes[6];
	t_cm_ill        *head, *filtered;
	int              out[6];

	// n == 0 resets the output and leaves the list untouched
	build_list(&head, nodes, 6, vals);
	filtered = &nodes[0].node;
	assert(ill_nfilter(&head, 0, cb_ill_is_even, NULL, &filtered) == 0);
	assert(filtered == NULL);
	assert(collect(head, out, 6) == 6);
	assert(out[0] == 1 && out[5] == 6);

	// keep matching elements among the first n, leave the suffix untouched
	build_list(&head, nodes, 6, vals);
	assert(ill_nfilter(&head, 3, cb_ill_is_even, NULL, &filtered) == 2);
	assert(collect(head, out, 6) == 4);
	assert(out[0] == 2 && out[1] == 4 && out[2] == 5 && out[3] == 6);
	assert(collect(filtered, out, 6) == 2);
	assert(out[0] == 1 && out[1] == 3);

	// inverse variant
	build_list(&head, nodes, 6, vals);
	assert(ill_nfilter_out(&head, 3, cb_ill_is_even, NULL, &filtered) == 1);
	assert(collect(head, out, 6) == 5);
	assert(out[0] == 1 && out[1] == 3 && out[2] == 4 && out[3] == 5 &&
	       out[4] == 6);
	assert(collect(filtered, out, 6) == 1);
	assert(out[0] == 2);

	// n larger than the list: every element is inspected
	build_list(&head, nodes, 3, odds);
	assert(ill_nfilter(&head, 10, cb_ill_is_even, NULL, &filtered) == 3);
	assert(head == NULL);
	assert(collect(filtered, out, 6) == 3);
	assert(out[0] == 1 && out[1] == 3 && out[2] == 5);

	// moving the head out updates the list head
	build_list(&head, nodes, 3, vals);
	assert(ill_nfilter(&head, 1, cb_ill_is_even, NULL, &filtered) == 1);
	assert(collect(head, out, 6) == 2);
	assert(out[0] == 2 && out[1] == 3);
	assert(collect(filtered, out, 6) == 1);
	assert(out[0] == 1);

	build_list(&head, nodes, 3, evens);
	assert(ill_nfilter_out(&head, 1, cb_ill_is_even, NULL, &filtered) == 1);
	assert(collect(head, out, 6) == 2);
	assert(out[0] == 4 && out[1] == 5);
	assert(collect(filtered, out, 6) == 1);
	assert(out[0] == 2);

	// invalid arguments leave the output untouched
	build_list(&head, nodes, 3, vals);
	filtered = &nodes[0].node;
	assert(ill_nfilter(NULL, 3, cb_ill_is_even, NULL, &filtered) == 0);
	assert(filtered == &nodes[0].node);
	assert(ill_nfilter(&head, 3, NULL, NULL, &filtered) == 0);
	assert(filtered == &nodes[0].node);
	assert(ill_nfilter(&head, 3, cb_ill_is_even, NULL, NULL) == 0);
	assert(ill_nfilter_out(NULL, 3, cb_ill_is_even, NULL, &filtered) == 0);
	assert(filtered == &nodes[0].node);
	assert(ill_nfilter_out(&head, 3, NULL, NULL, &filtered) == 0);
	assert(filtered == &nodes[0].node);
	assert(ill_nfilter_out(&head, 3, cb_ill_is_even, NULL, NULL) == 0);
}
