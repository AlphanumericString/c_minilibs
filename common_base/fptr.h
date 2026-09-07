#ifndef C_MINI__COMMON_FPTR
#define C_MINI__COMMON_FPTR

#include <stdbool.h>
#include <stddef.h>

#ifndef C_MINI__T_CALLBACK
#	define C_MINI__T_CALLBACK

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
 **/
typedef bool (*t_callback)(void *elem, void *arg);

#endif // T_CALLBACK

#ifndef C_MINI__T_CALLBACK_IT
#	define C_MINI__T_CALLBACK_IT

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \param  pos  position of the elem in the sub array backing
 * \param  last boolean on whether or not this is the last element of the call
 * \return expected to return true on success false otherwise
 **/
typedef bool (*t_callback_it)(void *elem, void *arg, size_t pos, bool last);

#endif // C_MINI__T_CALLBACK_IT

#ifndef C_MINI__T_COMPARATOR
#	define C_MINI__T_COMPARATOR

/**
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
 **/
typedef int (*t_comparator)(const void *elem, const void *arg);

#endif // T_COMPARATOR

#ifndef C_MINI__T_DTOR
#	define C_MINI__T_DTOR

/**
 * \brief duplication function pointer for datas
 * \param elem pointer to the duplication target
 * \return pointer to a valid copy of the data
 **/
typedef void (*t_dtor)(void *elem);

#endif // !C_MINI__T_DTOR

#ifndef C_MINI__T_DUPLICATOR
#	define C_MINI__T_DUPLICATOR

/**
 * \brief duplication function pointer for datas
 * \param elem pointer to the duplication target
 * \return pointer to a valid copy of the data
 **/
typedef void *(*t_duplicator)(const void *elem);

#endif // !C_MINI__T_DUPLICATOR

#ifndef C_MINI__T_MERGER
#	define C_MINI__T_MERGER

typedef void *(*t_merger)(void *elem, void *arg);

#endif // !C_MINI__T_MERGER

#endif // !C_MINI__COMMON_FPTR
