#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

NO_COV static void build_list(t_cm_idl **head, t_cm_test_dnode *nodes,
                              size_t n) {
	size_t i;

	*head = NULL;
	for (i = 0; i < n; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back_node(head, &nodes[i].dnode);
	}
}

NO_COV void test_cm_idl_fold(void) {
	t_cm_test_dnode n[5];
	t_cm_idl       *head = NULL;
	int             sum  = 0;
	int            *res;

	build_list(&head, n, 5);

	assert(idl_fold(NULL, cb_idl_sum, &sum) == &sum);
	assert(sum == 0);
	assert(idl_fold(head, cb_idl_sum, &sum) == &sum);
	assert(sum == 15);

	sum = 0;
	assert(idl_nfold(head, 0, cb_idl_sum, &sum) == &sum);
	assert(sum == 0);
	assert(idl_nfold(head, 1, cb_idl_sum, &sum) == &sum);
	assert(sum == 1);
	sum = 0;
	assert(idl_nfold(head, 3, cb_idl_sum, &sum) == &sum);
	assert(sum == 6);
	sum = 42;
	assert(idl_nfold(head, (size_t)-1, cb_idl_sum, &sum) == &sum);
	assert(sum == 57);

	assert(idl_fold(head, NULL, &sum) == NULL);
	assert(idl_fold(head, cb_idl_sum, NULL) == NULL);
	assert(idl_nfold(head, (size_t)-1, NULL, &sum) == NULL);

	alloc_set_call_limit(0);
	assert(idl_reduce(head, cb_idl_sum, sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	res = (int *)idl_reduce(head, cb_idl_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 15);
	free(res);
	res = (int *)idl_reduce(NULL, cb_idl_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 0);
	free(res);
	assert(idl_reduce(head, NULL, sizeof(int)) == NULL);
	assert(idl_reduce(head, cb_idl_sum, 0) == NULL);

	res = (int *)idl_nreduce(head, 3, cb_idl_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 6);
	free(res);
	res = (int *)idl_nreduce(head, 0, cb_idl_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 0);
	free(res);
	res = (int *)idl_nreduce(head, (size_t)-1, cb_idl_sum, sizeof(int));
	assert(res != NULL);
	assert(*res == 15);
	free(res);
}
