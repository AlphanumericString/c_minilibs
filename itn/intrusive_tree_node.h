#ifndef C_MINI__ITN
#define C_MINI__ITN

#include <stddef.h>

typedef struct t_itn {
	struct t_itn *left;
	struct t_itn *right;
	size_t        offset;
} t_itn;

typedef struct t_pitn {
	struct t_pitn *left;
	struct t_pitn *right;
	struct t_pitn *parent;
	size_t         offset;
} t_pitn;

typedef struct t_rbn {
	t_pitn pitn;
	enum { RB_RED, RB_BLACK } color;
} t_rbn;

typedef struct t_avl {
	t_pitn pitn;
	int    height;
} t_avl;

typedef t_pitn t_bst;

// Compare function type for tree ordering
typedef int (*tree_cmp_func)(const void *a, const void *b);

// Insert a node into a BST (returns 0 on success, -1 if duplicate)
int bst_insert(t_pitn *root, t_pitn *new_node, tree_cmp_func cmp);

// Search for a node in a BST (returns the node or NULL)
t_pitn *bst_search(t_pitn *root, const void *key, tree_cmp_func cmp);

// Remove a node from a BST (returns the removed node)
t_pitn *bst_remove(t_pitn *root, t_pitn *node);

// Find the minimum node in a subtree
t_pitn *bst_min(t_pitn *node);

// Find the maximum node in a subtree
t_pitn *bst_max(t_pitn *node);

// Find the successor of a node (in-order)
t_pitn *bst_successor(t_pitn *node);

// Find the predecessor of a node (in-order)
t_pitn *bst_predecessor(t_pitn *node);

// --- Rotation Helpers (for all parent-linked trees) ---
// Left rotate around 'node' (returns the new root of the subtree)
t_pitn *tree_rotate_left(t_pitn *node);

// Right rotate around 'node' (returns the new root of the subtree)
t_pitn *tree_rotate_right(t_pitn *node);

// --- Red-Black Tree Operations (t_rbn) ---
// Insert a node into an RB tree (fixes violations)
void rbn_insert(t_rbn *root, t_rbn *new_node, tree_cmp_func cmp);

// Remove a node from an RB tree (fixes violations)
void rbn_remove(t_rbn *root, t_rbn *node);

// Fix RB tree violations after insertion
void rbn_insert_fixup(t_rbn *root, t_rbn *node);

// Fix RB tree violations after removal
void rbn_remove_fixup(t_rbn *root, t_pitn *x, t_pitn *x_parent);

// --- AVL Tree Operations (t_avl) ---
// Insert a node into an AVL tree (fixes balance)
void avl_insert(t_avl *root, t_avl *new_node, tree_cmp_func cmp);

// Remove a node from an AVL tree (fixes balance)
void avl_remove(t_avl *root, t_avl *node);

// Update height of a node (helper for AVL)
void avl_update_height(t_avl *node);

// Balance a subtree (helper for AVL)
t_avl *avl_balance(t_avl *node);

// --- Tree Traversal ---
// In-order traversal (user provides a callback)
void tree_inorder(t_pitn *root, void (*callback)(t_pitn *node));

// Pre-order traversal
void tree_preorder(t_pitn *root, void (*callback)(t_pitn *node));

// Post-order traversal
void tree_postorder(t_pitn *root, void (*callback)(t_pitn *node));

// --- Utility Functions ---
// Check if a tree is valid (BST property)
int bst_validate(t_pitn *root, tree_cmp_func cmp);

// Check if an RB tree is valid (BST + RB properties)
int rbn_validate(t_rbn *root, tree_cmp_func cmp);

// Check if an AVL tree is valid (BST + AVL properties)
int avl_validate(t_avl *root, tree_cmp_func cmp);

// Get the height of a node (for AVL or generic use)
int tree_height(t_pitn *node);

// Get the size of a subtree (if size is tracked)
size_t tree_size(t_pitn *node);

#endif // C_MINI__ITN_H
