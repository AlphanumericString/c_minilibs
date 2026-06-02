#include "ill.h"

/**
 * \brief Gets the minimum element in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the minimum.
 * \return A pointer to the minimum element, or NULL if the list is empty.
 */
void *ill_get_min(t_ill *head, t_comparator c) {
    t_ill *curr;
    t_ill *min;

    if (!head || !c) return NULL;
    curr = head;
    min = head;
    while (curr->next) {
        if (c(ill_ntoe(curr->next), ill_ntoe(min)) < 0)
            min = curr->next;
        curr = curr->next;
    }
    return ill_ntoe(min);
}

/**
 * \brief Gets the maximum element in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the maximum.
 * \return A pointer to the maximum element, or NULL if the list is empty.
 */
void *ill_get_max(t_ill *head, t_comparator c) {
    t_ill *curr;
    t_ill *max;

    if (!head || !c) return NULL;
    curr = head;
    max = head;
    while (curr->next) {
        if (c(ill_ntoe(curr->next), ill_ntoe(max)) > 0)
            max = curr->next;
        curr = curr->next;
    }
    return ill_ntoe(max);
}

/**
 * \brief Gets the k-th element in the list (0-indexed).
 * \param head The head of the list.
 * \param k The index of the element to retrieve.
 * \return A pointer to the k-th element, or NULL if the index is out of bounds.
 */
void *ill_get_kth(t_ill *head, size_t k) {
    t_ill *curr;   

    if (!head) return NULL;

    curr = head;
    while (curr->next && k > 0) {
        curr = curr->next;
        k--;
    }
    if (k > 0) return NULL;
	return ill_ntoe(curr);
}