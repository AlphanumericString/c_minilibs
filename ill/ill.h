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
typedef bool  (*t_callback)(void *elem, void *arg);		// ok
// comparator -> <0, ==0, >0 for less than, equal, superior to.
typedef int   (*t_comparator)(void *elem, void *arg);	// ok
// duplicator -> deep copy function.
typedef void *(*t_duplicator)(const void *elem);		// ok

// Initialize node (not in any list)
void  ill_init(t_ill *node, size_t offset);        // ok
void *ill_copy(const t_ill *head, t_duplicator d); // ok

// NODE->ELEM and ELEM->NODE
void  *ill_ntoe(const t_ill *node);      // ok
t_ill *ill_eton(void *e, size_t offset); // ok

// Checks
bool ill_is_isolated(t_ill *node); // ok
bool ill_is_circular(t_ill *head); // ok
// auto detects if reverse sorted
bool ill_is_sorted(t_ill *head, t_comparator comparator); // ok
bool ill_has(t_ill *head, t_callback matcher, void *arg); // ok

// Insert 'node' after 'anchor' (anchor does not move)
void ill_append(t_ill *anchor, t_ill *node);   // ok
void ill_nappend(t_ill *anchor, t_ill *nodes); // ok

// Insert at front of list (updates head pointer)
t_ill *ill_npush(t_ill **head, t_ill *nodes);    // ~ok (any use case?)
t_ill *ill_push(t_ill **head, t_ill *node);      // ok
t_ill *ill_push_back(t_ill **head, t_ill *node); // ok

// Remove head/tail
t_ill *ill_pop(t_ill **head);      // ok
t_ill *ill_pop_back(t_ill **head); // ok

// Remove node
t_ill *ill_remove_after(t_ill *prev);                // ok
bool   ill_remove_node(t_ill **head, t_ill *target); // ok

// Find node
t_ill *ill_find_prev(t_ill *head, t_ill *target); // ok
t_ill *ill_find(t_ill *head, t_ill *target);      // !
// Get elem
t_ill *ill_get_prev(t_ill *head, t_comparator c, void *arg); // ok
t_ill *ill_get(t_ill *head, t_comparator c, void *arg);      // ok

// Get tail (last node)
t_ill *ill_get_tail(t_ill *head); // ok

// Get min/max
void  *ill_get_min(t_ill *head, t_comparator comp); // ok
void  *ill_get_max(t_ill *head, t_comparator comp); // ok
void *ill_get_kth(t_ill *head, size_t k);          // ok

// Get length
size_t ill_length(t_ill *head);                                  // ok
size_t ill_nlength(t_ill *head, size_t n);                       // ok
size_t ill_match_nb(t_ill *head, t_callback matcher, void *arg); // ok

// Forward iteration macro
bool  ill_for(t_ill *head, size_t n, t_callback c, void *u_data); // ok
bool  ill_foreach(t_ill *head, t_callback c, void *u_data);       // ok
// reverse
t_ill *ill_reverse(t_ill *head);                                   // ok

/*
 * TODO: Missing classic intrusive list functions to implement
 * 
 * 1. void ill_insert_before(t_ill **head, t_ill *anchor, t_ill *node);
 *    - To insert before a specific node.
 * 2. void ill_splice(t_ill *anchor, t_ill *start, t_ill *end);
 *    - To move a sublist from one list into another after 'anchor'.
 * 3. void ill_sort(t_ill **head, t_comparator comp);
 *    - Merge sort for the list.
 * 4. t_ill *ill_merge(t_ill *list1, t_ill *list2, t_comparator comp);
 *    - Merges two sorted lists into one.
 * 5. void ill_clear(t_ill **head, t_callback free_cb, void *arg);
 *    - Utility to iterate and free all elements, then set head to NULL.
 * 6. t_ill *ill_filter(t_ill *head, t_callback filter_cb, void *arg);
 *    - Creates a new list with elements that match the filter condition.
 * 7. bool ill_remove_if(t_ill **head, t_callback condition_cb, void *arg);
 *    - Removes all elements that match the condition.
 * 8. void ill_unique(t_ill *head, t_comparator comp);
 *    - Removes consecutive duplicates in a sorted list.
 *
 * Plan to tackle:
 * - Define these missing prototypes in a separate header or add them above.
 * - Create respective .c files for each functional block (e.g. ill_sort.c, ill_splice.c).
 * - Write comprehensive tests for them in test_main_ill.c.
 * - Add new .c files to the Makefile.
 */

#	define PRIV_ILL
#endif // !PRIV_ILL