#include <stdio.h>
#include <stdlib.h>

#include "rb_tests.h"

bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}

bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}

bool cb_dump_int(void *elem, void *arg) {
	(void)arg;
	printf("%d ", *(int *)elem);
	return true;
}
bool cb_add_one(void *elem, void *arg) {
	(void)arg;
	*(int *)elem += 1;
	return true;
}

int cb_cmp(void *a, void *b) {
	return *(int *)a - *(int *)b;
}

void *cb_dup(const void *elem) {
	int *res = malloc(sizeof(int));
	if (res) *res = *(const int *)elem;
	return res;
}

void cb_destroy(void *elem) {
	(void)elem;
}
