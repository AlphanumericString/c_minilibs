#include <assert.h>
#include <stdlib.h>

#include "rb_tests.h"

static void fill_seq(t_cm_rb *rb, size_t n) {
	size_t i;

	for (i = 0; i < n; i++) {
		int v = (int)i + 1;
		rb_push(rb, &v);
	}
}

static void reset_five(t_cm_rb *rb) {
	int vals[5] = {1, 2, 3, 4, 5};

	rb_clr(rb);
	rb_npush(rb, vals, 5);
}

static size_t build_wrapped(t_cm_rb *rb, bool magic) {
	size_t cap;

	if (magic) {
		assert(rb_create_magic(rb, sizeof(int), 8) != NULL);
		cap = rb_capacity(*rb);
	} else {
		int *arr = malloc(8 * sizeof(int));

		assert(arr != NULL);
		assert(rb_from_array(rb, arr, 8, sizeof(int)) != NULL);
		cap = rb_capacity(*rb);
	}
	fill_seq(rb, cap);
	rb->o_pos = rb->nb_max - 2;
	return (cap);
}

NO_COV void test_cm_rb_iter(void) {
	t_cm_rb rb = {0};
	void   *it_val;
	size_t  count;
	int     sum;

	rb_init(&rb, sizeof(int));
	fill_seq(&rb, 5);

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (rb_iter(&rb, 0, (size_t)-1, &it_val)) {
		assert(it_val == rb_at(rb, count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(rb_iter(&rb, 0, (size_t)-1, &it_val) == true);
	assert(it_val == rb_at(rb, 0));

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (rb_iter(&rb, 1, 3, &it_val)) {
		assert(it_val == rb_at(rb, count + 1));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 2);
	assert(sum == 5);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (rb_iter(&rb, 3, 99, &it_val))
		count++;
	assert(count == 2);
	assert(it_val == NULL);

	it_val = (void *)0x1;
	assert(rb_iter(&rb, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(rb_iter(&rb, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(rb_iter(&rb, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(rb_iter(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	assert(rb_iter(&rb, 0, (size_t)-1, NULL) == false);

	t_cm_rb bad = {0};
	it_val      = (void *)0x1;
	assert(rb_iter(&bad, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	t_cm_rb partial = {0};
	partial.array   = &partial;
	partial.sz_e    = sizeof(int);
	partial.nb_max  = 0;
	it_val          = (void *)0x1;
	assert(rb_iter(&partial, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	partial.nb_max = 1;
	partial.sz_e   = 0;
	it_val         = (void *)0x1;
	assert(rb_iter(&partial, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	rb_clr(&rb);
	it_val = (void *)0x1;
	assert(rb_iter(&rb, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_iter_rev(void) {
	t_cm_rb rb = {0};
	void   *it_val;
	size_t  count;
	int     sum;

	rb_init(&rb, sizeof(int));
	fill_seq(&rb, 5);

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (rb_iter_rev(&rb, 0, (size_t)-1, &it_val)) {
		assert(it_val == rb_at(rb, rb_size(rb) - 1 - count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(rb_iter_rev(&rb, 0, (size_t)-1, &it_val) == true);
	assert(it_val == rb_at(rb, rb_size(rb) - 1));

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (rb_iter_rev(&rb, 1, 3, &it_val)) {
		assert(it_val == rb_at(rb, 2 - count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 2);
	assert(sum == 5);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (rb_iter_rev(&rb, 3, 99, &it_val))
		count++;
	assert(count == 2);
	assert(it_val == NULL);

	it_val = (void *)0x1;
	assert(rb_iter_rev(&rb, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(rb_iter_rev(&rb, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(rb_iter_rev(&rb, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(rb_iter_rev(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	assert(rb_iter_rev(&rb, 0, (size_t)-1, NULL) == false);

	t_cm_rb bad = {0};
	it_val      = (void *)0x1;
	assert(rb_iter_rev(&bad, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	rb_clr(&rb);
	it_val = (void *)0x1;
	assert(rb_iter_rev(&rb, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_iter_range(void) {
	t_cm_rb     rb = {0};
	t_rb_it_ctx ctx;

	rb_init(&rb, sizeof(int));
	fill_seq(&rb, 5);

	ctx = (t_rb_it_ctx){0};
	assert(rb_iter_range(&rb, 0, (size_t)-1, cb_it_collect, &ctx) == true);
	assert(ctx.count == 5);
	assert(ctx.sum == 15);
	assert(ctx.first_pos == 0);
	assert(ctx.last_pos == 4);
	assert(ctx.saw_last == true);

	ctx = (t_rb_it_ctx){0};
	assert(rb_iter_range(&rb, 1, 4, cb_it_collect, &ctx) == true);
	assert(ctx.count == 3);
	assert(ctx.sum == 9);
	assert(ctx.first_pos == 1);
	assert(ctx.last_pos == 3);
	assert(ctx.saw_last == true);

	ctx = (t_rb_it_ctx){0};
	assert(rb_iter_range(&rb, 3, 99, cb_it_collect, &ctx) == true);
	assert(ctx.count == 2);
	assert(ctx.first_pos == 3);
	assert(ctx.last_pos == 4);
	assert(ctx.saw_last == true);

	ctx         = (t_rb_it_ctx){0};
	ctx.stop_at = 2;
	assert(rb_iter_range(&rb, 0, (size_t)-1, cb_it_stop, &ctx) == false);
	assert(ctx.count == 2);
	assert(ctx.sum == 3);
	assert(ctx.saw_last == false);

	ctx = (t_rb_it_ctx){0};
	assert(rb_iter_range(&rb, 2, 2, cb_it_collect, &ctx) == true);
	assert(ctx.count == 0);
	assert(rb_iter_range(&rb, 5, (size_t)-1, cb_it_collect, &ctx) == true);
	assert(ctx.count == 0);
	assert(rb_iter_range(&rb, 3, 1, cb_it_collect, &ctx) == true);
	assert(ctx.count == 0);

	assert(rb_iter_range(NULL, 0, (size_t)-1, cb_it_collect, &ctx) == false);
	assert(rb_iter_range(&rb, 0, (size_t)-1, NULL, &ctx) == false);

	t_cm_rb bad = {0};
	assert(rb_iter_range(&bad, 0, (size_t)-1, cb_it_collect, &ctx) == false);

	rb_clr(&rb);
	assert(rb_iter_range(&rb, 0, (size_t)-1, cb_it_collect, &ctx) == true);
	assert(ctx.count == 0);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_advance_it(void) {
	t_cm_rb rb = {0};
	void   *it;
	int    *typed;
	int     out;
	size_t  i;

	rb_init(&rb, sizeof(int));
	fill_seq(&rb, 5);

	it = NULL;
	for (i = 0; i < 5; i++) {
		it = rb_advance_it(&rb, it);
		assert(it == rb_at(rb, i));
	}
	assert(rb_advance_it(&rb, it) == NULL);
	assert(rb_advance_it(&rb, NULL) == rb_at(rb, 0));

	typed = NULL;
	typed = rb_advance_it(&rb, typed);
	assert(typed != NULL && *typed == 1);
	typed = rb_advance_it(&rb, typed);
	assert(typed != NULL && *typed == 2);

	assert(rb_advance_it(NULL, NULL) == NULL);
	t_cm_rb bad = {0};
	assert(rb_advance_it(&bad, NULL) == NULL);

	reset_five(&rb);
	it = rb_at(rb, 0);
	assert(rb_pop(&rb, &out) != NULL);
	assert(rb_advance_it(&rb, it) == NULL);

	rb_clr(&rb);
	assert(rb_advance_it(&rb, NULL) == NULL);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_recede_it(void) {
	t_cm_rb rb = {0};
	void   *it;
	int    *typed;
	int     out;
	size_t  i;

	rb_init(&rb, sizeof(int));
	fill_seq(&rb, 5);

	it = NULL;
	for (i = 5; i > 0; i--) {
		it = rb_recede_it(&rb, it);
		assert(it == rb_at(rb, i - 1));
	}
	assert(rb_recede_it(&rb, it) == NULL);
	assert(rb_recede_it(&rb, NULL) == rb_at(rb, 4));

	typed = NULL;
	typed = rb_recede_it(&rb, typed);
	assert(typed != NULL && *typed == 5);
	typed = rb_recede_it(&rb, typed);
	assert(typed != NULL && *typed == 4);

	assert(rb_recede_it(NULL, NULL) == NULL);
	t_cm_rb bad = {0};
	assert(rb_recede_it(&bad, NULL) == NULL);

	reset_five(&rb);
	it = rb_at(rb, 0);
	assert(rb_pop(&rb, &out) != NULL);
	assert(rb_recede_it(&rb, it) == NULL);

	rb_clr(&rb);
	assert(rb_recede_it(&rb, NULL) == NULL);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_iter_wrap(void) {
	t_cm_rb rb;
	size_t  cap;
	size_t  i;
	void   *it;
	bool    second;
	bool    magic;

	for (int m = 0; m < 2; m++) {
		magic = m != 0;
		rb    = (t_cm_rb){0};
		cap   = build_wrapped(&rb, magic);

		second = false;
		it     = NULL;
		i      = 0;
		while (rb_iter(&rb, 0, (size_t)-1, &it)) {
			void *expected = rb_at(rb, i);

			assert(it == expected);
			if ((unsigned char *)expected >=
			    (unsigned char *)rb.array + rb.nb_max * rb.sz_e)
				second = true;
			i++;
		}
		assert(i == cap);
		assert(it == NULL);
		assert(second == magic);

		it = NULL;
		i  = 0;
		while (rb_iter_rev(&rb, 0, (size_t)-1, &it)) {
			assert(it == rb_at(rb, cap - 1 - i));
			i++;
		}
		assert(i == cap);
		assert(it == NULL);

		it = rb_at(rb, 0);
		for (i = 1; i < cap; i++) {
			it = rb_advance_it(&rb, it);
			assert(it == rb_at(rb, i));
		}
		assert(rb_advance_it(&rb, it) == NULL);

		it = rb_at(rb, cap - 1);
		for (i = cap - 1; i > 0; i--) {
			it = rb_recede_it(&rb, it);
			assert(it == rb_at(rb, i - 1));
		}
		assert(rb_recede_it(&rb, it) == NULL);

		rb_destroy(&rb);
	}
}

NO_COV void test_cm_rb_iter_shift(void) {
	t_cm_rb rb = {0};
	void   *it;
	int     out;
	int     extra;
	int     last;

	rb_init(&rb, sizeof(int));
	reset_five(&rb);

	// a push mid-walk is visible and the walk keeps its place
	it = NULL;
	assert(rb_iter(&rb, 0, (size_t)-1, &it) == true);
	assert(it == rb_at(rb, 0));
	extra = 6;
	rb_push(&rb, &extra);
	assert(rb_iter(&rb, 0, (size_t)-1, &it) == true);
	assert(it == rb_at(rb, 1));
	last = *(int *)it;
	while (rb_iter(&rb, 0, (size_t)-1, &it))
		last = *(int *)it;
	assert(last == 6);

	// a pop consuming the current element stops the walk instead of dangling
	reset_five(&rb);
	it = NULL;
	assert(rb_iter(&rb, 0, (size_t)-1, &it) == true);
	assert(rb_pop(&rb, &out) != NULL);
	assert(rb_iter(&rb, 0, (size_t)-1, &it) == false);
	assert(it == NULL);
	assert(rb_iter(&rb, 0, (size_t)-1, &it) == true);
	assert(*(int *)it == 2);

	// a head move that puts the current element before start stops the walk
	reset_five(&rb);
	it = NULL;
	assert(rb_iter(&rb, 1, (size_t)-1, &it) == true);
	assert(it == rb_at(rb, 1));
	assert(rb_pop(&rb, &out) != NULL);
	assert(rb_iter(&rb, 1, (size_t)-1, &it) == false);
	assert(it == NULL);

	// the reverse walk also stops once its element is consumed
	reset_five(&rb);
	it = NULL;
	while (rb_iter_rev(&rb, 0, 5, &it)) {
		if (it == rb_at(rb, 0)) break;
	}
	assert(it == rb_at(rb, 0));
	assert(rb_pop(&rb, &out) != NULL);
	assert(rb_iter_rev(&rb, 0, (size_t)-1, &it) == false);
	assert(it == NULL);

	rb_destroy(&rb);
}
