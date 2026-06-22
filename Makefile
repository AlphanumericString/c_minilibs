# define variable for all modules dir
MODULES =	ill rb da ss dsu tls
D_FILES=
.DEFAULT_GOAL := all
BUILD_DIR=build
DOC_DIR=documentation
TST_DIR=tests
CC	=	clang
CFLAGS = -Wall -Wextra -MMD -MP -g3
TFLAGS = -g3
LIBNAMES=
OBJ_ALL=
TESTS_ALL=

define module_template
$1_SRC=	$$(shell find $1 -name '*.c')
$1_TSRC=	$$(shell find $(TST_DIR)/$1_tests -name '*.c')
$1_OBJ=	$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_SRC)))
$1_TOBJ=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_TSRC)))
$1_D=	$$(patsubst %.o, %.d, $$($1_OBJ)) \
		$$(patsubst %.o, %.d, $$($1_TOBJ))
lib$1.a: $$($1_OBJ)
	ar rcs $$@ $$^
test_$1.out: lib$1.a $$($1_TOBJ)
	$(CC) $(CFLAGS) $$($1_TOBJ) -o $$@ -L. -l$1
D_FILES	+= $$($1_D)
LIBNAMES += lib$1.a
OBJ_ALL += $$($1_OBJ)
TESTS_ALL += test_$1.out
$(BUILD_DIR)/$1/%.o: $1/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) -c $$< -o $$@
$(BUILD_DIR)/$(TST_DIR)/$1_tests/%.o: $(TST_DIR)/$1_tests/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) $(TFLAGS) -I. -c $$< -o $$@
endef

$(foreach mod,$(MODULES),$(eval $(call module_template,$(mod))))

all: $(LIBNAMES)
tests: $(TESTS_ALL)
tests_run: $(TESTS_ALL)
	@for t in $^; do \
		./$$t; \
	done

-include:$(D_FILES)
fclean:clean
	$(RM) -rf $(BUILD_DIR) $(DOC_DIR)
$(DOC_DIR):
	DOXYGEN_INPUT="$(MODULES)" doxygen Doxyfile &&	\
	echo -e "\n\nDocumentation at file:///$(shell pwd)/$(DOC_DIR)/html/index.html\n"
clean:
	$(RM) $(LIBNAMES) $(TESTS_ALL)
re:fclean
.PHONY: re clean fclean
