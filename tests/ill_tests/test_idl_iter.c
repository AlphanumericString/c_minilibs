#include <assert.h>

#include "ill_tests.h"

typedef struct s_cm_test_dnode_pad {
	char     pad[16];
	t_cm_idl dnode;
} t_cm_test_dnode_pad;

NO_COV static void iter_mixed_offsets(void) {
	t_cm_test_dnode     n1, n3;
	t_cm_test_dnode_pad n2;
	size_t              off = offsetof(t_cm_test_dnode, dnode);
	void               *it  = NULL;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n3, 30, 3);
	idl_init_node(&n2.dnode, offsetof(t_cm_test_dnode_pad, dnode));
	n1.dnode.next = &n2.dnode;
	n2.dnode.prev = &n1.dnode;
	n2.dnode.next = &n3.dnode;
	n3.dnode.prev = &n2.dnode;

	assert(idl_iter(&n1.dnode, &it, off) == true);
	assert(it == &n1);
	assert(idl_iter(&n1.dnode, &it, off) == true);
	assert(it == &n2);
	off = offsetof(t_cm_test_dnode_pad, dnode);
	assert(idl_iter(&n1.dnode, &it, off) == true);
	assert(it == &n3);
	off = offsetof(t_cm_test_dnode, dnode);
	assert(idl_iter(&n1.dnode, &it, off) == false);
	assert(it == NULL);
}

NO_COV void test_cm_idl_iter(void) {
	t_cm_test_dnode  n1, n2, n3;
	t_cm_idl        *head  = NULL;
	t_cm_test_dnode *typed = NULL;
	void            *it    = NULL;
	size_t           off   = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	assert(idl_iter(head, &it, off) == false);
	assert(it == NULL);
	assert(idl_iter(NULL, &it, off) == false);
	assert(idl_iter(NULL, NULL, off) == false);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	assert(idl_iter(head, NULL, off) == false);

	it = NULL;
	assert(idl_iter(head, &it, off) == true);
	assert(it == &n1);
	assert(idl_iter(head, &it, off) == true);
	assert(it == &n2);
	assert(idl_iter(head, &it, off) == true);
	assert(it == &n3);
	assert(idl_iter(head, &it, off) == false);
	assert(it == NULL);

	// exhausted handles restart from the head on the next call
	assert(idl_iter(head, &it, off) == true);
	assert(it == &n1);

	// typed iterator usage
	assert(idl_iter(head, (void **)&typed, off) == true);
	assert(typed == &n1);
	assert(idl_iter(head, (void **)&typed, off) == true);
	assert(typed == &n2);

	// start from an arbitrary element
	it = &n2;
	assert(idl_iter(head, &it, off) == true);
	assert(it == &n3);
	assert(idl_iter(head, &it, off) == false);
	assert(it == NULL);

	iter_mixed_offsets();
}
