#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_vector {
	size_t len;
	void  *data;
} t_vector;

typedef struct s_matrix {
	double *data;
	size_t  row;
	size_t  col;
} t_matrix;

t_matrix *matrix_from(void *data, size_t dlen, size_t row, size_t col);
void      matrix_clear(t_matrix *self);
void      matrix_free(t_matrix *self);
t_matrix *matrix_dump(t_matrix *self);

t_matrix *matrix_add(t_matrix *self, const t_matrix *other);
t_matrix *matrix_sub(t_matrix *self, const t_matrix *other);
t_matrix *matrix_scl(t_matrix *self, double scl);

#ifndef C_MINI__MATRIX_IMPL
#	define C_MINI__MATRIX_IMPL

t_matrix *matrix_from(void *data, size_t dlen, size_t row, size_t col) {
	t_matrix *m;
	char     *p;

	if (!data || dlen < row * col) return (NULL);
	p = malloc(sizeof(*m) + row * col);
	if (!p) return (NULL);
	m       = p + row * col;
	m->data = p;
	m->row  = row;
	m->col  = col;
	return (m);
}
void matrix_clear(t_matrix *self) {
	for (size_t i = 0; i < self->col * self->row; i++)
		self->data[i] = 0;
}
void matrix_free(t_matrix *self) {
	matrix_clear(self);
	free(self->data);
}
t_matrix *matrix_dump(t_matrix *self) {
	for (size_t i = 0; i < self->row; i++) { // col
		for (size_t j = 0; j < self->col; j++)
			printf(".\t");
		printf("\n");
	}
}
// t_matrix *matrix_new(void *data, size_t dlen) {}
//
// [a b]   [e f]   [a+e b+f]
// [c d] + [g h] = [c+g d+h]
t_matrix *matrix_add(t_matrix *self, const t_matrix *other) {
	if (!self || !other || self->row != other->row || self->col != other->col)
		return (NULL);
	for (size_t i = 0; i < self->col * self->row; i++)
		self->data[i] += other->data[i];
	return (self);
}
// [a b]   [e f]   [a-e b-f]
// [c d] - [g h] = [c-g d-h]
t_matrix *matrix_sub(t_matrix *self, const t_matrix *other) {
	if (!self || !other || self->row != other->row || self->col != other->col)
		return (NULL);
	for (size_t i = 0; i < self->col * self->row; i++)
		self->data[i] -= other->data[i];
	return (self);
}
// [a b]         [a*scl b*scl]
// [c d] * scl = [c*scl d*scl]
t_matrix *matrix_scl(t_matrix *self, double d) {
	if (!self || self->data) return (NULL);
	for (size_t i = 0; i < self->col * self->row; i++)
		self->data[i] *= d;
	return (self);
}

// linear_comb->[v1, v2][c1, c2]
// ->v1*c1 + v2*c2 + ...
//
// lerp - linear_interpolation
t_matrix *matrix_lerp(t_matrix *self, const t_matrix *o, double c) {
	if (!self || !o || self->row != o->row || self->col != o->col || c > 1 ||
	    c < 0)
		return NULL;
	if (c == 0) return (self);
	for (size_t i = 0; i < self->col * self->row; i++) {
		self->data[i] = self->data[i] * (1 - c) + o->data[i] * c;
	}
	return (self);
}

#endif // C_MINI__MATRIX_IMPL
