#include <stdio.h>
#include <stdlib.h>

#include "rb/round_buffer.h"

static int cmp_int(const void *a, const void *b) {
	return (*(const int *)a - *(const int *)b);
}

static bool print_int(void *elem, void *arg) {
	(void)arg;
	printf(" %d", *(int *)elem);
	return (true);
}

static bool keep_above_two(void *elem, void *arg) {
	(void)arg;
	return (*(int *)elem > 2);
}

static void *dup_int(const void *elem) {
	int *copy = malloc(sizeof(int));
	if (copy) *copy = *(const int *)elem;
	return (copy);
}

static void dump(const t_cm_rb *rb, const char *label) {
	printf("%-18s size=%zu cap=%zu free=%zu :", label, rb_size(*rb),
	       rb_capacity(*rb), rb_remaining_space(*rb));
	rb_apply(rb, print_int, NULL);
	printf("\n");
}

int main(void) {
	t_cm_rb rb     = {0};
	int     seed[] = {1, 2, 3, 4, 5, 6};

	rb_create(&rb, sizeof(int), 4);
	for (size_t i = 0; i < 3; i++)
		rb_push(&rb, &seed[i]);
	dump(&rb, "after 3 pushes");

	for (size_t i = 3; i < 6; i++)
		rb_push(&rb, &seed[i]);
	dump(&rb, "after wrap-around");

	printf("dropped oldest 1: %s\n", rb_drop(&rb, 1) ? "yes" : "no");
	dump(&rb, "after drop(1)");

	int popped;
	rb_pop(&rb, &popped);
	printf("pop -> %d\n", popped);
	printf("peek -> %d\n", *(int *)rb_peek(rb, NULL));
	dump(&rb, "after pop");

	printf("push_safe (room left): %s\n",
	       rb_push_safe(&rb, &popped) ? "ok" : "rejected");
	int extra = 7;
	while (rb_push_safe(&rb, &extra))
		;
	printf("push_safe while full : rejected (size=%zu cap=%zu)\n", rb_size(rb),
	       rb_capacity(rb));

	dump(&rb, "before filter");

	int found_val;
	if (rb_get(&rb, cmp_int, &popped, &found_val))
		printf("rb_get(%d) -> %d\n", popped, found_val);

	rb_filter(&rb, keep_above_two, NULL, NULL);
	dump(&rb, "after filter >2");

	int *mapped[4] = {0};
	if (rb_map(&rb, dup_int, (void **)mapped)) {
		printf("mapped:");
		for (size_t i = 0; i < rb_size(rb); i++) {
			printf(" %d", *mapped[i]);
			free(mapped[i]);
		}
		printf("\n");
	}

	t_cm_rb *bigger = rb_grow(&rb, 4);
	rb_destroy(&rb);
	rb = *bigger;
	free(bigger);
	printf("after grow: cap=%zu\n", rb_capacity(rb));

	rb_destroy(&rb);
	return (0);
}
