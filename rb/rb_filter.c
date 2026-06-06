#include <string.h>

#include "round_buffer.h"

// blank function to reduce branching at exec
static void blank(void *a) {
	(void)a;
}

/**
 * \brief filter elements in the buffer, keeping those where cb returns true
 * \param rb the round buffer
 * \param cb the callback function, if false: remove, if true: keep
 * \param arg argument for the callback
 * \param destroyer optional function to destroy removed elements
 * \return true on success, false otherwise
**/
bool rb_filter(t_rb *rb, t_callback cb, const void *arg,
               void (*destroyer)(void *elem)) {
	size_t read_pos;
	size_t write_pos;
	size_t count;
	size_t i = 0;
	void  *elem;

	if (!rb || !cb) return false;
	if (!destroyer) destroyer = blank;

	count     = rb_size(*rb);
	read_pos  = rb->o_pos;
	write_pos = rb->o_pos;

	while (i < count) {
		elem = rb->array + (read_pos * rb->sz_e);
		if (cb(elem, (void *)arg)) {
			if (read_pos != write_pos)
				memcpy(rb->array + (write_pos * rb->sz_e), elem, rb->sz_e);
			write_pos = (write_pos + 1) % rb->nb_max;
		} else {
			destroyer(elem);
		}
		read_pos = (read_pos + 1) % rb->nb_max;
		i++;
	}
	rb->i_pos = write_pos;
	return true;
}
