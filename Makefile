# define variable for all modules dir
.DEFAULT_GOAL	:= all

# list of targets / modules
MODULES		:= ill rb da ss dsu tls lru_cache hp sptr tpm
PROJECT_NAME:= c_mini
PRD_TARGET	:= $(PROJECT_NAME).profdata

# DIRS
BUILD_DIR	:= build
DOC_DIR		:= documentation
TST_DIR		:= tests
COV_DIR		:= coverage

# TOOLS
CC		:= clang
COV		:= llvm-cov
PRD		:= llvm-profdata
RM		:= rm -f
PRINT	:= printf
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

# FLAGS
DEBUG_LVL	?= 0
STD			?= gnu17
CFLAGS		:= -Wall -Wextra -MMD -MP --std=$(STD)
VFLAGS		:= --leak-check=full --show-leak-kinds=all --track-fds=yes \
				--trace-children=yes --track-origins=yes --read-var-info=yes \
				--show-reachable=yes --undef-value-errors=yes

ifeq ($(DEBUG_LVL), 0)
	CFLAGS += -DC_MINI__DEBUG_LEVEL=0
else
	CFLAGS += -g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=$(DEBUG_LVL)
endif

TFLAGS	:= $(CFLAGS)	-O0 -I./	\
			-fprofile-instr-generate -ftest-coverage -fcoverage-mapping \
			-fno-inline -fno-inline-functions

# FILES COLLECTOR
D_FILES		:=
LIB_FILES	:=
OBJ_FILES	:=
TST_FILES	:=
PRD_FILES	:=

# MODULE TEMPLATE
#
# module template define
define module_template
# VARS
# declaration
$1_SRC		:=$$(shell find $1 -name '*.c')
$1_TSRC		:=$$(shell find $(TST_DIR)/$1_tests -name '*.c')
$1_OBJ		:=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_SRC)))
$1_TOBJ		:=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/,$$($1_TSRC)))
$1_TOBJSRC	:=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/$(TST_DIR)/,$$($1_SRC)))
$1_D		:=$$(patsubst %.o, %.d, $$($1_OBJ)) $$(patsubst %.o, %.d, $$($1_TOBJ))
# var(collectors) update
D_FILES		+= $$($1_D)
LIB_FILES	+= lib$1.a
OBJ_FILES	+= $$($1_OBJ)
TST_FILES	+= test_$1.out
PRD_FILES	+= ./$(BUILD_DIR)/test_$1.profraw

# RULES
# base rule
$(BUILD_DIR)/$1/%.o: $1/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(CFLAGS) -I. -c $$< -o $$@
# src to test
$(BUILD_DIR)/$(TST_DIR)/$1/%.o: $1/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(TFLAGS) -c $$< -o $$@
# test src to test
$(BUILD_DIR)/$(TST_DIR)/$1_tests/%.o: $(TST_DIR)/$1_tests/%.c
	@mkdir -p $$(dir $$@)
	$(CC) $(TFLAGS) -c $$< -o $$@

# TARGETS
# base target
lib$1.a: $$($1_OBJ)
	ar rcs $$@ $$^
# test target
test_$1.out: $$($1_TOBJSRC) $$($1_TOBJ)
	$(CC) $(TFLAGS) $$($1_TOBJ) $$($1_TOBJSRC) -o $$@
# cov target
./$(BUILD_DIR)/test_$1.profraw: test_$1.out
	@LLVM_PROFILE_FILE=$$@ ./$$^ 1> /dev/null && \
		$(PRINT) "$$^ ok\n" || $(PRINT) "$$^ failed\n";

endef

# module template instentiation
$(foreach mod,$(MODULES),$(eval $(call module_template,$(mod))))

# concrete targets for tasks hierarchy
# coverage
$(PRD_TARGET):$(PRD_FILES)
	@$(PRD) merge -sparse $^ -o $@
$(COV_DIR)/index.html: $(PRD_TARGET)
	@mkdir -p $(COV_DIR)
	@$(COV) show -format=html												\
		-ignore-filename-regex='ss/*' -ignore-filename-regex='$(TST_DIR)/*'	\
		--show-branches=count	--show-directory-coverage					\
		-output-dir=$(COV_DIR)	-instr-profile=$^							\
		$(addprefix -object=, $(TST_FILES))
#		--line-coverage-lt=90 --region-coverage-lt=90

# documentation
$(DOC_DIR)/html/index.html: Doxyfile
	@DOXYGEN_INPUT="$(MODULES)" doxygen $^ 1> /dev/null &&	\
	$(PRINT) "\nDocumentation at file://$(shell pwd)/$@\n"

# auto-recompile on change
-include:$(D_FILES)


# RULES
#
# DOCUMENTATION
# all: compiles every module into it's own lib named lib<module_name>.a
# tests: compiles every module's tests
# tests_run: run every tests (using valgrind with cool args too)
# coverage: run coverage pass using llvm tooling
# documentation: run documentation generation using doxygen
# fclean: rm output dirs and final targets created by makefile
# clean: rm final targets create by makefile
# re: re-compile the project using rule fclean and all
#
all: $(LIB_FILES)
tests: $(TST_FILES)
tests_run: $(TST_FILES)
	@for t in $^; do \
		valgrind $(VFLAGS) -q ./$$t; \
	done
coverage: $(COV_DIR)/index.html
documentation:$(DOC_DIR)/html/index.html
fclean:clean
	$(RM) -r $(BUILD_DIR) $(DOC_DIR) $(COV_DIR)
clean:
	$(RM) $(LIB_FILES) $(TST_FILES) $(PRD_TARGET)
re:fclean
	@make --no-print-directory all
.PHONY: re clean fclean tests tests_run
