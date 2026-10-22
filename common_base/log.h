#ifndef C_MINI__CB_LOG

#	if !defined(C_MINI__TESTS) || !defined(C_MINI__DEBUG_LEVEL) ||            \
	    C_MINI__DEBUG_LEVEL == 0
#		ifndef CM_LOG
#			define CM_LOG(level, fmt, ...) (void)0;
#		endif // !CM_LOG

#		ifndef CM_DBG
#			define CM_DBG(fmt, ...) (void)0;
#		endif // !CM_DBG

#		ifndef CM_DBG_POS
#			define CM_DBG_POS(fmt, ...) (void)0;
#		endif // !CM_DBG_POS

#		ifndef CM_TODO
#			define CM_TODO(x, ...) (void)0;
#		endif //! CM__TODO
#	endif     // !TESTS || !DEBUG_LVL || DEBUG_LVL==0

#	ifdef C_MINI__TESTS

#		include <stdio.h>

// colors
#		define C_MINI__LOG_L_COLOR     "\033[33m"
#		define C_MINI__LOG_D_COLOR     "\033[36;1m"
#		define C_MINI__LOG_DP_COLOR    "\033[34;0m"
#		define C_MINI__LOG_RESET_COLOR "\033[0m"

// thread-safe version
#		ifdef C_MINI__LOG_THREADSAFE
#			include <pthread.h>
extern pthread_mutex_t console_lock;

#			ifndef CM_LOG
#				define CM_LOG(level, fmt, ...)                                \
					do {                                                       \
						if (level > (size_t)C_MINI__DEBUG_LEVEL) break;        \
						pthread_mutex_lock(&console_lock);                     \
						printf(C_MINI__LOG_L_COLOR                             \
						       "[LOG]" C_MINI__LOG_RESET_COLOR ": %s: " fmt    \
						       "\n",                                           \
						       __func__ __VA_OPT__(, ) __VA_ARGS__);           \
						fflush(stdout);                                        \
						pthread_mutex_unlock(&console_lock);                   \
					} while (0);
#			endif // !CM_LOG

#			ifndef CM_DBG
#				define CM_DBG(fmt, ...)                                       \
					do {                                                       \
						pthread_mutex_lock(&console_lock);                     \
						printf(C_MINI__LOG_D_COLOR                             \
						       "[DBG]" C_MINI__LOG_RESET_COLOR ": %s: " fmt    \
						       "\n",                                           \
						       __func__ __VA_OPT__(, ) __VA_ARGS__);           \
						fflush(stdout);                                        \
						pthread_mutex_unlock(&console_lock);                   \
					} while (0);
#			endif // !CM_DBG

// non-threadsafe version
#		else // || non-threadsafe

#			include <stdio.h>

#			ifndef CM_LOG
#				define CM_LOG(level, fmt, ...)                                \
					do {                                                       \
						if (level > (size_t)C_MINI__DEBUG_LEVEL) break;        \
						printf(C_MINI__LOG_L_COLOR                             \
						       "[LOG]" C_MINI__LOG_RESET_COLOR ": %s: " fmt    \
						       "\n",                                           \
						       __func__ __VA_OPT__(, ) __VA_ARGS__);           \
						fflush(stdout);                                        \
					} while (0);
#			endif // !CM_LOG

#			ifndef CM_DBG
#				define CM_DBG(fmt, ...)                                       \
					do {                                                       \
						printf(C_MINI__LOG_D_COLOR                             \
						       "[DBG]" C_MINI__LOG_RESET_COLOR ": %s: " fmt    \
						       "\n",                                           \
						       __func__ __VA_OPT__(, ) __VA_ARGS__);           \
						fflush(stdout);                                        \
					} while (0);
#			endif // !CM_DBG

#		endif // !Threadsafe

#		ifndef CM_DBG_POS
#			define CM_DBG_POS                                                 \
				CM_DBG(C_MINI__LOG_DP_COLOR "[POS]" C_MINI__LOG_RESET_COLOR    \
				                            ": %s:%d %s\n",                    \
				       __FILE__, __LINE__, __FUNCTION__)
#		endif // !CM_DBG_POS

#		ifndef CM_TODO
#			define CM_TODO(x, ...)                                            \
				CM_LOG(C_MINI__LEVEL_ALWAYS,                                   \
				       "%s:%d\t" C_MINI__LOG_ETODO "\t" x, __FILE__,           \
				       __LINE__ __VA_OPT__(, ) __VA_ARGS__)
#		endif //! CM__TODO

#		define C_MINI__LOG_F_PREFIX "Fail: "

#		define C_MINI__LOG_ETODO  C_MINI__LOG_F_PREFIX "Not yet implemented."
#		define C_MINI__LOG_EALLOC C_MINI__LOG_F_PREFIX "Allocation."
#		define C_MINI__LOG_EARG   C_MINI__LOG_F_PREFIX "Bad arguments."
#		define C_MINI__LOG_FAIL   C_MINI__LOG_F_PREFIX "Unspecified."

#		define C_MINI__LOG_SUCCESS "Success."

// levels
#		define C_MINI__LEVEL_INFO   4 // Success and other info
#		define C_MINI__LEVEL_WARN   3 // Bad args and other stuff
#		define C_MINI__LEVEL_ERR    2 // Internal big errors eg alloc
#		define C_MINI__LEVEL_ALWAYS 1 // Test/constant prints

#	endif // !TESTS

#endif // !C_MINI__CB_LOG
