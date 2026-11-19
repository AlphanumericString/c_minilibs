#ifndef C_MINI__MATHS
#define C_MINI__MATHS

#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) < (y) ? (x) : (y)

/**
 * \def CM_FORIT
 * \brief forward iterator for loop header without a body/scope
 * \param type type of the iterator
 * \param name name of the iterator variable
 * \param start first value of the iterator
 * \param end exclusive end value
 * \note use it directly with a block: CM_FORIT(int, i, 0, 4) { ... }
**/
#define CM_FORIT(type, name, start, end)                                       \
	for (type name = (start); name != (end); name++)

/**
 * \def CM_FORRIT
 * \brief reverse iterator for loop header without a body/scope
 * \param type type of the iterator
 * \param name name of the iterator variable
 * \param start first value of the iterator
 * \param end exclusive end value
 * \note use it directly with a block: CM_FORRIT(int, i, 4, 0) { ... }
**/
#define CM_FORRIT(type, name, start, end)                                      \
	for (type name = (start); name != (end); name--)

#endif // C_MINI__MATHS
