#ifndef C_MINI__CB_LOG

#	ifdef C_MINI__TESTS

#		include <stdio.h>

// colors
#		define C_MINI__LOG_L_COLOR     "\e[33m"
#		define C_MINI__LOG_D_COLOR     "\e[36;1m"
#		define C_MINI__LOG_DP_COLOR    "\e[34;0m"
#		define C_MINI__LOG_RESET_COLOR "\e[0m"

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

#			ifndef CM_DBG_POS
#				define CM_DBG_POS                                             \
					do {                                                       \
						pthread_mutex_lock(&console_lock);                     \
						printf(C_MINI__LOG_DP_COLOR                            \
						       "[POS]" C_MINI__LOG_RESET_COLOR ": %s:%d %s\n", \
						       __FILE__, __LINE__, __FUNCTION__);              \
						fflush(stdout);                                        \
						pthread_mutex_unlock(&console_lock);                   \
					} while (0);
#			endif // !CM_DBG_POS

// non-threadsafe version
#		else // !threadsafe

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

#			ifndef CM_DBG_POS
#				define CM_DBG_POS                                             \
					do {                                                       \
						printf(C_MINI__LOG_DP_COLOR                            \
						       "[POS]" C_MINI__LOG_RESET_COLOR ": %s:%d %s\n", \
						       __FILE__, __LINE__, __FUNCTION__);              \
						fflush(stdout);                                        \
					} while (0);
#			endif // !CM_DBG_POS

#		endif // macro end

#		define C_MINI__LOG_F_PREFIX "Fail: "

#		define C_MINI__LOG_ETODO  C_MINI__LOG_F_PREFIX "Not yet implemented."
#		define C_MINI__LOG_EALLOC C_MINI__LOG_F_PREFIX "Allocation."
#		define C_MINI__LOG_EARG   C_MINI__LOG_F_PREFIX "Bad arguments."
#		define C_MINI__LOG_FAIL   C_MINI__LOG_F_PREFIX "Unspecified."

#		define C_MINI__LOG_SUCCESS "Success."

// levels
#		define C_MINI__LEVEL_INFO   3 // Success and other info
#		define C_MINI__LEVEL_WARN   2 // Bad args and other stuff
#		define C_MINI__LEVEL_ERR    1 // Internal big errors eg alloc
#		define C_MINI__LEVEL_ALWAYS 0 // Test/constant prints

#	else // || TESTS

#		ifndef CM_LOG
#			define CM_LOG(level, fmt, ...) (void)0;
#		endif // !CM_LOG

#		ifndef CM_DBG
#			define CM_DBG(fmt, ...) (void)0;
#		endif // !CM_DBG

#		ifndef CM_DBG_POS
#			define CM_DBG_POS (void)0;
#		endif // !CM_DBG_POS

#	endif // !TESTS

#endif // !C_MINI__CB_LOG
