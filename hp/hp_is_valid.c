#include "hp_internal.h"

/**
 * \brief check if a given heap is valid
 * \param hp pointer of the heap to check
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_is_valid(const t_cm_hp *hp) {
	return (hp && hp->elems && hp->cmp && hp->sz_e && hp->nb <= hp->capacity);
}
/**
 * \brief check if a given heap is empty on element count
 * \param hp pointer of the heap to check
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_is_empty(const t_cm_hp *hp) {
	return (hp && hp->nb == 0);
}
/**
 * \brief check if a given heap is full
 * \param hp pointer of the heap to check
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_is_full(const t_cm_hp *hp) {
	return (hp_is_valid(hp) && hp->nb == hp->capacity);
}

/**
 * \brief check if a given heap is a valid min heap (structure and order)
 * \param hp pointer of the heap to check
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_min_is_valid(const t_cm_hp *hp) {
	if (!hp_is_valid(hp)) return (false);
	for (size_t i = 1; i < hp->nb; i++)
		if (hp->cmp(hpi_elem_ptr(hp, (i - 1) / 2), hpi_elem_ptr(hp, i)) > 0)
			return (false);
	return (true);
}

/**
 * \brief check if a given heap is a valid max heap (structure and order)
 * \param hp pointer of the heap to check
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_max_is_valid(const t_cm_hp *hp) {
	if (!hp_is_valid(hp)) return (false);
	for (size_t i = 1; i < hp->nb; i++)
		if (hp->cmp(hpi_elem_ptr(hp, (i - 1) / 2), hpi_elem_ptr(hp, i)) < 0)
			return (false);
	return (true);
}
