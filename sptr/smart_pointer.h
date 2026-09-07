#ifndef C_MINI__SPTR
#define C_MINI__SPTR

#include <stdbool.h>
#include <stddef.h>

#ifdef C_MINI__TESTS

#	include <stdio.h>

#	define DBG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                        \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0);

#	define LOG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[32m[LOG]\e[0m: %s: " fmt "\n",                          \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0);

#	define DBG_POS                                                            \
		do {                                                                   \
			DBG(" %s:%d \e[33;4m[PATH]\e[0m", __FILE__, __LINE__);             \
			fflush(stdout);                                                    \
		} while (0);

#	define C_MINI__SPTR_LOG_SUCCESS   "Success"
#	define C_MINI__SPTR_LOG_EARG      "Bad arguments"
#	define C_MINI__SPTR_LOG_ILLUNIQ   C_MINI__SPTR_LOG_EARG " Illegal(uniq)"
#	define C_MINI__SPTR_LOG_ILLDATA   C_MINI__SPTR_LOG_EARG " Illegal(data)"
#	define C_MINI__SPTR_LOG_ILLSHARED C_MINI__SPTR_LOG_EARG " Illegal(shared)"
#	define C_MINI__SPTR_LOG_ETODO     "TODO"
#	define C_MINI__SPTR_LOG_EALLOC    "Allocation failure"

#else

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif

/**
 * \defgroup sptr Smart PoinTeR
 **/

/**
 * \brief enum for the strength of the smart pointer
 * \note starts at 1 to identify error/uninitialized via 0
 * \ingroup sptr
 **/
typedef enum { SPTR_W = 1, SPTR_S = 2 } e_ptrstr;

/**
 * \brief enum for the type of the smart pointer
 * \note starts at 1 to identify error/uninitialized via 0
 * \ingroup sptr
 **/
typedef enum { SPTR_SHRD = 1, SPTR_UNIQ = 2 } e_ptrtype;

/**
 * \brief internal structure for smart pointers
 * \param raw pointer value encapsulated in the smart pointer
 * \param closure function to call when free'ing the value
 * \param wc WeakCount number of elements with a view on the box
 * \param sc StrongCount number of elements "owning" the value
 * \param type whether the sptr is shared or uniq
 * \ingroup sptr
 **/
typedef struct s_box {
	void     *raw;
	void      (*dtor)(void *);
	size_t    wc;
	size_t    sc;
	e_ptrtype type;
} t_smartbox;

/**
 * \brief internal structure for smart pointers
 * \param smart_strength strength of the sptr itself
 * \param box address to the controling object box
 * \ingroup sptr
 **/
typedef struct {
	e_ptrstr    smart_strength;
	t_smartbox *box;
} t_sptr;

t_sptr sptr_make_shared_v(void *raw_ptr, void (*free)(void *));
t_sptr sptr_make_shared_fromsptr(t_sptr *smart_ptr, void (*free)(void *));

t_sptr sptr_make_uniq_v(void *raw_ptr, void (*free)(void *));
t_sptr sptr_make_uniq_fromsptr(t_sptr *smart_ptr, void (*free)(void *));

/**
 * \def sptr_make_shared(pointer, closure)
 * \brief _generic macro to re-direct to [..]shared_v and [..]shared_fromsptr
 * \param pointer either a t_sptr* or anything else
 * \param closure closure to call when free'ing pointer
 * \ingroup sptr
 **/
#define sptr_make_shared(pointer, closure)                                     \
	_Generic((pointer),                                                        \
	    t_sptr *: sptr_make_shared_fromsptr,                                   \
	    default: sptr_make_shared_v)(pointer, closure)
/**
 * \def sptr_make_uniq(pointer, closure)
 * \brief _generic macro to re-direct to [..]uniq_v and [..]uniq_fromsptr
 * \param pointer either a t_sptr* or anything else
 * \param closure closure to call when free'ing pointer
 * \ingroup sptr
 **/
#define sptr_make_uniq(pointer, closure)                                       \
	_Generic((pointer),                                                        \
	    t_sptr *: sptr_make_uniq_fromsptr,                                     \
	    default: sptr_make_uniq_v)(pointer, closure)

bool sptr_delete(t_sptr *sptr);

t_sptr sptr_copy(t_sptr *sptr);
t_sptr sptr_weakcopy(t_sptr *sptr);
t_sptr sptr_strongcopy(t_sptr *sptr);

void  *sptr_get(const t_sptr *sptr);
size_t sptr_get_wc(const t_sptr *sptr);
size_t sptr_get_sc(const t_sptr *sptr);
void   (*sptr_get_dtor(const t_sptr *s))(void *);

bool sptr_weaken(t_sptr *sptr);
bool sptr_strengthen(t_sptr *sptr);
bool sptr_steal(t_sptr *to, t_sptr *sptr);

void sptr_dump(int fd, const t_sptr *t);

#endif //! C_MINI__SPTR