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

typedef enum { SPTR_WEAK = 1, SPTR_STRONG = 2 } e_ptrstr;
typedef enum { SPTR_SHARED = 1, SPTR_UNIQ = 2 } e_ptrtype;
typedef struct s_box {
	void     *raw;
	void      (*closure)(void *);
	size_t    wc;
	size_t    sc;
	e_ptrtype type;
} t_smartbox;

typedef struct {
	e_ptrstr    smart_strength;
	t_smartbox *box;
} t_sptr;

t_sptr sptr_make_shared_v(void *raw_ptr, void (*free)(void *));
t_sptr sptr_make_shared_fromsptr(t_sptr *smart_ptr, void (*free)(void *));

t_sptr sptr_make_uniq_v(void *raw_ptr, void (*free)(void *));
t_sptr sptr_make_uniq_fromsptr(t_sptr *smart_ptr, void (*free)(void *));

#define sptr_make_shared(pointer, free_func)                                   \
	_Generic((pointer),                                                        \
	    t_sptr *: sptr_make_shared_fromsptr,                                   \
	    default: sptr_make_shared_v)(pointer, free_func)
#define sptr_make_uniq(pointer, free_func)                                     \
	_Generic((pointer),                                                        \
	    t_sptr *: sptr_make_uniq_fromsptr,                                     \
	    default: sptr_make_uniq_v)(pointer, free_func)

bool sptr_delete(t_sptr *sptr);

t_sptr sptr_copy(t_sptr *sptr);
t_sptr sptr_weakcopy(t_sptr *sptr);
t_sptr sptr_strongcopy(t_sptr *sptr);

void  *sptr_get(const t_sptr *sptr);
size_t sptr_get_wc(const t_sptr *sptr);
size_t sptr_get_sc(const t_sptr *sptr);
void   (*sptr_get_closure(const t_sptr *s))(void *);

bool sptr_weaken(t_sptr *sptr);
bool sptr_strengthen(t_sptr *sptr);
bool sptr_steal(t_sptr *to, t_sptr *sptr);

void sptr_dump(int fd, const t_sptr *t);

#endif //! C_MINI__SPTR
