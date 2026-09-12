#ifndef C_MINI__SPTR
#define C_MINI__SPTR

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

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
#	define C_MINI__SPTR_LOG_ILL       "Illegal"
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
 * \defgroup sptr Smart pointers
 **/

/**
 * \brief generic reference counted box with dtor, value and count
 * \param data data to wrapp arround
 * \param dtor destructor to call uppon destruction
 * \param rc reference count of the object (calls dtor on data when 0)
 * \ingroup sptr
 **/
typedef struct {
	void  *data;
	void   (*dtor)(void *);
	size_t rc;
} t_rcbox;

/**
 * \brief generic refcounted box with weak_count (c++ style implementation)
 * \param rcb standard ref-counted box (rc re-used for strong count)
 * \param weak_count the weak count to know when to delete the weak box
 * \note pointers from wcbox can be converted to rcbox with only truncation of
 *  information. this means that converting between wcbox and rcbox is trivial
 *  if you can assertain for sure that it was allocated as wcbox.
 * \ingroup sptr
 **/
typedef struct {
	t_rcbox rcb;
	size_t  weak_count;
} t_wcbox;

/**
 * \brief generic reference counted box with ownership (rust style)
 * \param wcbox wrapper for the box (for shared infos) (rc fiexed at 1)
 * \param owner ownership for rust-style ergonomics
 * \ingroup sptr
 **/
typedef struct s_rsptr {
	t_wcbox *wcbox;
	bool     owner;
} t_rsptr;

/**
 * \brief typedef for expicit name not generic ref-count box
 * \ingroup sptr
 **/
typedef t_rcbox t_strong_ptr;

/**
 * \brief typedef for expicit name not generic ref-count box
 * \ingroup sptr
 **/
typedef t_wcbox t_weak_ptr;

/**
 * \brief typedef for expicit name not generic ref-count box
 * \ingroup sptr
 **/
typedef t_rsptr t_uniq_ptr;

typedef void (*t_dtor)(void *v);

t_strong_ptr *sptr_make_shared_from_raw(void *raw_ptr, t_dtor d);
t_weak_ptr   *sptr_make_shared_from_weak(t_weak_ptr *weak_ptr, t_dtor d);
t_weak_ptr   *sptr_make_shared_from_strong(t_strong_ptr *strong_ptr, t_dtor d);
t_strong_ptr *sptr_make_shared_from_uniq(t_uniq_ptr *uniq, t_dtor d);
/**
 * \def sptr_make_shared(pointer, closure)
 * \brief _generic macro to re-direct to sptr_make_shared*
 * \param pointer either a sptr* or anything else
 * \param dtor closure to call when free'ing pointer
 * \note ALWAYS return a weak pointer unless it create from raw or uniq. If you
 * want to duplicate a pointer witht the same strength use _copy functions
 * \ingroup sptr
 **/
#define sptr_make_shared(pointer, dtor)                                        \
	_Generic((pointer),                                                        \
	    t_uniq_ptr *: sptr_make_shared_from_uniq,                              \
	    t_weak_ptr *: sptr_make_shared_from_weak,                              \
	    t_strong_ptr *: sptr_make_shared_from_strong,                          \
	    default: sptr_make_shared_from_raw)(pointer, dtor)

t_uniq_ptr sptr_make_uniq_from_raw(void *raw_ptr, t_dtor d);
// t_uniq_ptr sptr_make_uniq_from_weak(t_weak_ptr * weak_ptr, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_strong(t_strong_ptr *strong_ptr, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_uniq(t_uniq_ptr uniq, t_dtor d);
/**
 * \def sptr_make_uniq(pointer, closure)
 * \brief _generic macro to re-direct to sptr_make_uniq*
 * \param pointer either a sptr* or anything else
 * \param dtor closure to call when free'ing pointer
 * \ingroup sptr
 **/
#define sptr_make_uniq(pointer, dtor)                                          \
	_Generic((pointer),                                                        \
	    t_uniq_ptr: sptr_make_uniq_from_uniq,                                  \
	    t_strong_ptr *: sptr_make_uniq_from_strong,                            \
	    default: sptr_make_uniq_from_raw)(pointer, dtor)
// t_weak_ptr *: sptr_make_uniq_from_weak,

bool sptr_is_uniqs(const t_strong_ptr *strong_ptr);
bool sptr_is_uniqw(const t_weak_ptr *weak_ptr);
bool sptr_is_uniqu(const t_uniq_ptr uniq_ptr);

/**
 * \def sptr_is_uniq(pointer)
 * \brief _generic macro to re-direct to sptr_is_uniq*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_is_uniq(pointer)                                                  \
	_Generic((pointer),                                                        \
	    const t_strong_ptr *: sptr_is_uniqs,                                   \
	    const t_weak_ptr *: sptr_is_uniqw)(pointer)

bool sptr_is_shareds(const t_strong_ptr *smart_ptr);
bool sptr_is_sharedw(const t_weak_ptr *weak_ptr);
bool sptr_is_sharedu(const t_uniq_ptr uniq_ptr);

/**
 * \def sptr_is_shared(pointer)
 * \brief _generic macro to re-direct to sptr_is_shared*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_is_shared(pointer)                                                \
	_Generic((pointer),                                                        \
	    const t_uniq_ptr: sptr_is_sharedu,                                     \
	    const t_strong_ptr *: sptr_is_shareds,                                 \
	    const t_weak_ptr *: sptr_is_sharedw)(pointer)

bool sptr_is_owns(const t_strong_ptr *ptr);
bool sptr_is_ownw(const t_weak_ptr *ptr);
bool sptr_is_ownu(const t_uniq_ptr ptr);

/**
 * \def sptr_is_own(pointer)
 * \brief _generic macro to re-direct to sptr_is_own*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_is_own(pointer)                                                   \
	_Generic((pointer),                                                        \
	    const t_uniq_ptr: sptr_is_ownu,                                        \
	    const t_strong_ptr *: sptr_is_owns,                                    \
	    const t_weak_ptr *: sptr_is_ownw)(pointer)

bool sptr_deletes(t_strong_ptr *strong_ptr);
bool sptr_deletew(t_weak_ptr *weak_ptr);
bool sptr_deleteu(t_uniq_ptr uniq_ptr);

/**
 * \def sptr_delete(pointer)
 * \brief _generic macro to re-direct to sptr_delete*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_delete(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_uniq_ptr: sptr_deleteu,                                              \
	    t_strong_ptr *: sptr_deletes,                                          \
	    t_weak_ptr *: sptr_deletew)(pointer)

// const and non-const variants for extra strong typechecking in projects
const void *sptr_getsc(const t_strong_ptr *sptr);
const void *sptr_getwc(const t_weak_ptr *sptr);

void *sptr_gets(t_strong_ptr *sptr);
void *sptr_getw(t_weak_ptr *sptr);
void *sptr_getu(t_uniq_ptr sptr);

/**
 * \def sptr_get(pointer)
 * \brief _generic macro to re-direct to sptr_get*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_get(pointer)                                                      \
	_Generic((pointer),                                                        \
	    const t_strong_ptr *: sptr_getsc,                                      \
	    const t_weak_ptr *: sptr_getwc,                                        \
	    t_uniq_ptr: sptr_getu,                                                 \
	    t_strong_ptr *: sptr_gets,                                             \
	    t_weak_ptr *: sptr_getw)(pointer)

size_t sptr_get_strongcount_s(const t_strong_ptr *ptr);
size_t sptr_get_strongcount_w(const t_weak_ptr *ptr);
size_t sptr_get_ownercount_u(const t_uniq_ptr ptr);

/**
 * \def sptr_get_rc(pointer)
 * \brief _generic macro to re-direct to sptr_get_rc*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_get_rc(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_uniq_ptr: sptr_get_ownercount_u,                                     \
	    t_strong_ptr *: sptr_get_strongcount_s,                                \
	    const t_strong_ptr *: sptr_get_strongcount_s,                          \
	    t_weak_ptr *: sptr_get_strongcount_w,                                  \
	    const t_weak_ptr *: sptr_get_strongcount_w)(pointer)

size_t sptr_get_weakcount_s(const t_strong_ptr *ptr);
size_t sptr_get_weakcount_w(const t_weak_ptr *ptr);
size_t sptr_get_weakcount_u(const t_uniq_ptr ptr);

/**
 * \def sptr_get_wc(pointer)
 * \brief _generic macro to re-direct to sptr_get_wc*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_get_wc(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_uniq_ptr: sptr_get_weakcount_u,                                      \
	    t_strong_ptr *: sptr_get_weakcount_s,                                  \
	    const t_strong_ptr *: sptr_get_weakcount_s,                            \
	    t_weak_ptr *: sptr_get_weakcount_w,                                    \
	    const t_weak_ptr *: sptr_get_weakcount_w)(pointer)

t_dtor sptr_get_dtors(const t_strong_ptr *ptr);
t_dtor sptr_get_dtorw(const t_weak_ptr *ptr);
t_dtor sptr_get_dtoru(const t_uniq_ptr ptr);

/**
 * \def sptr_get_dtor(pointer)
 * \brief _generic macro to re-direct to sptr_get_dtor*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_get_dtor(pointer)                                                 \
	_Generic((pointer),                                                        \
	    const t_strong_ptr *: sptr_get_dtors,                                  \
	    const t_weak_ptr *: sptr_get_dtorw,                                    \
	    t_uniq_ptr: sptr_get_dtoru,                                            \
	    t_strong_ptr *: sptr_get_dtors,                                        \
	    t_weak_ptr *: sptr_get_dtorw)(pointer)

bool sptr_set_dtors(t_strong_ptr *ptr, t_dtor dtor);
bool sptr_set_dtorw(t_weak_ptr *ptr, t_dtor dtor);
bool sptr_set_dtoru(t_uniq_ptr ptr, t_dtor dtor);

/**
 * \def sptr_set_dtor(pointer)
 * \brief _generic macro to re-direct to sptr_set_dtor*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_set_dtor(pointer)                                                 \
	_Generic((pointer),                                                        \
	    const t_strong_ptr *: sptr_set_dtors,                                  \
	    const t_weak_ptr *: sptr_set_dtorw,                                    \
	    t_uniq_ptr: sptr_set_dtoru,                                            \
	    t_strong_ptr *: sptr_set_dtors,                                        \
	    t_weak_ptr *: sptr_set_dtorw)(pointer)

t_strong_ptr *sptr_copys(t_strong_ptr *sptr);
t_weak_ptr   *sptr_copyw(t_weak_ptr *weak_ptr);
t_uniq_ptr    sptr_copyu(t_uniq_ptr uniq_ptr);

/**
 * \def sptr_copy(pointer)
 * \brief _generic macro to re-direct to sptr_copy*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_copy(pointer)                                                     \
	_Generic((pointer),                                                        \
	    t_strong_ptr *: sptr_copys,                                            \
	    t_weak_ptr *: sptr_copyw,                                              \
	    t_uniq_ptr: sptr_copyu)(pointer)

void sptr_dumps(int fd, const t_strong_ptr *ptr);
void sptr_dumpw(int fd, const t_weak_ptr *ptr);
void sptr_dumpu(int fd, const t_uniq_ptr ptr);

/**
 * \def sptr_dump(pointer)
 * \brief _generic macro to re-direct to sptr_dump*
 * \param pointer either a sptr* or anything else
 * \ingroup sptr
 **/
#define sptr_dump(pointer)                                                     \
	_Generic((pointer),                                                        \
	    const t_strong_ptr *: sptr_dumps,                                      \
	    const t_weak_ptr *: sptr_dumpw,                                        \
	    const t_uniq_ptr: sptr_dumpu)(pointer)

// -- non generic functions

t_strong_ptr *sptr_strengthen(t_weak_ptr *sptr);
t_weak_ptr   *sptr_weaken(t_strong_ptr *sptr);
bool          sptr_steal(t_uniq_ptr *to, t_uniq_ptr *from);

#endif //! C_MINI__SPTR
