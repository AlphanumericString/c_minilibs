#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define C_MINI__SPTR_INTERNAL
#include "sptr/smart_pointer.h"

/**
 * \brief convert a shared (strong) pointer to a string
 * \param sptr shared (strong) pointer to convert
 * \return string representation of the shared (strong) pointer
 * \warning caller is responsible for freeing the returned string
 * \note fields printed: data, strong count, weak count, dtor
 * \ingroup sptr
**/
char *sptr_to_strings(const t_strong_ptr *sptr) {
	char       *ret = NULL;
	size_t      len;
	const char *fmt = "SPTR STRONG { data %p, count %zu (weak:%zu), dtor %p}";

	if (!sptr || !sptr->meta) return (strdup("SPTR STRONG (nil)"));

	len = snprintf(ret, 0, fmt, sptr_get(sptr), sptr_get_rc(sptr),
	               sptr_get_wc(sptr), sptr_get_dtor(sptr));
	ret = malloc(len + 1);
	if (!ret) return (NULL);
	snprintf(ret, len + 1, fmt, sptr_get(sptr), sptr_get_rc(sptr),
	         sptr_get_wc(sptr), sptr_get_dtor(sptr));
	return (ret);
}
/**
 * \brief convert a shared (weak) pointer to a string
 * \param sptr shared (weak) pointer to convert
 * \return string representation of the shared (weak) pointer
 * \warning caller is responsible for freeing the returned string
 * \note fields printed: data, strong count, weak count, dtor
 * \ingroup sptr
**/
char *sptr_to_stringw(const t_weak_ptr *sptr) {
	char       *ret = NULL;
	size_t      len;
	const char *fmt = "SPTR WEAK { data %p, count %zu (weak:%zu), dtor %p}";

	if (!sptr || !sptr->meta) return (strdup("SPTR WEAK (nil)"));
	len = snprintf(ret, 0, fmt, sptr_get(sptr), sptr_get_rc(sptr),
	               sptr_get_wc(sptr), (void *)sptr_get_dtor(sptr));
	ret = malloc(len + 1);
	if (!ret) return (NULL);
	snprintf(ret, len + 1, fmt, sptr_get(sptr), sptr_get_rc(sptr),
	         sptr_get_wc(sptr), (void *)sptr_get_dtor(sptr));
	return (ret);
}
/**
 * \brief convert a owned pointer to a string
 * \param sptr owned pointer to convert
 * \return string representation of the owned pointer
 * \warning caller is responsible for freeing the returned string
 * \note fields printed: ownership status, data, strong count, weak count, dtor
 * \ingroup sptr
**/
char *sptr_to_stringo(const t_owned_ptr *sptr) {
	char       *ret = NULL;
	size_t      len;
	const char *fmt = "SPTR OWND %s { data %p, count %zu (weak:%zu), dtor %p}";

	if (!sptr || !sptr->meta) return (strdup("SPTR OWND (nil)"));
	len = snprintf(ret, 0, fmt, sptr->owner ? "owned" : "borrowed",
	               sptr_get(sptr), sptr_get_rc(sptr), sptr_get_wc(sptr),
	               (void *)sptr_get_dtor(sptr));
	ret = malloc(len + 1);
	if (!ret) return (NULL);
	snprintf(ret, len + 1, fmt, sptr->owner ? "owned" : "borrowed",
	         sptr_get(sptr), sptr_get_rc(sptr), sptr_get_wc(sptr),
	         (void *)sptr_get_dtor(sptr));
	return (ret);
}
/**
 * \brief convert a uniq pointer to a string
 * \param sptr uniq pointer to convert
 * \return string representation of the uniq pointer
 * \warning caller is responsible for freeing the returned string
 * \note fields printed: data, dtor
 * \ingroup sptr
**/
char *sptr_to_stringu(const t_uniq_ptr *sptr) {
	char       *ret = NULL;
	size_t      len;
	const char *fmt = "SPTR UNIQ { data %p, dtor %p}";

	if (!sptr) return (strdup("SPTR UNIQ (nil)"));
	len = snprintf(ret, 0, fmt, sptr_get(sptr), (void *)sptr_get_dtor(sptr));
	ret = malloc(len + 1);
	if (!ret) return (NULL);
	snprintf(ret, len + 1, fmt, sptr_get(sptr), (void *)sptr_get_dtor(sptr));
	return (ret);
}

/**
 * \brief dump debug information for a shared (strong) pointer to a given file
 * descriptor
 * \param fd file descriptor to output to
 * \param sptr shared (strong) pointer to dump
 * \return true on success, false otherwise
 * \note prints information using the sptr_to_string format
 * \ingroup sptr
**/
bool sptr_dumps(int fd, const t_strong_ptr *sptr) {
	char *s;

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return false;
	}
	s = sptr_to_strings(sptr);
	if (!s) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return false;
	}
	dprintf(fd, "%s\n", s);
	free(s);
	return true;
}
/**
 * \brief dump debug information for a shared (weak) pointer to a given file
 * descriptor
 * \param fd file descriptor to output to
 * \param wptr shared (weak) pointer to dump
 * \return true on success, false otherwise
 * \note prints information using the sptr_to_string format
 * \ingroup sptr
**/
bool sptr_dumpw(int fd, const t_weak_ptr *wptr) {
	char *s;

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return false;
	}
	s = sptr_to_stringw(wptr);
	if (!s) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return false;
	}
	dprintf(fd, "%s\n", s);
	free(s);
	return true;
}
/**
 * \brief dump debug information for a owned pointer to a given file descriptor
 * \param fd file descriptor to output to
 * \param optr owned pointer to dump
 * \return true on success, false otherwise
 * \note prints information using the sptr_to_string format
 * \ingroup sptr
**/
bool sptr_dumpo(int fd, const t_owned_ptr *optr) {
	char *s;

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return false;
	}
	s = sptr_to_stringo(optr);
	if (!s) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return false;
	}
	dprintf(fd, "%s\n", s);
	free(s);
	return true;
}
/**
 * \brief dump debug information for a uniq pointer to a given file descriptor
 * \param fd file descriptor to output to
 * \param uptr uniq pointer to dump
 * \return true on success, false otherwise
 * \note prints information using the sptr_to_string format
 * \ingroup sptr
**/
bool sptr_dumpu(int fd, const t_uniq_ptr *uptr) {
	char *s;

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__SPTR_LOG_EARG)
		return false;
	}
	s = sptr_to_stringu(uptr);
	if (!s) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__SPTR_LOG_EALLOC)
		return false;
	}
	dprintf(fd, "%s\n", s);
	free(s);
	return true;
}
