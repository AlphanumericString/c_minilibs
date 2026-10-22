#ifndef C_MINI__SPTR
#define C_MINI__SPTR

#include <assert.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

#define C_MINI__SPTR_ATOMIC
#define C_MINI__LOG_THREADSAFE

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__SPTR_CONSOLE_LOCK // make_shared

#	include <pthread.h>
pthread_mutex_t console_lock;

#endif

#ifdef C_MINI__TESTS

#	define C_MINI__SPTR_LOG_SUCCESS   C_MINI__LOG_SUCCESS
#	define C_MINI__SPTR_LOG_EARG      C_MINI__LOG_F_PREFIX "Bad arguments"
#	define C_MINI__SPTR_LOG_ILL       C_MINI__SPTR_LOG_EARG ": Illegal."
#	define C_MINI__SPTR_LOG_ILLUNIQ   C_MINI__SPTR_LOG_EARG ": Illegal(uniq)"
#	define C_MINI__SPTR_LOG_ILLDATA   C_MINI__SPTR_LOG_EARG ": Illegal(data)"
#	define C_MINI__SPTR_LOG_ILLSHARED C_MINI__SPTR_LOG_EARG ": Illegal(shared)"
#	define C_MINI__SPTR_LOG_ILLDTOR   C_MINI__SPTR_LOG_EARG ": Illegal(dtor)"
#	define C_MINI__SPTR_LOG_ETODO     C_MINI__LOG_ETODO
#	define C_MINI__SPTR_LOG_EALLOC    C_MINI__LOG_EALLOC
#endif

#define C_MINI__SPTR_DEFAULT_VAL {0}

/**
 * \defgroup sptr Smart pointers
**/

/**
 * \brief generic reference counted box with full capabilities
 * \param data pointer to the wrapped address
 * \param dtor pointer to the function to call when strong count equal 0
 * \param rc strong count (when 0 dtor is called on data)
 * \param wc weak count (when weak count 0 AND strong count 0 box can be safely
 * free'd)
 * \ingroup sptr
**/
typedef struct {
	void  *data;
	t_dtor dtor;
#ifdef C_MINI__SPTR_ATOMIC
	atomic_size_t rc;
	atomic_size_t wc;
#else
	size_t rc;
	size_t wc;
#endif
} t_fbox;

/**
 * \brief generic box for pointer handling, coupling dtor and data
 * \param data pointer to the address wrapped by the box
 * \param dtor pointer to the dtor to call when data needs to be destroyed
 * \ingroup sptr
**/
typedef struct {
	void  *data;
	t_dtor dtor;
} t_box;

/**
 * \brief strong shared smart pointer
 * \param data direct access to the data pointer
 * \param meta pointer to the shared metadata box
 * \ingroup sptr
**/
typedef struct {
	void   *data;
	t_fbox *meta;
} t_strong_ptr;

/**
 * \brief owned shared smart pointer
 * \param data direct access to the data address
 * \param meta pointer to the shared metadata box
 * \param owner ownership boolean
 * \ingroup sptr
**/
typedef struct s_rsptr {
	void   *data;
	t_fbox *meta;
	bool    owner;
} t_owned_ptr;

/**
 * \brief shared weak smart pointer
 * \param meta pointer to the shared metadata box
 * \note left as struct instead of typedef for future proofing
 * \ingroup sptr
**/
typedef struct {
	t_fbox *meta;
} t_weak_ptr;

/**
 * \brief re-define of t_box to dedicated t_uniq_ptr for clarity
 * \ingroup sptr
**/
typedef t_box t_uniq_ptr;

t_strong_ptr sptr_make_shared_from_raw(void *raw_ptr, t_dtor d);
t_strong_ptr sptr_make_shared_from_weak(t_weak_ptr *weak_ptr, t_dtor d);
t_strong_ptr sptr_make_shared_from_strong(t_strong_ptr *strong_ptr, t_dtor d);
t_strong_ptr sptr_make_shared_from_owned(t_owned_ptr *owned, t_dtor d);
t_strong_ptr sptr_make_shared_from_uniq(t_uniq_ptr *uniq_ptr, t_dtor d);
/**
 * \def sptr_make_shared(pointer, dtor)
 * \brief _generic macro to re-direct to sptr_make_shared*
 * \param pointer pointer to wrap (raw, weak, strong, owned, uniq)
 * \param dtor destructor callback to call when freeing pointer
 * \return created or updated smart pointer, or NULL on failure
 * \note for specific infos check doc for sptr_make_shared_from_<type>
 * \ingroup sptr
**/
#define sptr_make_shared(pointer, dtor)                                        \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_make_shared_from_weak,                              \
	    t_strong_ptr *: sptr_make_shared_from_strong,                          \
	    t_owned_ptr *: sptr_make_shared_from_owned,                            \
	    t_uniq_ptr *: sptr_make_shared_from_uniq,                              \
	    default: sptr_make_shared_from_raw)(pointer, dtor)

t_weak_ptr sptr_make_weak_from_weak(t_weak_ptr *w);
t_weak_ptr sptr_make_weak_from_strong(t_strong_ptr *s);
/**
 * \def sptr_make_weak(pointer)
 * \brief _generic macro to re-direct to sptr_make_weak_from_*
 * \param pointer pointer to make a weak shared pointer from any shared pointer
 * \return weak pointer struct
 * \note for specific infos check doc for sptr_make_weak_from_<weak|strong>
 * \ingroup sptr
**/
#define sptr_make_weak(pointer)                                                \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_make_weak_from_weak,                                \
	    t_strong_ptr *: sptr_make_weak_from_strong)(pointer)

t_owned_ptr sptr_make_owned_from_raw(void *raw_ptr, t_dtor d);
t_owned_ptr sptr_make_owned_from_strong(t_strong_ptr *strong_ptr, t_dtor d);
t_owned_ptr sptr_make_owned_from_owned(t_owned_ptr *owned, t_dtor d);
t_owned_ptr sptr_make_owned_from_uniq(t_uniq_ptr *uniq_ptr, t_dtor d);
/**
 * \def sptr_make_owned(pointer, dtor)
 * \brief _generic macro to re-direct to sptr_make_owned*
 * \param pointer pointer to wrap (raw pointer, strong ptr, or owned ptr)
 * \param dtor destructor callback to call when freeing pointer
 * \return owned pointer struct (t_owned_ptr)
 * \note for specific infos check doc for sptr_make_owned_from_<type>
 * \ingroup sptr
**/
#define sptr_make_owned(pointer, dtor)                                         \
	_Generic((pointer),                                                        \
	    t_strong_ptr *: sptr_make_owned_from_strong,                           \
	    t_owned_ptr *: sptr_make_owned_from_owned,                             \
	    t_uniq_ptr *: sptr_make_owned_from_uniq,                               \
	    default: sptr_make_owned_from_raw)(pointer, dtor)

t_uniq_ptr sptr_make_uniq_from_raw(void *raw, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_weak(t_weak_ptr *wptr, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_strong(t_strong_ptr *sptr, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_owned(t_owned_ptr *optr, t_dtor d);
t_uniq_ptr sptr_make_uniq_from_uniq(t_uniq_ptr *uptr, t_dtor d);
/**
 * \def sptr_make_uniq(pointer, dtor)
 * \brief _generic macro to re-direct to sptr_make_uniq*
 * \param pointer pointer to wrap (raw pointer, strong ptr, or uniq ptr)
 * \param dtor destructor callback to call when freeing pointer
 * \return unique pointer struct (t_owned_ptr)
 * \note for specific infos check doc for sptr_make_uniq_from_<type>
 * \ingroup sptr
**/
#define sptr_make_uniq(pointer, dtor)                                          \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_make_uniq_from_weak,                                \
	    t_strong_ptr *: sptr_make_uniq_from_strong,                            \
	    t_owned_ptr *: sptr_make_uniq_from_owned,                              \
	    t_uniq_ptr *: sptr_make_uniq_from_uniq,                                \
	    default: sptr_make_uniq_from_raw)(pointer, dtor)

bool sptr_is_uniqs(const t_strong_ptr *strong_ptr);
bool sptr_is_uniqw(const t_weak_ptr *weak_ptr);
bool sptr_is_uniqo(const t_owned_ptr *owned_ptr);
bool sptr_is_uniqu(const t_uniq_ptr *u);
/**
 * \def sptr_is_uniq(pointer)
 * \brief _generic macro to re-direct to sptr_is_uniq*
 * \param pointer smart pointer to check (weak, strong, owned, uniq)
 * \return true if the smart pointer has a strong count of 1, false otherwise
 * \note for specific infos check doc for sptr_is_uniq<type's letter>
 * \ingroup sptr
**/
#define sptr_is_uniq(pointer)                                                  \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_is_uniqw,                                           \
	    t_strong_ptr *: sptr_is_uniqs,                                         \
	    t_uniq_ptr *: sptr_is_uniqu,                                           \
	    t_owned_ptr *: sptr_is_uniqo,                                          \
	    const t_weak_ptr *: sptr_is_uniqw,                                     \
	    const t_strong_ptr *: sptr_is_uniqs,                                   \
	    const t_uniq_ptr *: sptr_is_uniqu,                                     \
	    const t_owned_ptr *: sptr_is_uniqo)(pointer)

bool sptr_is_shareds(const t_strong_ptr *smart_ptr);
bool sptr_is_sharedw(const t_weak_ptr *weak_ptr);
bool sptr_is_sharedo(const t_owned_ptr *owned_ptr);
bool sptr_is_sharedu(const t_uniq_ptr *u);
/**
 * \def sptr_is_shared(pointer)
 * \brief _generic macro to re-direct to sptr_is_shared*
 * \param pointer smart pointer to check (weak, strong, owned, uniq)
 * \return true if the smart pointer has a strong count greater than 1, false
 * otherwise
 * \note for specific infos check doc for sptr_is_shared<type's letter>
 * \ingroup sptr
**/
#define sptr_is_shared(pointer)                                                \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_is_sharedw,                                         \
	    t_strong_ptr *: sptr_is_shareds,                                       \
	    t_uniq_ptr *: sptr_is_sharedu,                                         \
	    t_owned_ptr *: sptr_is_sharedo,                                        \
	    const t_weak_ptr *: sptr_is_sharedw,                                   \
	    const t_strong_ptr *: sptr_is_shareds,                                 \
	    const t_uniq_ptr *: sptr_is_sharedu,                                   \
	    const t_owned_ptr *: sptr_is_sharedo)(pointer)

bool sptr_deletes(t_strong_ptr *strong_ptr);
bool sptr_deletew(t_weak_ptr *weak_ptr);
bool sptr_deleteo(t_owned_ptr *owned_ptr);
bool sptr_deleteu(t_uniq_ptr *u);
/**
 * \def sptr_delete(pointer)
 * \brief _generic macro to re-direct to sptr_delete*
 * \param pointer smart pointer to decrement or delete (weak, strong, owned,
 * uniq)
 * \return true if successfully decremented or deleted, false otherwise
 * \note for specific infos check doc for sptr_delete<type's letter>
 * \ingroup sptr
**/
#define sptr_delete(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_deletew,                                            \
	    t_strong_ptr *: sptr_deletes,                                          \
	    t_uniq_ptr *: sptr_deleteu,                                            \
	    t_owned_ptr *: sptr_deleteo)(pointer)

// Const get
const void *sptr_getsc(const t_strong_ptr *sptr);
const void *sptr_getwc(const t_weak_ptr *sptr);
const void *sptr_getoc(const t_owned_ptr *sptr);
const void *sptr_getuc(const t_uniq_ptr *u);
// non-const get
void       *sptr_gets(t_strong_ptr *sptr);
void       *sptr_getw(t_weak_ptr *sptr);
void       *sptr_geto(t_owned_ptr *sptr);
void       *sptr_getu(t_uniq_ptr *u);
/**
 * \def sptr_get(pointer)
 * \brief _generic macro to re-direct to sptr_get*
 * \param pointer smart pointer (const or non-const)
 * \return pointer to the underlying data, or NULL if invalid or destroyed
 * \note for specific infos check doc for sptr_get<type's letter>[C (const)]
 * \ingroup sptr
**/
#define sptr_get(pointer)                                                      \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_getw,                                               \
	    t_strong_ptr *: sptr_gets,                                             \
	    t_owned_ptr *: sptr_geto,                                              \
	    t_uniq_ptr *: sptr_getu,                                               \
	    const t_weak_ptr *: sptr_getwc,                                        \
	    const t_strong_ptr *: sptr_getsc,                                      \
	    const t_owned_ptr *: sptr_getoc,                                       \
	    const t_uniq_ptr *: sptr_getuc)(pointer)

size_t sptr_get_strongcount_s(const t_strong_ptr *sptr);
size_t sptr_get_strongcount_w(const t_weak_ptr *wptr);
size_t sptr_get_strongcount_o(const t_owned_ptr *optr);
size_t sptr_get_strongcount_u(const t_uniq_ptr *uptr);
/**
 * \def sptr_get_rc(pointer)
 * \brief _generic macro to re-direct to sptr_get_strongcount_*
 * \param pointer smart pointer (weak, strong, owned, uniq)
 * \return strong reference count (or 0 on error)
 * \note for specific infos check doc for sptr_strongcount_<type's letter>
 * \ingroup sptr
**/
#define sptr_get_rc(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_get_strongcount_w,                                  \
	    t_strong_ptr *: sptr_get_strongcount_s,                                \
	    t_uniq_ptr *: sptr_get_strongcount_u,                                  \
	    t_owned_ptr *: sptr_get_strongcount_o,                                 \
	    const t_weak_ptr *: sptr_get_strongcount_w,                            \
	    const t_strong_ptr *: sptr_get_strongcount_s,                          \
	    const t_uniq_ptr *: sptr_get_strongcount_u,                            \
	    const t_owned_ptr *: sptr_get_strongcount_o)(pointer)

size_t sptr_get_weakcount_s(const t_strong_ptr *sptr);
size_t sptr_get_weakcount_w(const t_weak_ptr *wptr);
size_t sptr_get_weakcount_o(const t_owned_ptr *optr);
size_t sptr_get_weakcount_u(const t_uniq_ptr *uptr);
/**
 * \def sptr_get_wc(pointer)
 * \brief _generic macro to re-direct to sptr_get_weakcount_*
 * \param pointer smart pointer (weak, strong, owned, uniq)
 * \return weak reference count (or 0 on error)
 * \note for specific infos check doc for sptr_weakcount_<type's letter>
 * \ingroup sptr
**/
#define sptr_get_wc(pointer)                                                   \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_get_weakcount_w,                                    \
	    t_strong_ptr *: sptr_get_weakcount_s,                                  \
	    t_uniq_ptr *: sptr_get_weakcount_u,                                    \
	    t_owned_ptr *: sptr_get_weakcount_o,                                   \
	    const t_weak_ptr *: sptr_get_weakcount_w,                              \
	    const t_strong_ptr *: sptr_get_weakcount_s,                            \
	    const t_uniq_ptr *: sptr_get_weakcount_u,                              \
	    const t_owned_ptr *: sptr_get_weakcount_o)(pointer)

t_dtor sptr_get_dtors(const t_strong_ptr *ptr);
t_dtor sptr_get_dtorw(const t_weak_ptr *ptr);
t_dtor sptr_get_dtoro(const t_owned_ptr *ptr);
t_dtor sptr_get_dtoru(const t_uniq_ptr *ptr);
/**
 * \def sptr_get_dtor(pointer)
 * \brief _generic macro to re-direct to sptr_get_dtor*
 * \param pointer smart pointer (weak, strong, owned, uniq)
 * \return destructor function pointer, or NULL on error
 * \note for specific infos check doc for sptr_get_dtor<type's letter>
 * \ingroup sptr
**/
#define sptr_get_dtor(pointer)                                                 \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_get_dtorw,                                          \
	    t_strong_ptr *: sptr_get_dtors,                                        \
	    t_uniq_ptr *: sptr_get_dtoru,                                          \
	    t_owned_ptr *: sptr_get_dtoro,                                         \
	    const t_weak_ptr *: sptr_get_dtorw,                                    \
	    const t_strong_ptr *: sptr_get_dtors,                                  \
	    const t_uniq_ptr *: sptr_get_dtoru,                                    \
	    const t_owned_ptr *: sptr_get_dtoro)(pointer)

bool sptr_set_dtors(t_strong_ptr *ptr, t_dtor dtor);
bool sptr_set_dtorw(t_weak_ptr *ptr, t_dtor dtor);
bool sptr_set_dtoro(t_owned_ptr *ptr, t_dtor dtor);
bool sptr_set_dtoru(t_uniq_ptr *ptr, t_dtor dtor);
/**
 * \def sptr_set_dtor(pointer, dtor)
 * \brief _generic macro to re-direct to sptr_set_dtor*
 * \param pointer smart pointer (weak, strong, owned, uniq)
 * \param dtor function pointer to call when freeing data inside the sptr
 * \return true on success, false otherwise
 * \note for specific infos check doc for sptr_set_dtor<type's letter>
 * \ingroup sptr
**/
#define sptr_set_dtor(pointer, dtor)                                           \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_set_dtorw,                                          \
	    t_strong_ptr *: sptr_set_dtors,                                        \
	    t_uniq_ptr *: sptr_set_dtoru,                                          \
	    t_owned_ptr *: sptr_set_dtoro)(pointer, dtor)

t_strong_ptr *sptr_copys(t_strong_ptr *sptr);
t_weak_ptr   *sptr_copyw(t_weak_ptr *weak_ptr);
t_owned_ptr   sptr_copyo(t_owned_ptr *owned_ptr);
t_uniq_ptr    sptr_copyu(t_uniq_ptr *ptr);
/**
 * \def sptr_copy(pointer)
 * \brief _generic macro to re-direct to sptr_copy*
 * \param pointer smart pointer to copy
 * \return copied smart pointer (or empty t_owned_ptr on error)
 * \note for specific infos check doc for sptr_copy<type's letter>
 * \ingroup sptr
**/
#define sptr_copy(pointer)                                                     \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_copyw,                                              \
	    t_strong_ptr *: sptr_copys,                                            \
	    t_uniq_ptr *: sptr_copyu,                                              \
	    t_owned_ptr *: sptr_copyo)(pointer)

bool sptr_dumps(int fd, const t_strong_ptr *ptr);
bool sptr_dumpw(int fd, const t_weak_ptr *ptr);
bool sptr_dumpo(int fd, const t_owned_ptr *ptr);
bool sptr_dumpu(int fd, const t_uniq_ptr *ptr);
/**
 * \def sptr_dump(fd, pointer)
 * \brief _generic macro to re-direct to sptr_dump*
 * \param fd file descriptor to output the dump to
 * \param pointer smart pointer to dump (weak, strong, owned, uniq)
 * \return true on success, false otherwise.
 * \note for specific infos check doc for sptr_dump<type's letter>
 * \ingroup sptr
**/
#define sptr_dump(fd, pointer)                                                 \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_dumpw,                                              \
	    t_strong_ptr *: sptr_dumps,                                            \
	    t_uniq_ptr *: sptr_dumpu,                                              \
	    t_owned_ptr *: sptr_dumpo,                                             \
	    const t_weak_ptr *: sptr_dumpw,                                        \
	    const t_strong_ptr *: sptr_dumps,                                      \
	    const t_uniq_ptr *: sptr_dumpu,                                        \
	    const t_owned_ptr *: sptr_dumpo)(fd, pointer)

char *sptr_to_strings(const t_strong_ptr *sptr);
char *sptr_to_stringw(const t_weak_ptr *sptr);
char *sptr_to_stringo(const t_owned_ptr *sptr);
char *sptr_to_stringu(const t_uniq_ptr *sptr);
/**
 * \brief turns a smart pointer to a string
 * \param pointer to turn to string
 * \note for specific infos check doc for sptr_to_string<type's letter>
 * \warning THE CALLER MUST FREE THE RETURNED STRING!
 * \ingroup sptr
**/
#define sptr_to_string(pointer)                                                \
	_Generic((pointer),                                                        \
	    t_weak_ptr *: sptr_to_stringw,                                         \
	    t_strong_ptr *: sptr_to_strings,                                       \
	    t_uniq_ptr *: sptr_to_stringu,                                         \
	    t_owned_ptr *: sptr_to_stringo,                                        \
	    const t_weak_ptr *: sptr_to_stringw,                                   \
	    const t_strong_ptr *: sptr_to_strings,                                 \
	    const t_uniq_ptr *: sptr_to_stringu,                                   \
	    const t_owned_ptr *: sptr_to_stringo)(pointer)

// -- non generic functions

t_strong_ptr sptr_into_strong(t_weak_ptr *sptr);
t_weak_ptr   sptr_into_weak(t_strong_ptr *sptr);
bool         sptr_transfer_ownership(t_owned_ptr *to, t_owned_ptr *from);
bool         sptr_move_ownership(t_owned_ptr *to, t_owned_ptr *from);
t_uniq_ptr  *sptr_uniq_yield(t_uniq_ptr *dst, t_uniq_ptr *from);
bool         sptr_uniq_swap(t_uniq_ptr *sw1, t_uniq_ptr *sw2);

#ifdef C_MINI__SPTR_INTERNAL

bool sptri_is_sole_box_owns(const t_strong_ptr *ptr);
bool sptri_is_sole_box_ownw(const t_weak_ptr *ptr);
bool sptri_is_sole_box_owno(const t_owned_ptr *ptr);
bool sptri_is_sole_box_ownu(const t_uniq_ptr *u);
/**
 * \def sptri_is_sole_bow_owner(pointer)
 * \brief _generic macro to re-direct to sptr_is_own*
 * \param pointer smart pointer to check (weak, strong, owned, uniq)
 * \return true if the pointer is considered an owner of the data, false
 * otherwise
 * \note for specific infos check doc for sptr_is_sole_box_own<type's letter>
 * \ingroup sptr
**/
#	define sptr_is_sole_box_owner(pointer)                                    \
		_Generic((pointer),                                                    \
		    t_weak_ptr *: sptri_is_sole_box_ownw,                              \
		    t_strong_ptr *: sptri_is_sole_box_owns,                            \
		    t_uniq_ptr *: sptri_is_sole_box_ownu,                              \
		    t_owned_ptr *: sptri_is_sole_box_owno,                             \
		    const t_weak_ptr *: sptri_is_sole_box_ownw,                        \
		    const t_strong_ptr *: sptri_is_sole_box_owns,                      \
		    const t_uniq_ptr *: sptri_is_sole_box_ownu,                        \
		    const t_owned_ptr *: sptri_is_sole_box_owno)(pointer)

// defined functions documentation here due to multiple [wr]c_[inc|dec|get]
//		implementation for atomic and non-atomic and other variations of rc/wc

/**
 * \brief increases the strong count (rc) of a meta info box
 * \param b the box to increase the strong count (rc) of
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
void sptri_rc_inc(t_fbox *b);
/**
 * \brief decrease the strong count (rc) of a meta info box
 * \param b the box to decrease the strong count (rc) of
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
void sptri_rc_dec(t_fbox *b);

/**
 * \brief increase the weak count (wc) of a meta info box
 * \param b the box to increase the weak count (wc) of
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
void sptri_wc_inc(t_fbox *b);
/**
 * \brief decrease the weak count (wc) of a meta info box
 * \param b the box to decrease the weak count (wc) of
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
void sptri_wc_dec(t_fbox *b);

/**
 * \brief get the strong count (rc) of a meta info box
 * \param b the box to get the strong count (rc) of
 * \return the value of strong count (rc)
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
size_t sptri_rc_get(t_fbox *b);
/**
 * \brief get the weak count (wc) of a meta info box
 * \param b the box to get the weak count (wc) of
 * \return the value of weak count (wc)
 * \note variation exists for : base size_t, atomic size_t
 * \ingroup sptr
**/
size_t sptri_wc_get(t_fbox *b);

#endif // !C_MINI__SPTR_INTERNAL

/**
 * \def auto_strong
 * \brief shortcut to declare an auto deleted strong pointer when variable goes
 * out of scope
 * \ingroup sptr
**/
#define auto_strong __attribute__((cleanup(sptr_deletes))) t_strong_ptr
/**
 * \def auto_weak
 * \brief shortcut to declare an auto deleted weak pointer when variable goes
 * out of scope
 * \ingroup sptr
**/
#define auto_weak   __attribute__((cleanup(sptr_deletew))) t_weak_ptr
/**
 * \def auto_owned
 * \brief shortcut to declare an auto deleted owned pointer when variable goes
 * out of scope
 * \ingroup sptr
**/
#define auto_owned  __attribute__((cleanup(sptr_deleteo))) t_owned_ptr
/**
 * \def auto_uniq
 * \brief shortcut to declare an auto deleted unique pointer when variable goes
 * out of scope
 * \ingroup sptr
**/
#define auto_uniq   __attribute__((cleanup(sptr_deleteu))) t_unique_ptr

#endif //! C_MINI__SPTR
