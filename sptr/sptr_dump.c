#include <stdio.h>

#include "sptr/smart_pointer.h"

static int depth = 0;

#define put_with_offset(fd, fmt, arg)                                          \
	do {                                                                       \
		for (int i = 0; i < depth; i++)                                        \
			dprintf(fd, "\t");                                                 \
		dprintf(fd, fmt, arg);                                                 \
	} while (0);

static void sptr_dump_box(int fd, const t_smartbox *box) {
	char *type[] = {
	    "UNK",
	    "SHARED",
	    "UNIQ",
	};
	int typ = 0;

	if (fd < 0) return;
	put_with_offset(fd, "box: %p", box);
	if (box) {
		printf(" {\n");
		depth++;
		if (box->type == SPTR_SHRD || box->type == SPTR_UNIQ) typ = box->type;
		else typ = 0;
		put_with_offset(fd, "type:%s\n", type[typ]);
		put_with_offset(fd, "strong_nb:%zu\n", box->sc);
		put_with_offset(fd, "weak_nb:%zu\n", box->wc);
		put_with_offset(fd, "data:%p\n", box->raw);
		put_with_offset(fd, "free-er:%p\n", box->dtor);
		depth--;
		put_with_offset(fd, "}\n", NULL);
	} else {
		printf("\n");
	}
}

/**
 * \brief dumps info about the sptr at given filedescriptor \a fd
 * \param fd filedescriptor to write info to
 * \param t the smart pointer to describe
 * \ingroup sptr
 **/
void sptr_dump(int fd, const t_sptr *t) {
	char *stre[] = {
	    "UNK",
	    "WEAK",
	    "STRONG",
	};
	int str = 0;

	if (fd < 0) return;
	dprintf(fd, "sptr:%p {\n", t);
	depth++;
	if (!t) {
		put_with_offset(fd, "nil\n", NULL);
		depth--;
	} else {
		if (t->smart_strength == SPTR_W || t->smart_strength == SPTR_S)
			str = t->smart_strength;
		else str = 0;
		put_with_offset(fd, "strength:%s\n", stre[str]);
		sptr_dump_box(fd, t->box);
		depth--;
	}
	put_with_offset(fd, "}\n", NULL);
}
