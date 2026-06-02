#include <stdio.h>

#include "./rb/round_buffer.h"

#define RB_DISP(rb)                                                            \
	printf("RB info\n"                                                         \
	       "\t.cappacity %zu\n\t.size_element %zu\n\t.array %p\n\t.tail "      \
	       "%zu\n\t.head %zu\n",                                               \
	       rb.nb_max, rb.sz_e, rb.array, rb.i_pos, rb.o_pos);

void display_whole_rb(t_rb *rb) {
	long buff[16] = {0};

	printf("whole:\t");
	rb_npeek(*rb, 16, buff);
	for (int i = 0; i < 16; i++) {
		printf("%ld", buff[i]);
		if (i != 16 - 1) printf(", ");
	}
	printf("\n");
}

void test_base(void) {
	t_rb rb = {0};
	int  x  = 42;
	int  y  = -1;

	printf("base:\n");
	RB_DISP(rb);

	printf("init:\n");
	rb_init(&rb, sizeof(int));
	RB_DISP(rb);

	printf("push:\n");
	rb_push(&rb, &x);
	RB_DISP(rb);

	printf("peek:\n");
	printf("y:%d\n", y);
	rb_peek(rb, &y);
	printf("y:%d\n", y);
	RB_DISP(rb);

	y = -1;
	printf("pop:\n");
	printf("y:%d\n", y);
	rb_pop(&rb, &y);
	printf("y:%d\n", y);
	RB_DISP(rb);

	printf("destroy:\n");
	rb_destroy(&rb);
	RB_DISP(rb);
}
void test_advanced(void) {
	t_rb         rb = {0};
	long         a;
	const size_t rb_total_size = 16;
	const size_t buffer_size   = 10;
	long         buffer[10];

	a = 0;
	rb_create(&rb, sizeof(long), rb_total_size + 1);
	for (size_t i = 0; i < rb_total_size; i++) {
		a++;
		rb_push(&rb, &a);
	}
	printf("start:\n");
	display_whole_rb(&rb);

	printf("npeek (10):\n");
	rb_npeek(rb, buffer_size, buffer);
	for (size_t i = 0; i < buffer_size; i++)
		printf("%ld ", buffer[i]);
	printf("\n");
	display_whole_rb(&rb);

	printf("drop(8):\n");
	rb_drop(&rb, 8);
	display_whole_rb(&rb);

	printf("pushing %zu\n", rb_total_size - 8);
	for (size_t i = 0; i < rb_total_size - 8; i++) {
		a++;
		rb_push(&rb, &a);
	}
	display_whole_rb(&rb);

	printf("npop:\n");
	rb_npop(&rb, buffer_size, buffer);
	for (size_t i = 0; i < buffer_size; i++)
		printf("%ld ", buffer[i]);
	printf("\n");
	display_whole_rb(&rb);

	printf("npop:\n");
	rb_npop(&rb, buffer_size, buffer);
	for (size_t i = 0; i < buffer_size; i++)
		printf("%ld ", buffer[i]);
	printf("\n");
	display_whole_rb(&rb);
	rb_destroy(&rb);
}

int main(void) {
	// test_base();
	test_advanced();
	return (0);
}
