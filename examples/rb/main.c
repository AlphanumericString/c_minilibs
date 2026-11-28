#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	size_t n = rb_size(*rb);

	printf("%-18s size=%zu cap=%zu free=%zu :", label, n, rb_capacity(*rb),
	       rb_remaining_space(*rb));
	rb_napply(rb, n < 8 ? n : 8, print_int, NULL);
	if (n > 8) printf(" ...");
	printf("\n");
}

int main(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int     v;

	// the capacity is rounded up to a whole page so the buffer can be
	// mapped twice, back to back
	rb_create(&rb, sizeof(int), 4);
	cap = rb_capacity(rb);
	printf("magic=%s cap=%zu\n", rb_is_magic(rb) ? "yes" : "no", cap);

	for (v = 1; v <= 3; v++)
		rb_push(&rb, &v);
	dump(&rb, "after 3 pushes");

	// fill up to the window edge, then a bit more to wrap around
	for (v = 4; v <= (int)cap + 2; v++)
		rb_push(&rb, &v);
	dump(&rb, "after wrap-around");

	// a wrapped window can be filled with a single bulk copy
	rb_clr(&rb);
	int seed[] = {1, 2, 3, 4, 5, 6};
	rb_npush(&rb, seed, sizeof(seed) / sizeof(seed[0]));
	dump(&rb, "after npush(6)");

	printf("dropped oldest 1: %s\n", rb_drop(&rb, 1) ? "yes" : "no");
	dump(&rb, "after drop(1)");

	int popped;
	rb_pop(&rb, &popped);
	printf("pop -> %d\n", popped);
	printf("peek -> %d\n", *(int *)rb_peek(rb, NULL));
	dump(&rb, "after pop");

	printf("push_safe (room left): %s\n",
	       rb_push_safe(&rb, &popped) ? "ok" : "rejected");

	// a non magic buffer (from_array) keeps the exact capacity and rejects a
	// push once full; from_array takes ownership of the heap array
	int    *raw          = malloc(4 * sizeof(int));
	t_cm_rb small        = {0};
	int     small_vals[] = {1, 2, 3, 4};

	memcpy(raw, small_vals, sizeof(small_vals));
	rb_from_array(&small, raw, 4, sizeof(int));
	for (size_t i = 0; i < rb_capacity(small); i++)
		rb_push(&small, &small_vals[i]);
	printf("from_array: magic=%s cap=%zu push_safe while full: %s\n",
	       rb_is_magic(small) ? "yes" : "no", rb_capacity(small),
	       rb_push_safe(&small, &popped) ? "ok" : "rejected");
	rb_destroy(&small);

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

	// growing always yields a conforming (magic when available) buffer
	t_cm_rb *bigger = rb_grow(&rb, 4);
	rb_destroy(&rb);
	rb = *bigger;
	free(bigger);
	printf("after grow: cap=%zu magic=%s\n", rb_capacity(rb),
	       rb_is_magic(rb) ? "yes" : "no");

	rb_destroy(&rb);
	return (0);
}
