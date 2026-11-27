#include <stddef.h>
#include <stdio.h>

#include "ill/idl/intrusive_doubly_linked.h"
#include "ill/ill/intrusive_linked_lists.h"

typedef struct {
	int      id;
	int      value;
	t_cm_ill node;
} t_cm_sitem;

typedef struct {
	int      id;
	int      value;
	t_cm_idl dnode;
} t_cm_ditem;

static bool print_sitem(void *elem, void *arg) {
	(void)arg;
	printf(" %d", ((t_cm_sitem *)elem)->value);
	return (true);
}

static bool print_ditem(void *elem, void *arg) {
	(void)arg;
	printf(" %d", ((t_cm_ditem *)elem)->value);
	return (true);
}

int main(void) {
	t_cm_sitem s[3];
	t_cm_ditem d[3];
	t_cm_ill  *shead = NULL;
	t_cm_idl  *dhead = NULL;
	size_t     i;

	for (i = 0; i < 3; i++) {
		s[i].id    = (int)i;
		s[i].value = (int)(i + 1);
		ill_init_node(&s[i].node, offsetof(t_cm_sitem, node));
		ill_circ_push_back_node(&shead, &s[i].node);

		d[i].id    = (int)i;
		d[i].value = (int)(i + 11);
		idl_init_node(&d[i].dnode, offsetof(t_cm_ditem, dnode));
		idl_circ_push_back_node(&dhead, &d[i].dnode);
	}

	printf("singly circular (%zu):", ill_circ_length(shead));
	ill_circ_foreach(shead, print_sitem, NULL);
	printf("\n");
	printf("head=%d tail=%d\n",
	       ((t_cm_sitem *)ill_ntoe(ill_circ_get_head_node(shead)))->value,
	       ((t_cm_sitem *)ill_ntoe(ill_circ_get_tail_node(shead)))->value);

	printf("doubly circular (%zu):", idl_circ_length(dhead));
	idl_circ_foreach(dhead, print_ditem, NULL);
	printf("\n");
	printf("head=%d tail=%d\n",
	       ((t_cm_ditem *)idl_ntoe(idl_circ_get_head_node(dhead)))->value,
	       ((t_cm_ditem *)idl_ntoe(idl_circ_get_tail_node(dhead)))->value);

	printf("doubly circular reversed:");
	idl_circ_foreach_rev(dhead, print_ditem, NULL);
	printf("\n");

	{
		t_cm_sitem *sf = ill_circ_pop(&shead);
		t_cm_sitem *sb = ill_circ_pop_back(&shead);
		t_cm_ditem *df = idl_circ_pop(&dhead);
		t_cm_ditem *db = idl_circ_pop_back(&dhead);

		printf("popped singly front=%d back=%d\n", sf->value, sb->value);
		printf("popped doubly front=%d back=%d\n", df->value, db->value);
	}

	ill_circ_clear(&shead, NULL);
	idl_circ_clear(&dhead, NULL);
	return (0);
}
