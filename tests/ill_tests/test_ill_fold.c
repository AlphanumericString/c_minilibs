#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

NO_COV static void build_list(t_cm_ill **head, t_cm_test_node *nodes,
                              size_t n) {
	size_t i;

	*head = NULL;
	for (i = 0; i < n; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back_node(head, &nodes[i].node);
	}
}

NO_COV void test_cm_ill_fold(void) {
	t_cm_test_node n[5];
	t_cm_ill      *head = NULL;
	int            sum  = 0;
	int           *res;

	build_list(&head, n, 5);

	assert(ill_fold(NULL, cb_ill_sum, &sum) == &sum);
	assert(sum == 0);
	assert(ill_fold(head, cb_ill_sum, &sum) == &sum);
	assert(sum == 15);

	sum = 0;
	assert(ill_nfold(head, 0, cb_ill_sum, &sum) == &sum);
	assert(sum == 0);
	assert(ill_nfold(head, 1, cb_ill_sum, &sum) == &sum);
	assert(sum == 1);
	sum = 0;
	assert(ill_nfold(head, 3, cb_ill_sum, &sum) == &sum);
	assert(sum == 6);
	sum = 42;
	assert(ill_nfold(head, (size_t)-1, cb_ill_sum, &sum) == &sum);
	assert(sum == 57);

	assert(ill_fold(head, NULL, &sum) == NULL);
	assert(ill_fold(head, cb_ill_sum, NULL) == NULL);
	assert(ill_nfold(head, (size_t)-1, NULL, &sum) == NULL);

	alloc_set_call_limit(0);
	assert(ill_reduce(head, cb_ill_sum, sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	res = (int *)ill_reduce(head, cb_ill_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 15);
	free(res);
	res = (int *)ill_reduce(NULL, cb_ill_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 0);
	free(res);
	assert(ill_reduce(head, NULL, sizeof(int)) == NULL);
	assert(ill_reduce(head, cb_ill_sum, 0) == NULL);

	res = (int *)ill_nreduce(head, 3, cb_ill_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 6);
	free(res);
	res = (int *)ill_nreduce(head, 0, cb_ill_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 0);
	free(res);
	res = (int *)ill_nreduce(head, (size_t)-1, cb_ill_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 15);
	free(res);
}
