#ifndef C_MINI__CACHE_GENERIC_H
#define C_MINI__CACHE_GENERIC_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define C_MINI__LRU_HASH_SKIP 10

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	ifndef LOG
#		define LOG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                      \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                    \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS                                                        \
			do {                                                               \
				printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__, __LINE__,  \
				       __FUNCTION__);                                          \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG_POS

#	define C_MINI__LRU_LOG_EVICTION                                           \
		"Eviction: key=%p reason:ttl, delta (%ld) >= ttl (%ld)"
#	define C_MINI__LRU_LOG_UPDATE   "Update ttl: %ld->%ld)"
#	define C_MINI__LRU_LOG_HIT      "Hit: hit=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRU_LOG_MISS_LRU "LRU-Miss: miss=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRU_LOG_MISS_TTL                                           \
		"TTL-Miss: miss=%zu, key:%p, key_len:%zu delta(%ld)"
#	define C_MINI__LRU_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#	define C_MINI__LRU_LOG_CREATEVAL "Create: key:%p, key_len:%zu"
#	define C_MINI__LRU_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"
#	define C_MINI__LRU_LOG_FAST_FREE "Fast free via list started"
#	define C_MINI__LRU_LOG_LFREE     "Cleanup via list started"
#	define C_MINI__LRU_LOG_HFREE     "Cleanup via hash started"

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
	void  *key;
	void  *value;
	size_t k_len;
} t_keyvalue;

#ifdef C_MINI__LRU_INTERNAL

typedef struct s_keycache {
	t_keyvalue kv;

	struct timespec time; // time of emition

	struct s_keycache *ttl_prev;
	struct s_keycache *ttl_next;

	struct s_keycache *lru_prev;
	struct s_keycache *lru_next;

} t_lruc_keynode;

typedef struct {
	size_t cap;
	size_t nb;
	t_lruc_keynode *
	    *h_arr; // array of allocated nodes ordered via hash for lookup

	t_lruc_keynode *MRU; // MRU
	t_lruc_keynode *LRU; // LRU

	t_lruc_keynode *TTL_Max;  // TTL head
	t_lruc_keynode *TTL_Min;  // TTL tail
	long            ttl_glob; // TTL window

	void         (*dtor)(void *); //
	t_lruc_stats stats;
} t_lruc;

#else

typedef struct {
	size_t cap;
	size_t nb;
	void **h_arr; // array of allocated nodes ordered via hash for lookup

	void *MRU; // MRU
	void *LRU; // LRU

	void *TTL_Max;  // TTL head
	void *TTL_Min;  // TTL tail
	long  ttl_glob; // TTL window

	void         (*dtor)(void *); //
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
t_lruc *cache_create(size_t capacity, void (*free_er)(void *));
t_lruc *cache_init(t_lruc *cache, size_t cappacity, void (*free_er)(void *));
void    cache_destroy(t_lruc *cache); // destroy sub-ressources not cache itself
size_t  hash_djb2(const void *key, size_t size);

// put
int cache_put_kv(t_lruc *cache, t_keyvalue kv);
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value);
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len, void *value,
                  uint64_t ttl_ms);

// get
void *cache_get(t_lruc *cache, const void *key, size_t key_len);

// rms
void cache_clear(t_lruc *cache); // clear sub ressources
int  cache_remove(t_lruc *cache, const void *key, size_t key_len);

// has
bool cache_has(const t_lruc *cache, const void *key, size_t key_len);

// expired
bool cache_isexpired(t_lruc *cache, const void *key,
                     size_t key_len); //  is key in the cache and expired?
void cache_cleanup(t_lruc *cache);    // call dtor on all expired elements
void cache_setttl(t_lruc *cache, long ttl);

// infos
size_t       cache_size(const t_lruc *cache);
uint64_t     cache_hits(const t_lruc *cache);
uint64_t     cache_misses(const t_lruc *cache);
uint64_t     cache_expired(const t_lruc *cache);
t_lruc_stats cache_stats(const t_lruc *cache);

// foreach
void cache_foreach(t_lruc *cache, t_kvcallback fn, void *user_data);
void cache_foreach_ru_ordered(t_lruc *c, t_kvcallback fn, void *user_data);
void cache_foreach_ttl_ordered(t_lruc *c, t_kvcallback fn, void *user_data);

#ifdef C_MINI__LRU_INTERNAL
void cache_unlink_node(t_lruc *c, t_lruc_keynode *node);
long distance_ms(struct timespec a, struct timespec b);

t_lruc_keynode *cache_search_keyinhash(const t_lruc *c, const void *key,
                                       size_t key_len, size_t *_pos);
t_lruc_keynode *cache_search_keyinlist(const t_lruc *c, const void *key,
                                       size_t key_len);

void            push_lruc(t_lruc *c, t_lruc_keynode *node);
t_lruc_keynode *pop_lruc(t_lruc *c, t_lruc_keynode *node);
t_lruc_keynode *pop_ttl(t_lruc *c, t_lruc_keynode *n);

#endif // C_MINI__LRU_INTERNAL

// TODO: change TTL to minheap -> o(log n) for insertion (faster)

#endif // C_MINI__CACHE_GENERIC_H
