#include <stddef.h>
#include <stdio.h>

#include "ill/ill/intrusive_linked_lists.h"

typedef struct {
	int      id;
	int      priority;
	t_cm_ill node;
} t_cm_task;

static int cmp_priority(const void *a, const void *b) {
	return (((const t_cm_task *)a)->priority -
	        ((const t_cm_task *)b)->priority);
}

static bool print_cm_task(void *elem, void *arg) {
	(void)arg;
	printf(" [%d/%d]", ((t_cm_task *)elem)->id, ((t_cm_task *)elem)->priority);
	return (true);
}

static bool is_urgent(void *elem, void *arg) {
	(void)arg;
	return (((t_cm_task *)elem)->priority >= 5);
}

static void init_cm_tasks(t_cm_task *tasks, size_t n, const int *prio) {
	for (size_t i = 0; i < n; i++) {
		tasks[i].id       = (int)i;
		tasks[i].priority = prio[i];
		ill_init(&tasks[i].node, offsetof(t_cm_task, node));
	}
}

int main(void) {
	static const int prio[] = {4, 9, 1, 7, 3, 8, 2, 6};
	t_cm_task        tasks[8];
	t_cm_ill        *head     = NULL;
	t_cm_ill        *filtered = NULL;

	init_cm_tasks(tasks, 8, prio);
	for (size_t i = 0; i < 8; i++)
		ill_push_back(&head, &tasks[i].node);

	printf("list (%zu nodes):", ill_length(head));
	ill_foreach(head, print_cm_task, NULL);
	printf("\n");

	printf("head is node %d, tail is node %d\n",
	       ((t_cm_task *)ill_ntoe(head))->id,
	       ((t_cm_task *)ill_ntoe(ill_get_tail(head)))->id);

	ill_sort(&head, cmp_priority);
	printf("sorted by priority:");
	ill_foreach(head, print_cm_task, NULL);
	printf("\n");

	printf("smallest priority task: %d\n",
	       ((t_cm_task *)ill_get_min(head, cmp_priority))->id);
	printf("largest priority task: %d\n",
	       ((t_cm_task *)ill_get_max(head, cmp_priority))->id);

	ill_filter(&head, is_urgent, NULL, &filtered);
	printf("urgent (kept):");
	ill_foreach(head, print_cm_task, NULL);
	printf("\n");
	printf("deferred (filtered out):");
	ill_foreach(filtered, print_cm_task, NULL);
	printf("\n");

	head = ill_reverse(head);
	printf("reversed:");
	ill_foreach(head, print_cm_task, NULL);
	printf("\n");

	return (0);
}
