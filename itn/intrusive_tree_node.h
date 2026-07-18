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
} t_itnp;

typedef struct t_rbn {
	t_itnp pitn;
	enum { RB_RED, RB_BLACK } color;
} t_itn_rb;

typedef struct t_avl {
	t_itnp pitn;
	int    height;
} t_itn_avl;

typedef t_itnp t_itn_bst;

// Compare function type for tree ordering
typedef int (*tree_cmp_func)(const void *a, const void *b);

// --- General tree functions ---
// ------ Rotation Helpers (for all parent-linked trees) ------
// Left rotate around 'node' (returns the new root of the subtree)
t_itnp *tree_rotate_left(t_itnp *node);
// Right rotate around 'node' (returns the new root of the subtree)
t_itnp *tree_rotate_right(t_itnp *node);
// In-order traversal (user provides a callback)
void    tree_inorder(t_itnp *root, void (*callback)(t_itnp *node));
// Pre-order traversal
void    tree_preorder(t_itnp *root, void (*callback)(t_itnp *node));
// Post-order traversal
void    tree_postorder(t_itnp *root, void (*callback)(t_itnp *node));
// Get the height of a node (for AVL or generic use)
int     tree_height(t_itnp *node);
// Get the size of a subtree (if size is tracked)
size_t  tree_size(t_itnp *node);

// --- Binary Search Tree ---
// Insert a node into a BST (returns 0 on success, -1 if duplicate)
int     bst_insert(t_itnp *root, t_itnp *new_node, tree_cmp_func cmp);
// Search for a node in a BST (returns the node or NULL)
t_itnp *bst_search(t_itnp *root, const void *key, tree_cmp_func cmp);
// Remove a node from a BST (returns the removed node)
t_itnp *bst_remove(t_itnp *root, t_itnp *node);
// Find the minimum node in a subtree
t_itnp *bst_min(t_itnp *node);
// Find the maximum node in a subtree
t_itnp *bst_max(t_itnp *node);
// Find the successor of a node (in-order)
t_itnp *bst_successor(t_itnp *node);
// Find the predecessor of a node (in-order)
t_itnp *bst_predecessor(t_itnp *node);
// Check if a tree is valid (BST property)
int     bst_validate(t_itnp *root, tree_cmp_func cmp);

// --- Red-Black Tree Operations (t_rbn) ---
// Insert a node into an RB tree (fixes violations)
void rbn_insert(t_itn_rb *root, t_itn_rb *new_node, tree_cmp_func cmp);
// Remove a node from an RB tree (fixes violations)
void rbn_remove(t_itn_rb *root, t_itn_rb *node);
// Fix RB tree violations after insertion
void rbn_insert_fixup(t_itn_rb *root, t_itn_rb *node);
// Fix RB tree violations after removal
void rbn_remove_fixup(t_itn_rb *root, t_itnp *x, t_itnp *x_parent);
// Check if an RB tree is valid (BST + RB properties)
int  rbn_validate(t_itn_rb *root, tree_cmp_func cmp);

// --- AVL Tree Operations (t_avl) ---
// Insert a node into an AVL tree (fixes balance)
void       avl_insert(t_itn_avl *root, t_itn_avl *new_node, tree_cmp_func cmp);
// Remove a node from an AVL tree (fixes balance)
void       avl_remove(t_itn_avl *root, t_itn_avl *node);
// Update height of a node (helper for AVL)
void       avl_update_height(t_itn_avl *node);
// Balance a subtree (helper for AVL)
t_itn_avl *avl_balance(t_itn_avl *node);
// Check if an AVL tree is valid (BST + AVL properties)
int        avl_validate(t_itn_avl *root, tree_cmp_func cmp);

#endif // C_MINI__ITN_H
