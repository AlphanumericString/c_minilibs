#include "intrusive_tree_node.h"

// Get the height of a node (for AVL or generic use)
int tree_height(t_itnp *node) {
	int l, r;

	if (!node) return (-1);
	r = tree_height(node->right);
	l = tree_height(node->left);
	if (l > r) return (l + 1);
	return (r + 1);
}
// Get the size of a subtree (if size is tracked)
size_t tree_size(t_itnp *node) {
	if (!node) return (0);
	return (1 + tree_size(node->left) + tree_size(node->right));
}
