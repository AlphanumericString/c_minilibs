#ifndef C_MINI__LRUC_H
#define C_MINI__LRUC_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// TODO: use a C_MINI__LRU_SATURATION to define max average load per buckets
// right now : 1. meaning for x buckets max x nodes active.

#define C_MINI__LRUC_INTERNAL
#define C_MINI__LRUC_HASH_SKIP 10

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	define DBG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[33;5m[DBG]\e[0m: %s: " fmt "\n",                        \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0)

#	define LOG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[32m[LOG]\e[0m: %s: " fmt "\n",                          \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0)

#	define DBG_POS                                                            \
		do {                                                                   \
			DBG(" %s:%d \e[33;4m[PATH]\e[0m", __FILE__, __LINE__);             \
			fflush(stdout);                                                    \
		} while (0)

#define C_MINI__LRUC_LOG_SUCCESS  "Success"
#define C_MINI__LRUC_LOG_EARG     "Bad arguments"
#define C_MINI__LRUC_LOG_EALLOC   "Allocation faillure"
#define C_MINI__LRUC_LOG_ECLOCK   "Clock error"
#define C_MINI__LRUC_LOG_FAILLURE "Faillure"
#define C_MINI__LRUC_LOG_EVICTION "Eviction: key=%p delta (%ld) >= ttl (%ld)"
#define C_MINI__LRUC_LOG_UPDATE   "Update ttl: %ld->%ld"
#define C_MINI__LRUC_LOG_HIT      "Hit: hit=%zu, key:%p, key_len:%zu"
#define C_MINI__LRUC_LOG_MISS_LRU "LRU-Miss: miss=%zu, key:%p, key_len:%zu"
#define C_MINI__LRUC_LOG_MISS_TTL "TTL-Miss: miss=%zu, key:%p, key_len:%zu delta(%ld)"
#define C_MINI__LRUC_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#define C_MINI__LRUC_LOG_CREATEVAL "Create: key:%p, key_len:%zu"
#define C_MINI__LRUC_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"
#define C_MINI__LRUC_LOG_FAST_FREE "Fast free via list started"
#define C_MINI__LRUC_LOG_LFREE     "Cleanup via list started"
#define C_MINI__LRUC_LOG_HFREE     "Cleanup via hash started"

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TESTS

typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} t_lruc_stats;

typedef struct {
	const void *key;
	void       *value;
	size_t      k_len;
} t_keyvalue;

#ifdef C_MINI__LRUC_INTERNAL

typedef struct s_keycache {
	t_keyvalue kv;

	uint64_t time; // time of emition

	struct s_keycache *hash_next;

	size_t	heap_index;

	struct s_keycache *ttl_prev;
	struct s_keycache *ttl_next;

	struct s_keycache *lru_prev;
	struct s_keycache *lru_next;

} t_lruc_keynode;

typedef struct {
	size_t           buckets_nb;
	size_t           node_in_use;
	t_lruc_keynode **buckets; // allocated nodes ordered via hash for lookup
	t_lruc_keynode **heap; // ttl minheap

	t_lruc_keynode *MRU; // MRU
	t_lruc_keynode *LRU; // LRU

	t_lruc_keynode *TTL_Max;  // TTL head
	t_lruc_keynode *TTL_Min;  // TTL tail
	uint64_t        ttl_glob; // TTL window

	void         (*dtor)(void *); //
	t_lruc_stats stats;
} t_lruc;

#else

typedef struct {
	size_t buckets_nb;
	size_t node_in_use;
	void **buckets;
	void **heap;

	void *MRU; // MRU
	void *LRU; // LRU

	void *TTL_Max;  // TTL head
	void *TTL_Min;  // TTL tail
	long  ttl_glob; // TTL window

	void         (*dtor)(void *);
	t_lruc_stats stats;
} t_lruc;

#endif // C_MINI__LRU_INTERNAL

#ifndef CMINI__T_KVCALLBACK_DEFINE
#	define CMINI__T_KVCALLBACK_DEFINE

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_kvcallback)(t_keyvalue *elem, void *arg);

#endif // CMINI__T_CALLBAKC_DEFINE

// create
t_lruc *lruc_create(size_t capacity, void (*free_er)(void *));
t_lruc *lruc_init(t_lruc *cache, size_t cappacity, void (*free_er)(void *));
void    lruc_destroy(t_lruc *cache); // destroy sub-ressources not cache itself
void    lruc_free(t_lruc *cache);    // destroy cache itself

// put
int lruc_put_kv(t_lruc *cache, t_keyvalue kv);
int lruc_put(t_lruc *cache, const void *key, size_t key_len,
              const void *value);
int lruc_put_ttl(t_lruc *cache, const void *key, size_t key_len,
                  const void *value, uint64_t ttl_ms);

// get
void *lruc_get(t_lruc *cache, const void *key, size_t key_len);

// rms
void lruc_clear(t_lruc *cache); // clear sub ressources
int  lruc_remove(t_lruc *cache, const void *key, size_t key_len);

// has
bool lruc_has(const t_lruc *cache, const void *key, size_t key_len);

// expired
bool lruc_isexpired(t_lruc *cache, const void *key,
                     size_t key_len); //  is key in the cache and expired?
void lruc_cleanup(t_lruc *cache);    // call dtor on all expired elements
void lruc_setttl(t_lruc *cache, long ttl_ms); // set default ttl fo keys

// infos
size_t       lruc_size(const t_lruc *cache);
uint64_t     lruc_hits(const t_lruc *cache);
uint64_t     lruc_misses(const t_lruc *cache);
uint64_t     lruc_expired(const t_lruc *cache);
t_lruc_stats lruc_stats(const t_lruc *cache);

// foreach
void lruc_foreach(const t_lruc *cache, t_kvcallback fn, void *user_data);
void lruc_foreach_ru_ordered(const t_lruc *c, t_kvcallback fn,
                              void *user_data);
void lruc_foreach_ttl_ordered(const t_lruc *c, t_kvcallback fn,
                               void *user_data);

#ifdef C_MINI__LRUC_INTERNAL
uint64_t lruci_timeto64(struct timespec t);
bool     lruci_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len);
void     lruci_unlink_node(t_lruc *c, t_lruc_keynode *node, size_t hash);

size_t lruci_hash_djb2(const void *key, size_t size);

t_lruc_keynode *lruci_keyinhash(const t_lruc *c, const void *key,
                                       size_t key_len, size_t *_pos);
t_lruc_keynode *lruci_keyinlist(const t_lruc *c, const void *key,
                                       size_t key_len);

void            lruci_push_lru(t_lruc *c, t_lruc_keynode *node);
t_lruc_keynode *lruci_pop_lru(t_lruc *c, t_lruc_keynode *node);
t_lruc_keynode *lruci_pop_ttl(t_lruc *c, t_lruc_keynode *n);
t_lruc_keynode *lruci_pop_hash(t_lruc *c, t_lruc_keynode *n, size_t hash);
void lruci_push_hash(t_lruc *c, t_lruc_keynode *n, size_t hash);

#endif // !C_MINI__LRUC_INTERNAL

#endif // !C_MINI__LRUC_H
