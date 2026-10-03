#include "dynamic_array.h"

/**
 * \brief check if a given da is valid
 * \param da pointer of dynamic array to check
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_is_valid(const t_da *da) {
	return (da && da->array && da->sz_e && da->nb_e <= da->cap_e);
}
/**
 * \brief check if a given da is valid not zeroed out
 * \param da pointer of dynamic array to check
 * \return true on success, false otherwise if empty on any field
 * \note return false if ANY field is zeroed out
 * \ingroup da
**/
bool da_is_not_null(const t_da *da) {
	return (da && da->array && da->cap_e && da->sz_e);
}
/**
 * \brief check if a given da is empty on element count
 * \param da pointer of dynamic array to check
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_is_empty(const t_da *da) {
	return (da && da->nb_e == 0);
}
/**
 * \brief check if a given da is full
 * \param da pointer of dynamic array to check
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_is_full(const t_da *da) {
	return (da_is_valid(da) && da->nb_e == da->cap_e);
}
