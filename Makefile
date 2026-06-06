# define variable for all modules dir
MODULES =	ill rb da ss dsu tls
D_FILES=
.DEFAULT_GOAL := all
BUILD_DIR=build
DOC_DIR=documentation
CC	=	clang
CFLAGS = -Wall -Wextra -g3 -MMD -MP
LIBNAMES=
OBJ_ALL=
TESTS_ALL=

define module_template
$1_SRC=	$(wildcard $1/*.c)
$1_OBJ=	$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_SRC)))
$1_D=	$$(patsubst %.o, %.d, $$($1_OBJ))
lib$1.a: $$($1_OBJ)
	ar rcs $$@ $$^
test_$1.out: lib$1.a test_main_$1.c
	$(CC) $(CFLAGS) test_main_$1.c -o $$@ -L. -l$1
D_FILES	+= $$($1_D)
LIBNAMES += lib$1.a
OBJ_ALL += $$($1_OBJ)
TESTS_ALL += test_$1.out
$(BUILD_DIR)/$1/%.o: $1/%.c
	@mkdir -p $(BUILD_DIR)/$1
	$(CC) $(CFLAGS) -c $$< -o $$@
endef

$(foreach mod,$(MODULES),$(eval $(call module_template,$(mod))))

all: $(LIBNAMES)
tests: $(TESTS_ALL)
	@for t in $^; do \
		./$$t; \
	done
-include:$(D_FILES)
fclean:clean
	$(RM) $(OBJ_ALL) $(D_FILES) ./*.d
	$(RM) -rf $(BUILD_DIR) $(DOC_DIR)
$(DOC_DIR):
	DOXYGEN_INPUT="$(MODULES)" doxygen Doxyfile &&	\
	echo -e "\n\nDocumentation at file:///$(shell pwd)/$(DOC_DIR)/html/index.html\n"
clean:
	$(RM) $(LIBNAMES) $(TESTS_ALL)
re:fclean
.PHONY: re clean fclean
