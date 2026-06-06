#include <stdlib.h>
#include <unistd.h>

#include "tls/ctls.h"

struct s_mine {
	int error;
	int last_status;
};

#define TLS_FTLS_STRUCT struct s_mine

#include "tls/ftls.h"
// #include "tls/vtls.h"

static void dump_mine_node(struct s_mine a) {
	printf("error:%d\tlst_status:%d\n", a.error, a.last_status);
}

// tries to get toto then increment and print.
void *ctls_dummy(void *__arg) {
	t_ctls_typewrap *a;
	char            *thrd_name = (char *)__arg;
	size_t           i         = 15;

	ctls_set("toto", (t_ctls_typewrap){.type = CTLS_INT, .value.i = 42});
	while (i--) {
		a = ctls_get("toto");
		printf("%s:", thrd_name);
		if (!a) printf("no toto\n");
		else {
			if (a->type == CTLS_INT) a->value.i++;
			ctls_dump_typewrap(*a);
			printf("\n");
		}
		sleep(1);
		if (i == 10) {
			printf("removing toto from ctls buffer: %s\n",
			       ctls_rm("toto") ? "true" : "false");
		}
	}
	return (NULL);
}

// tries to get ftls set random(0-9) lst_status if lst_status == 0, error+=1
void *ftls_dummy(void *__arg) {
	char   *thrd_name = (char *)__arg;
	size_t  i         = 15;
	t_ftls *a;

	while (i--) {
		a = ftls_get();
		printf("%s:", thrd_name);
		if (!a) printf("no ftls\n");
		else {
			a->last_status = rand() % 10;
			if (!a->last_status) a->error++;
			dump_mine_node(*a);
		}
		sleep(1);
	}
	return (NULL);
}

static int ctls_tests(void) {
	t_ctls_typewrap *a;
	pthread_t        handle1;
	pthread_t        handle2;
	void            *ret;

	a = ctls_get("toto");
	printf("main:");
	a ? ctls_dump_typewrap(*a) : printf("no toto");
	printf("\n");

	ctls_set("toto", (t_ctls_typewrap){.type = CTLS_INT, .value.i = 42});
	a = ctls_get("toto");
	printf("main:");
	a ? ctls_dump_typewrap(*a) : printf("no toto");
	printf("\n");

	pthread_create(&handle1, NULL, ctls_dummy, " jcvd");
	pthread_create(&handle2, NULL, ctls_dummy, "trump");

	pthread_join(handle1, &ret);
	pthread_join(handle2, &ret);
	return EXIT_SUCCESS;
}

static int ftls_tests(void) {
	pthread_t handle1;
	pthread_t handle2;
	void     *ret;
	t_ftls   *a;

	a = ftls_get();
	printf("main:");
	a ? dump_mine_node(*a) : printf("no ftls");
	printf("\n");

	a->error       = 42;
	a->last_status = 99;
	a              = ftls_get();
	printf("main:");
	a ? dump_mine_node(*a) : printf("no ftls");
	printf("\n");

	pthread_create(&handle1, NULL, ftls_dummy, " jcvd");
	pthread_create(&handle2, NULL, ftls_dummy, "trump");

	pthread_join(handle1, &ret);
	pthread_join(handle2, &ret);
	return EXIT_SUCCESS;
}

static int vtls_tests(void) {
	return EXIT_SUCCESS;
}

int main(void) {
	int r = 0;

	r = ftls_tests();
	if (!r) r = ctls_tests();
	if (!r) r = vtls_tests();
	return (r);
}
