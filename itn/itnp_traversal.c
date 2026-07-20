#include "itn/intrusive_tree_node.h"

// In-order traversal (user provides a callback)
void itnp_inorder(t_itnp *root, void (*callback)(t_itnp *node)) {
	if (!root || !callback) return;
	itnp_inorder(root->left, callback);
	callback(root);
	itnp_inorder(root->right, callback);
	return;
}
// Pre-order traversal
void itnp_preorder(t_itnp *root, void (*callback)(t_itnp *node)) {
	if (!root || !callback) return;
	itnp_inorder(root->left, callback);
	itnp_inorder(root->right, callback);
	callback(root);
	return;
}
// Post-order traversal
void itnp_postorder(t_itnp *root, void (*callback)(t_itnp *node)) {
	if (!root) return;
	callback(root);
	itnp_inorder(root->left, callback);
	itnp_inorder(root->right, callback);
	return;
}
