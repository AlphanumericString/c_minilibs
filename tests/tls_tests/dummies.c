#include <unistd.h>
#include "tls_tests.h"

void dump_mine_node(struct s_mine a) {
	printf("value_a:%d\tvalue_b:%d\n", a.value_a, a.value_b);
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
			a->value_b = rand() % 10;
			if (!a->value_b) a->value_a++;
			dump_mine_node(*a);
		}
		sleep(1);
	}
	return (NULL);
}
