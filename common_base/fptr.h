#ifndef C_MINI__COMMON_FPTR
#define C_MINI__COMMON_FPTR

#include <stdbool.h>
#include <stddef.h>

#ifndef C_MINI__T_CALLBACK
#	define C_MINI__T_CALLBACK

/**
 * \typedef t_cm_callback
 * \brief  callback function pointer, true when the operation is successful,
 * elem is always the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success, false otherwise
**/
typedef bool (*t_cm_callback)(void *elem, void *arg);

#endif // T_CALLBACK

#ifndef C_MINI__T_CALLBACK_IT
#	define C_MINI__T_CALLBACK_IT

/**
 * \typedef t_cm_callback_it
 * \brief  callback function pointer, true when the operation is successful,
 * elem is always the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \param  pos  position of the elem in the sub array backing
 * \param  last boolean on whether or not this is the last element of the call
 * \return expected to return true on success, false otherwise
**/
typedef bool (*t_cm_callback_it)(void *elem, void *arg, size_t pos, bool last);

#endif // C_MINI__T_CALLBACK_IT

#ifndef C_MINI__T_COMPARATOR
#	define C_MINI__T_COMPARATOR

/**
 * \typedef t_cm_comparator
 * \brief comparator function pointer, <0, 0, >0 for inferior to, equal,
 * superior to
 * \param elem pointer to the element
 * \param arg argument passed as is from up function
 * \return expected to return <0, 0, >0 for inferior to, equal, superior to
**/
typedef int (*t_cm_comparator)(const void *elem, const void *arg);

#endif // T_COMPARATOR

#ifndef C_MINI__T_DTOR
#	define C_MINI__T_DTOR

/**
 * \typedef t_cm_dtor
 * \brief dtor, destructor function to be attached to a data/ressource
 * \param elem pointer to the cleanup target
**/
typedef void (*t_cm_dtor)(void *elem);

#endif // !C_MINI__T_DTOR

#ifndef C_MINI__T_DUPLICATOR
#	define C_MINI__T_DUPLICATOR

/**
 * \typedef t_cm_duplicator
 * \brief duplication function pointer for data
 * \param elem pointer to the duplication target
 * \return pointer to a valid copy of the data
**/
typedef void *(*t_cm_duplicator)(const void *elem);

#endif // !C_MINI__T_DUPLICATOR

#ifndef C_MINI__T_MERGER
#	define C_MINI__T_MERGER

/**
 * \typedef t_cm_merger
 * \brief merger function pointer for data merging and other merge tasks
 * \param elem pointer to the accumulator
 * \param arg pointer to the value in the struct
 * \return pointer to a valid copy of the data
**/
typedef void *(*t_cm_merger)(void *elem, void *arg);

#endif // !C_MINI__T_MERGER

#ifndef C_MINI__T_CALL
#	define C_MINI__T_CALL

/**
 * \typedef t_cm_call
 * \brief function pointer for non-returning functions executing and not
 * yielding any values
 * \param arg pointer to the value given to the call
 * \note made for thread pool manager
**/
typedef void (*t_cm_call)(void *arg);

#endif // !C_MINI__T_CALL

#ifndef C_MINI__T_WORK
#	define C_MINI__T_WORK

/**
 * \typedef t_cm_work
 * \brief function pointer for functions yielding a value as a return.
 * \param arg pointer to the value given to the work function
 * \return the result of the work done on arg
 * \note made for thread pool manager
**/
typedef void *(*t_cm_work)(void *arg);

#endif // !C_MINI__T_WORK

#endif // !C_MINI__COMMON_FPTR
