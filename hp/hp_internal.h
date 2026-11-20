#ifndef C_MINI__HP_HELPERS
#define C_MINI__HP_HELPERS

#include "heap.h"

/**
 * \internal
 * \brief get a pointer to the element at index idx of the heap storage
 * \param h heap pointer
 * \param idx index of the element
 * \return pointer to the element
 * \ingroup hp
**/
void *hpi_elem_ptr(const t_cm_hp *h, size_t idx);

/**
 * \internal
 * \brief swap two elements of the heap storage
 * \param h heap pointer
 * \param pos1 index of the first element
 * \param pos2 index of the second element
 * \ingroup hp
**/
void hpi_swap(const t_cm_hp *h, size_t pos1, size_t pos2);

/**
 * \internal
 * \brief sift an element up until the heap invariant is restored
 * \param h heap pointer
 * \param idx index of the element to sift
 * \param max true for a max heap, false for a min heap
 * \ingroup hp
**/
void hpi_sift_up(t_cm_hp *h, size_t idx, bool max);

/**
 * \internal
 * \brief sift an element down until the heap invariant is restored
 * \param h heap pointer
 * \param idx index of the element to sift
 * \param max true for a max heap, false for a min heap
 * \ingroup hp
**/
void hpi_sift_down(t_cm_hp *h, size_t idx, bool max);

/**
 * \internal
 * \brief rebuild the whole heap in place using Floyd's algorithm
 * \param h heap pointer
 * \param max true for a max heap, false for a min heap
 * \ingroup hp
**/
void hpi_heapify(t_cm_hp *h, bool max);

/**
 * \internal
 * \brief replace the root element and restore the heap invariant
 * \param h heap pointer
 * \param e element replacing the root
 * \param out buffer receiving the previous root
 * \param max true for a max heap, false for a min heap
 * \ingroup hp
**/
void hpi_replace_root(t_cm_hp *h, const void *e, void *out, bool max);

#endif // !C_MINI__HP_HELPERS
