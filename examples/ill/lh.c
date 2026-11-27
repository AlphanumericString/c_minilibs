#include <stddef.h>
#include <stdio.h>

#include "ill/lh/list_handler.h"

typedef struct {
	int id;
	int value;
} t_cm_prefix;

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

static bool print_item(void *elem, void *arg) {
	(void)arg;
	printf(" %d", ((t_cm_prefix *)elem)->value);
	return (true);
}

int main(void) {
	t_cm_sitem s[3];
	t_cm_ditem d[3];
	t_cm_lh    sh;
	t_cm_lh    dh;
	size_t     off_s = offsetof(t_cm_sitem, node);
	size_t     off_d = offsetof(t_cm_ditem, dnode);
	size_t     i;

	lh_init(&sh, CM_LH_SINGLY, off_s);
	lh_init(&dh, CM_LH_DOUBLY, off_d);

	for (i = 0; i < 3; i++) {
		s[i].id    = (int)i;
		s[i].value = (int)(i + 1);
		ill_init_node(&s[i].node, off_s);
		lh_push_back(&sh, &s[i]);

		d[i].id    = (int)i;
		d[i].value = (int)(i + 11);
		idl_init_node(&d[i].dnode, off_d);
		lh_push_back(&dh, &d[i]);
	}

	printf("singly handler (%zu):", lh_length(&sh));
	lh_foreach(&sh, print_item, NULL);
	printf("\n");
	printf("head=%d tail=%d\n", ((t_cm_prefix *)lh_get_head(&sh))->value,
	       ((t_cm_prefix *)lh_get_tail(&sh))->value);

	printf("doubly handler (%zu):", lh_length(&dh));
	lh_foreach(&dh, print_item, NULL);
	printf("\n");
	printf("head=%d tail=%d\n", ((t_cm_prefix *)lh_get_head(&dh))->value,
	       ((t_cm_prefix *)lh_get_tail(&dh))->value);

	{
		t_cm_prefix *sf = lh_pop(&sh);
		t_cm_prefix *sb = lh_pop_back(&sh);
		t_cm_prefix *df = lh_pop(&dh);
		t_cm_prefix *db = lh_pop_back(&dh);

		printf("popped singly front=%d back=%d\n", sf->value, sb->value);
		printf("popped doubly front=%d back=%d\n", df->value, db->value);
	}

	// the same handler calls dispatch to the circular implementation
	lh_init(&sh, CM_LH_CIRC_SINGLY, off_s);
	lh_init(&dh, CM_LH_CIRC_DOUBLY, off_d);
	for (i = 0; i < 3; i++) {
		lh_push_back(&sh, &s[i]);
		lh_push_back(&dh, &d[i]);
	}
	printf("circular singly handler (%zu):", lh_length(&sh));
	lh_foreach(&sh, print_item, NULL);
	printf("\n");
	printf("circular doubly handler (%zu):", lh_length(&dh));
	lh_foreach(&dh, print_item, NULL);
	printf("\n");

	lh_clear(&sh, NULL);
	lh_clear(&dh, NULL);
	return (0);
}
