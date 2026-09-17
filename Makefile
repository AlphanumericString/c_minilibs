# define variable for all modules dir
.DEFAULT_GOAL	:= all

MODULES			= ill rb da ss dsu tls lru_cache hp sptr

BUILD_DIR		= build
DOC_DIR			= documentation
TST_DIR			= tests
COV_DIR			= coverage
D_FILES			=

CC				= clang
COV				= llvm-cov
PRD				= llvm-profdata

DEBUG_LVL		?= 0
STD				= gnu17
CFLAGS			= -Wall -Wextra -MMD -MP --std=$(STD)

ifeq ($(DEBUG_LVL), 0)
	CFLAGS := $(CFLAGS) -DC_MINI__DEBUG_LEVEL=0
else
	CFLAGS += -g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=$(DEBUG_LVL)
endif

# Check for llvm-cov and llvm-profdata
#   If not found, use the version 12 if available
#   If not found, use the version 14 (latest version)
ifeq (, $(shell which $(COV) 2> /dev/null))
	COV			=	llvm-cov-12
	ifeq (, $(shell which $(COV) 2> /dev/null))
		COV			=	llvm-cov-14
	endif
endif

ifeq (, $(shell which $(PRD) 2> /dev/null))
	PRD			=	llvm-profdata-12
	ifeq (, $(shell which $(PRD) 2> /dev/null))
		PRD			=	llvm-profdata-14
	endif
endif

TFLAGS=
LIBNAMES=
OBJ_ALL=
TESTS_ALL=
VALGRIND_OPT= --leak-check=full --show-leak-kinds=all --track-fds=yes \
        --trace-children=yes --track-origins=yes --read-var-info=yes \
        --show-reachable=yes --undef-value-errors=yes

TFLAGS	=\
			-fprofile-instr-generate -ftest-coverage -fcoverage-mapping	\
			-fno-inline -fno-inline-functions							\
			-O0 -I./

## tests

define module_template
# module vars
$1_SRC=	$$(shell find $1 -name '*.c')
$1_TSRC=	$$(shell find $(TST_DIR)/$1_tests -name '*.c')
$1_OBJ=	$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_SRC)))
$1_TOBJ=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_TSRC)))
$1_TOBJSRC=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/$(TST_DIR)/,$$($1_SRC)))
$1_D=	$$(patsubst %.o, %.d, $$($1_OBJ)) \
		$$(patsubst %.o, %.d, $$($1_TOBJ))
# add module to glob vars
D_FILES	+= $$($1_D)
LIBNAMES += lib$1.a
OBJ_ALL += $$($1_OBJ)
TESTS_ALL += test_$1.out
# rules
# define base rule to compile
$(BUILD_DIR)/$1/%.o: $1/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) -I. -c $$< -o $$@
# define rule to compile base as test
$(BUILD_DIR)/$(TST_DIR)/$1/%.o: $1/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) $(TFLAGS) -c $$< -o $$@
# define rule to compile test as test
$(BUILD_DIR)/$(TST_DIR)/$1_tests/%.o: $(TST_DIR)/$1_tests/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) $(TFLAGS) -I. -c $$< -o $$@
# final targets
# module base target
lib$1.a: $$($1_OBJ)
	ar rcs $$@ $$^
# module test target
test_$1.out: $$($1_TOBJSRC) $$($1_TOBJ)
	$(CC) $(TFLAGS) $(CFLAGS) $$($1_TOBJ) $$($1_TOBJSRC) -o $$@
endef

$(foreach mod,$(MODULES),$(eval $(call module_template,$(mod))))

all: $(LIBNAMES)
tests: $(TESTS_ALL)
tests_run: $(TESTS_ALL)
	@for t in $^; do \
		valgrind $(VALGRIND_OPT) -q ./$$t; \
	done
$(COV_DIR): $(TESTS_ALL)
	@mkdir -p $(COV_DIR)
	@for t in $^; do														\
		LLVM_PROFILE_FILE=./$(BUILD_DIR)/$$t.profraw ./$$t 1> /dev/null ;	\
		done
	@$(PRD) merge -sparse													\
		$(addsuffix .profraw, $(addprefix ./$(BUILD_DIR)/, $(TESTS_ALL)))	\
		-o merged.profdata
	@$(COV) show -format=html									\
		-ignore-filename-regex=$(TST_DIR)/*							\
		--show-branches=count	--show-directory-coverage			\
		-output-dir=$(COV_DIR)										\
		-instr-profile=merged.profdata $(addprefix -object=, $(TESTS_ALL))
	@rm -rf merged.profdata

-include:$(D_FILES)
fclean:clean
	$(RM) -rf $(BUILD_DIR) $(DOC_DIR) $(COV_DIR)
$(DOC_DIR):
	@DOXYGEN_INPUT="$(MODULES)" doxygen Doxyfile 1> /dev/null &&	\
	echo -e "\nDocumentation at file:///$(shell pwd)/$(DOC_DIR)/html/index.html\n"
clean:
	$(RM) $(LIBNAMES) $(TESTS_ALL)
re:fclean
	@make --no-print-directory all
.PHONY: re clean fclean $(COV_DIR)
