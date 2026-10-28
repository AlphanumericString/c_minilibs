#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

NO_COV void test_ill_copy(void) {
	t_test_node  elem_1;
	t_test_node  elem_2;
	t_test_node *copy_1;
	t_test_node *copy_2;

	INIT_NODE(elem_1, 42, 1);
	INIT_NODE(elem_2, 84, 2);
	LINK_UP_NODES(&elem_1.node, &elem_2.node);

	copy_1 = ill_copy(&elem_1.node, cb_ill_duplicate);

	assert(copy_1 != NULL);
	assert(copy_1->id == elem_1.id);
	assert(copy_1->value == elem_1.value);
	assert(copy_1->node.next != NULL);

	copy_2 = (t_test_node *)ill_ntoe(copy_1->node.next);
	assert(copy_2->id == elem_2.id);
	assert(copy_2->value == elem_2.value);
	assert(copy_2->node.next == NULL);

	free(copy_2);
	free(copy_1);

	assert(ill_copy(&elem_1.node, NULL) == NULL);
	assert(ill_copy(NULL, cb_ill_duplicate) == NULL);
	assert(ill_copy(&elem_1.node, cb_bad_duplicator) == NULL);
}
