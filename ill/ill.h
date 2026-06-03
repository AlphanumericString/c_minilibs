#ifndef PRIV_ILL

#	include <stdbool.h>
#	include <stddef.h>

/**
 * \brief t_ill struct
 * \param offset offset of the node in the element
 * \param next pointer to the next node
**/
typedef struct s_ill {
	size_t        offset;
	struct s_ill *next;
} t_ill;

// callback -> true operation successfull, elem is always the node, arg is passed as is from up function
typedef bool  (*t_callback)(void *elem, void *arg);
// comparator -> <0, ==0, >0 for less than, equal, superior to.
typedef int   (*t_comparator)(void *elem, void *arg);
// duplicator -> deep copy function.
typedef void *(*t_duplicator)(const void *elem);

// Initialize node (not in any list)
void  ill_init(t_ill *node, size_t offset);
void *ill_copy(const t_ill *head, t_duplicator d);

// NODE->ELEM and ELEM->NODE
void  *ill_ntoe(const t_ill *node);
t_ill *ill_eton(void *e, size_t offset);

// Checks
bool ill_is_isolated(t_ill *node);
bool ill_is_circular(t_ill *head);
// auto detects if reverse sorted
bool ill_is_sorted(t_ill *head, t_comparator comparator);
bool ill_has(t_ill *head, t_callback matcher, void *arg);

// Insert 'node' before 'anchor' (anchor does move)
void ill_prepend(t_ill **head, t_ill *anchor, t_ill *node);	// !
void ill_nprepend(t_ill **head, t_ill *anchor, t_ill *nodes);	// !
// Insert 'node' after 'anchor' (anchor does not move)
void ill_append(t_ill *anchor, t_ill *node);
void ill_nappend(t_ill *anchor, t_ill *nodes);

// Insert at front of list (updates head pointer)
t_ill *ill_npush(t_ill **head, t_ill *nodes);
t_ill *ill_push(t_ill **head, t_ill *node);
t_ill *ill_push_back(t_ill **head, t_ill *node);

// Remove head/tail
t_ill *ill_pop(t_ill **head);
t_ill *ill_pop_back(t_ill **head);

// Remove node
t_ill *ill_remove_after(t_ill *prev);
bool   ill_remove_node(t_ill **head, t_ill *target);

// Find node
t_ill *ill_find_prev(t_ill *head, t_ill *target);
t_ill *ill_find(t_ill *head, t_ill *target);      // !
// Get elem
t_ill *ill_get_prev(t_ill *head, t_comparator c, void *arg);
t_ill *ill_get(t_ill *head, t_comparator c, void *arg);

// Get tail (last node)
t_ill *ill_get_tail(t_ill *head);

// Get min/max
void  *ill_get_min(t_ill *head, t_comparator comp);
void  *ill_get_max(t_ill *head, t_comparator comp);
void *ill_get_kth(t_ill *head, size_t k);

// Get length
size_t ill_length(t_ill *head);
size_t ill_nlength(t_ill *head, size_t n);
size_t ill_match_nb(t_ill *head, t_callback matcher, void *arg);

// Forward iteration macro
bool  ill_for(t_ill *head, size_t n, t_callback c, void *u_data);
bool  ill_foreach(t_ill *head, t_callback c, void *u_data);
// reverse
t_ill *ill_reverse(t_ill *head);

// move
// if !end_cut cut to the end of list included otherwise stop before end_cut
void ill_move(t_ill *dest_anchor, t_ill *start_cut, t_ill *end_cut);	// !
//- Merges two sorted lists into one.
void ill_sort(t_ill **head, t_comparator comp);							// !
t_ill *ill_sorted_merge(t_ill *list1, t_ill *list2, t_comparator comp);	// !

// filter
// list for tru is returned list of false has sotred in filtered
t_ill	*ill_filter(t_ill *head, t_callback cb, void *arg, t_ill **filtered);
// remove if aka filter in place. if !free_func no free.
bool	ill_remove_if(t_ill **head, t_callback condition_cb, void *arg, void (*free_func)(void *elem));
// (requirement: list must be sorted)
void	ill_unique(t_ill *head, t_comparator c); // !

/*
 * TODO: Missing classic intrusive list functions to implement
 *
 * implement functions with '!' present
 *
 * Plan to tackle:
 * - Create respective .c files for each functional block (e.g. ill_sort.c, ill_splice.c).
 * - Write comprehensive tests for them in test_main_ill.c.
 */

#	define PRIV_ILL
#endif // !PRIV_ILL
