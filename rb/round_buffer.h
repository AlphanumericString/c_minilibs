#ifndef PRIV_RB
#	include <stdlib.h>
#	include <stddef.h>
#	include <stdbool.h>

/**
 * \brief round buffer struct, rb
 * \param array the raw buffer pointer
 * \param nb_max number max of elements in the array
 * \param i_pos input position
 * \param o_pos output position
 * \param sz_e size of elements in the array
 **/
typedef struct s_roundbuffer_array {
	void  *array;
	size_t nb_max;

	size_t i_pos;
	size_t o_pos;

	size_t sz_e;
} t_rb;

// init
t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e);
t_rb *rb_init(t_rb *rb, size_t sz_elem);
t_rb *rb_from_array(t_rb *rb, void *array, size_t nb_elem, size_t sz_elem);
t_rb *rb_new(size_t sz_elem);

// delete
void rb_destroy(t_rb *rb);
void rb_clr(t_rb *rb);

// getters
bool   rb_full(t_rb rb);
bool   rb_empty(t_rb rb);
size_t rb_size(t_rb rb);
size_t rb_cappacity(t_rb rb);
size_t rb_free(t_rb rb);

// remove
bool  rb_drop(t_rb *rb, size_t n);
void *rb_pop(t_rb *rb, void *elem_out);
void *rb_npop(t_rb *rb, size_t n, void *elem_out);
void *rb_peek(t_rb rb, void *elem_out);
void *rb_npeek(t_rb rb, size_t n, void *elem_out);

// add
void rb_push(t_rb *rb, const void *data);
void rb_npush(t_rb *rb, const void *data, size_t nb_elem);
bool rb_push_safe(t_rb *rb, const void *data);
bool rb_npush_safe(t_rb *rb, const void *data, size_t nb_elem);

#	define PRB_NB_ELEM_DFLT 4
#	define PRB_SZ_ELEM_DFLT 1024

#	define PRIV_RB
#endif // !PRIV_RB
