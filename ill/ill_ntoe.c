#include "ill.h"

/**
 * \brief Gets the pointer to the element containing the node.
 * \param node The intrusive node.
 * \return A pointer to the user's element structure.
 */
void *ill_ntoe(const t_ill *node)
{
	return ((void *)(((char *)node) - node->offset));
}
/**
 * \brief Gets the intrusive node within an element structure.
 * \param e Pointer to the user's element structure.
 * \param offset The offset of the node within the structure.
 * \return A pointer to the intrusive node.
 */
t_ill *ill_eton(void *e, size_t offset) {
	return ((t_ill *)((char *)e + offset));
}
