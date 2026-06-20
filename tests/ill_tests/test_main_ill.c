#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./ill/ill/intrusive_linked_lists.h"

// EXAMPLE

// Example data structure
struct message {
	int   priority;
	char  text[128];
	t_ill node; // Only next pointer
};
bool print_debug_callback(void *element, void *user_data) {
	struct message *msg     = (struct message *)element;

	(void)user_data;
	fflush(stdout);
	printf("[%p w/ %p]\n", msg, (void *)&msg->node); fflush(stdout);
	return (true);
}

// Callback now receives the container directly (no container_of needed!)
bool print_message_callback(void *element, void *user_data) {
	struct message *msg     = (struct message *)element;
	int            *counter = (int *)user_data;

	fflush(stdout);
	printf("[P%d:%s] ", msg->priority, msg->text);
	fflush(stdout);
	if (counter) (*counter)++;
	return (true);
}

bool find_highest_priority_callback(void *element, void *user_data) {
	struct message *msg     = (struct message *)element;
	int            *highest = (int *)user_data;

	if (msg->priority > *highest) *highest = msg->priority;
	return (true);
}

void	*duplicate_message(const void *element) {
	struct message *copy = NULL;
	struct message *src = (struct message *)element;

	copy = malloc(sizeof(*src));
	if (copy) {
		copy->priority = src->priority;
		copy->text[0] = 0;
		strlcpy(copy->text, src->text, sizeof(copy->text));
	}
	return (copy);
}

bool free_message_callback(void *element, void *user_data) {
	struct message *msg = (struct message *)element;
	(void)user_data;
	free(msg);
	return (true);
}

// Example callback that modifies elements
bool double_priority_callback(void *element, void *user_data) {
	struct message *msg = (struct message *)element;
	(void)user_data;
	msg->priority *= 2;
	return (true);
}

int cmp_priority(void *elem1, void *elem2) {
	struct message *m1 = (struct message *)elem1;
	struct message *m2 = (struct message *)elem2;
	return (m1->priority - m2->priority);
}

int main() {
	printf("=== Foreach with Offset Parameter Demo ===\n");

	// Build a list
	struct s_ill *head = NULL;

	struct message *m1 = malloc(sizeof(struct message));
	struct message *m2 = malloc(sizeof(struct message));
	struct message *m3 = malloc(sizeof(struct message));

	m1->priority = 1;
	m2->priority = 5;
	m3->priority = 3;

	snprintf(m1->text, sizeof(m1->text), "Low");
	snprintf(m2->text, sizeof(m2->text), "High");
	snprintf(m3->text, sizeof(m3->text), "Medium");

	ill_init(&m1->node, offsetof(struct message, node));
	ill_init(&m2->node, offsetof(struct message, node));
	ill_init(&m3->node, offsetof(struct message, node));

	ill_push(&head, &m1->node);
	ill_push(&head, &m2->node);
	ill_push(&head, &m3->node);

	// Calculate offset once
	printf("Original list: ");
	int count = 0;
	ill_foreach(head, print_message_callback, &count);
	printf("\nElement count: %d\n", count);

	// Find highest priority
	int highest_priority = 0;
	ill_foreach(head, find_highest_priority_callback, &highest_priority);
	printf("Highest priority: %d\n", highest_priority);

	// Modify elements
	ill_foreach(head, double_priority_callback, NULL);

	printf("After doubling priorities: ");
	ill_foreach(head, print_message_callback, NULL);
	printf("\n");

	struct message *mcopy = ill_copy(head, duplicate_message);
	printf("Copy : ");
	ill_foreach(&mcopy->node, print_message_callback, NULL);
	printf("\n");
	ill_foreach(&mcopy->node, free_message_callback, NULL);

	// struct message *mcopy2 = ill_copy(head, duplicate_message);
	// t_ill *head_copy2 = &mcopy2->node;
	// printf("Copy2 : ");
	// ill_foreach(head_copy2, print_message_callback, NULL);
	// ill_foreach(head_copy2, print_debug_callback, NULL);
	// printf("\n");
	// printf("pop_back: \n");
	// for (t_ill *a = ill_pop_back(&head_copy2); a; a = ill_pop_back(&head_copy2)) {
	// 	printf("{a: %p }", a);
	// 	printf("{head: %p head->nxt:%p}", head_copy2,
	// 		head_copy2 ? head_copy2->next : 0);
	// 	print_message_callback(ill_ntoe(a), NULL); printf("\n"); fflush(stdout);
	// 	free_message_callback(ill_ntoe(a), NULL);
	// }
	// printf("\n");

	// === Additional Tests for the ill/ library ===
	printf("\n=== Additional Tests ===\n");
	printf("List length: %zu\n", ill_length(head));

	t_ill *tail = ill_get_tail(head);
	if (tail) {
		printf("Tail element priority: %d\n", ((struct message *)ill_ntoe(tail))->priority);
	}

	head = ill_reverse(head);
	printf("After reverse: ");
	ill_foreach(head, print_message_callback, NULL);
	printf("\n");

	head = ill_reverse(head); // reverse back

	struct message *max_msg = ill_get_max(head, cmp_priority);
	struct message *min_msg = ill_get_min(head, cmp_priority);
	if (max_msg) printf("Max priority in list: %d\n", max_msg->priority);
	if (min_msg) printf("Min priority in list: %d\n", min_msg->priority);

	printf("Is list circular? %s\n", ill_is_circular(head) ? "yes" : "no");
	printf("Is list sorted? %s\n", ill_is_sorted(head, cmp_priority) ? "yes" : "no");

	if (head && head->next) {
		t_ill *to_rm = head->next;
		printf("Removing element with priority %d...\n", ((struct message *)ill_ntoe(to_rm))->priority);
		if (ill_remove_node(&head, to_rm)) {
			free_message_callback(ill_ntoe(to_rm), NULL);
		}
	}
	printf("After removal: ");
	ill_foreach(head, print_message_callback, NULL);
	printf("\n");

	// Clean up
	ill_foreach(head, free_message_callback, NULL);

	return 0;
}
