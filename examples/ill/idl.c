#include <stddef.h>
#include <stdio.h>

#include "ill/idl/intrusive_doubly_linked.h"

typedef struct {
	int      id;
	int      value;
	t_cm_idl node;
} t_cm_item;

static int cmp_value(const void *a, const void *b) {
	return (((const t_cm_item *)a)->value - ((const t_cm_item *)b)->value);
}

static bool print_cm_item(void *elem, void *arg) {
	(void)arg;
	printf(" %d", ((t_cm_item *)elem)->value);
	return (true);
}

static void init_cm_items(t_cm_item *items, size_t n, const int *values) {
	for (size_t i = 0; i < n; i++) {
		items[i].id    = (int)i;
		items[i].value = values[i];
		idl_init(&items[i].node, offsetof(t_cm_item, node));
	}
}

int main(void) {
	static const int values[] = {30, 10, 50, 20, 40};
	t_cm_item        items[5];
	t_cm_idl        *head = NULL;

	init_cm_items(items, 5, values);
	for (size_t i = 0; i < 5; i++)
		idl_push_back(&head, &items[i].node);

	printf("list (%zu nodes):", idl_length(head));
	idl_foreach(head, print_cm_item, NULL);
	printf("\n");
	printf("head=%d tail=%d\n",
	       ((t_cm_item *)idl_ntoe(idl_get_head(head)))->value,
	       ((t_cm_item *)idl_ntoe(idl_get_tail(head)))->value);

	idl_sort(&head, cmp_value);
	printf("sorted:");
	idl_foreach(head, print_cm_item, NULL);
	printf("\n");
	printf("sorted flag: %s\n",
	       idl_is_sorted(head, cmp_value) ? "true" : "false");

	t_cm_idl *back  = idl_pop_back(&head);
	t_cm_idl *front = idl_pop(&head);
	printf("popped front=%d back=%d\n", ((t_cm_item *)idl_ntoe(front))->value,
	       ((t_cm_item *)idl_ntoe(back))->value);

	head = idl_reverse(head);
	printf("reversed:");
	idl_foreach(head, print_cm_item, NULL);
	printf("\n");

	return (0);
}
