#include <stdbool.h>
#include <stdlib.h>

#include "xarray.h"

t_xarray *xa_new(void) {
	t_xarray *xret;

	xret = malloc(sizeof(*xret));
	if (!xret) {
		return (NULL);
	}
	*xret = (t_xarray){0};
	return (xret);
}

t_xarray *xa_init(t_xarray *to_init) {
	if (!to_init) {
		return (NULL);
	}
	*to_init = (t_xarray){0};
	return (to_init);
}
bool xai_delete_node(struct s_xarray_node *node) {
	if (!node) return false;
	for (size_t i = 0; i < C_MINI___XA_NB_NODES; i++) {
		// if (IS_NODE()) -> call recursively, set null
		// else if != NULL -> to null
		// else continue
	}
	return (true);
}
bool xa_delete(t_xarray *to_del) {
	if (!to_del) {
		return false;
	}
	for (size_t i = 0; i < C_MINI___XA_NB_NODES; i++) {
		if (to_del->nodes[i]) {
			xai_delete_node(to_del->nodes[i]);
			to_del->nodes[i] = NULL;
		}
	}
	*to_del = (t_xarray){0};
	return (true);
}
bool xa_free(t_xarray *to_free) {
	if (!to_free) return false;
	xa_delete(to_free);
	free(to_free);
	return (true);
}
