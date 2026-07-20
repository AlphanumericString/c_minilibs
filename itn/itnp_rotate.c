#include "intrusive_tree_node.h"

// Left rotate around 'node' (returns the new root of the subtree)
t_itnp *itnp_rotate_left(t_itnp *node) {
	t_itnp *right;
	t_itnp *right__left;

	if (!node) return (NULL);
	if (!node->right) return (node);
	right       = node->right;
	right__left = right->left;
	right->left = node;
	node->right = right__left;
// TODO:-- parents;
	return (right);
}
// Right rotate around 'node' (returns the new root of the subtree)
t_itnp *itnp_rotate_right(t_itnp *node) {
	t_itnp *left;
	t_itnp *left_right;

	if (!node) return (NULL);
	if (!node->left) return (node);
	left        = node->left;
	left_right  = left->right;
	left->right = node;
	node->left  = left_right;
// TODO:-- parents;
	return (left);
}
