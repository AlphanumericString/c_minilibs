# define variable for all modules dir
.DEFAULT_GOAL	:= all

# list of targets / modules
MODULES		:= da dsu hp ill lru_cache rb sptr tls tpm

# module selector
# when left empty every module is targeted.
MODULE			?=
SELECTED_MODULES:=

MODULE_CL		= $(filter $(MODULES), $(MODULE))

PROJECT_NAME:= c_mini
PRD_TARGET	:= $(PROJECT_NAME).profdata

# DIRS
BUILD_DIR	:= build
DOC_DIR		:= documentation
TST_DIR		:= tests
COV_DIR		:= coverage
SINGLE_DIR	:= single

# TOOLS
CC			:= clang
COV			:= llvm-cov
PRD			:= llvm-profdata
CPPCHECK	:= cppcheck
RM			:= rm -f
PRINT		:= printf

# FLAGS
# Compilation flags
DEBUG_LVL	?= 0
STD			?= gnu17
CHECK_STD	?= c17
CFLAGS		:= -Wall -Wextra -MMD -MP --std=$(STD) -Werror

ifeq ($(DEBUG_LVL), 0)
	CFLAGS += -DC_MINI__DEBUG_LEVEL=0
else
	CFLAGS += -g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=$(DEBUG_LVL)
endif

TFLAGS	?= $(CFLAGS)	-O0 -I./	\
			-fprofile-instr-generate -ftest-coverage -fcoverage-mapping \
			-fno-inline -fno-inline-functions -DC_MINI__TESTS
TLINKER_FLAGS = -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc \
				-Wl,--wrap=clock_gettime
VGFLAGS		?= $(filter-out -fprofile-instr-generate -ftest-coverage \
				-fcoverage-mapping -DC_MINI__DEBUG_LEVEL=0,$(TFLAGS)) \
				-g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=99
# Tools flags
CPPCHECK_LOG_LOC=$(BUILD_DIR)/cppcheck_checks_ran.txt

CHECK_FLAGS	?= --enable=all --std=$(CHECK_STD) --language=c --inline-suppr \
				-I. --suppress=missingIncludeSystem --suppress=unusedFunction \
				--suppress=staticFunction --suppress=variableScope \
				--check-level=exhaustive --quiet \
				--checkers-report=$(CPPCHECK_LOG_LOC)
VFLAGS		:= --leak-check=full --show-leak-kinds=all --track-fds=yes \
				--trace-children=yes --track-origins=yes --read-var-info=yes \
				--show-reachable=yes --undef-value-errors=yes

# FILES COLLECTOR
D_FILES		:=
LIB_FILES	:=
OBJ_FILES	:=
PRD_FILES	:=
TST_PROGS	:=
VG_PROGS	:=

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
# valgrind flavour (no coverage instrumentation)
$1_TOBJVG	:=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/vg/,$$($1_TSRC)))
$1_TOBJSRCVG:=$$(patsubst %.c, %.o, $$(addprefix $(BUILD_DIR)/vg/,$$($1_SRC)))
$1_DVG		:=$$(patsubst %.o, %.d, $$($1_TOBJVG)) $$(patsubst %.o, %.d, $$($1_TOBJSRCVG))
# var(collectors) update
D_FILES		+= $$($1_D) $$($1_DVG)
LIB_FILES	+= lib$1.a
OBJ_FILES	+= $$($1_OBJ)
TST_PROGS	+= test_$1.out
VG_PROGS	+= test_$1.vg.out
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
# static analysis
analyze_$1:
	@mkdir -p $$(dir $(CPPCHECK_LOG_LOC))
	@$(CPPCHECK) $(CHECK_FLAGS) -I./$1 $1
	@$(PRINT) "static analysis: $1 done (checks ran disponible at $(CPPCHECK_LOG_LOC))\n"

# TARGETS
# base target
lib$1.a: $$($1_OBJ)
	ar rcs $$@ $$^
# test target
test_$1.out: $$($1_TOBJSRC) $$($1_TOBJ)
	$(CC) $(TFLAGS) $$($1_TOBJ) $$($1_TOBJSRC) -o $$@ $$(TLINKER_FLAGS)
# valgrind test target
test_$1.vg.out: $$($1_TOBJSRCVG) $$($1_TOBJVG)
	$(CC) $(VGFLAGS) $$($1_TOBJVG) $$($1_TOBJSRCVG) -o $$@ $$(TLINKER_FLAGS)
# cov target
./$(BUILD_DIR)/test_$1.profraw: test_$1.out
	@LLVM_PROFILE_FILE=$$@ ./$$^ 1> /dev/null && \
		$(PRINT) "$$^ ok\n" || $(PRINT) "$$^ failed\n";
endef

$(BUILD_DIR)/vg/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(VGFLAGS) -c $< -o $@

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
		$(addprefix -object=, $(TST_PROGS))
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
# all: compiles every module into it's own lib named lib<module_name>.a (debug via DEBUG_LVL=<value>)
# tests: compiles every module's tests (debug via DEBUG_LVL=<value>)
# tests_run: build and run every module's tests under valgrind
# valgrind: build and run tests under valgrind (module aware via MODULE=<name>)
# static_analysis: run the static analyzer (module aware via MODULE=<name>)
# norm_check: check that every C/H file is clang-format compliant
# single_file: amalgamate MODULE into a single stb style header (MODULE required)
# coverage: run coverage pass using llvm tooling (debug via DEBUG_LVL=<value>)
# documentation: run documentation generation using doxygen
# fclean: rm output dirs and final targets created by makefile
# clean: rm final targets create by makefile
# re: re-compile the project using rule fclean and all
# remove_module: removes a specified module from the project
#
all: $(LIB_FILES)
tests: $(TST_PROGS)
tests_run: $(VG_PROGS)
	@for t in $^; do \
		valgrind $(VFLAGS) -q ./$$t; \
	done
valgrind: $(addprefix test_,$(addsuffix .vg.out,$(MODULE_CL)))
	@for t in $^; do \
		valgrind $(VFLAGS) -q ./$$t; \
	done
static_analysis:
	@if [ -z "$(MODULE_CL)" ] ; then \
		for m in $(MODULES) ; do \
			make --no-print-directory analyze_$$m ; \
		done ; \
	else \
		make --no-print-directory analyze_$(MODULE_CL) ; \
	fi ;
norm_check:
	@clang-format -i $$(fd '.*\.[ch]$$' ./ )
	@sed 's/^ \*\*\//**\//' -i $$(fd '.*\.[ch]$$' ./ )
	@if ! git diff --quiet --diff-filter=ACMR -- '*.c' '*.h' ; then \
		$(PRINT) "norm_checking error: the following files are not clang-format compliant:\n" ; \
		git --no-pager diff --name-only --diff-filter=ACMR -- '*.c' '*.h' ; \
		exit 1 ; \
	fi
	@$(PRINT) "norm_checking: all files are clang-format compliant\n"
single_file:
	@if [ -z "$(MODULE_CL)" ] ; then \
		$(PRINT) '$(RED)Invalid$(RESET). Please use `single_file` rule with a module (eg: `make single_file MODULE=<module_name>`)'; \
		exit 1 ; \
	fi
	@mkdir -p $(SINGLE_DIR)
	@sh scripts/amalgamate.sh $(MODULE_CL) $(SINGLE_DIR)/$(MODULE_CL).h $(MODULE_CL)
	@$(PRINT) "single file: $(SINGLE_DIR)/$(MODULE_CL).h\n"
coverage: $(COV_DIR)/index.html
documentation:$(DOC_DIR)/html/index.html
fclean:clean
	$(RM) -r $(BUILD_DIR) $(DOC_DIR) $(COV_DIR) $(SINGLE_DIR)
clean:
	$(RM) $(LIB_FILES) $(TST_PROGS) $(VG_PROGS) $(PRD_TARGET)
re:fclean
	@make --no-print-directory all
remove_module:
	@if [ -z "$(MODULE_CL)" ] ; then \
		$(PRINT) "$(RED)Invalid$(RESET). Please use \`remove_module\` %s%s\n" \
		"rule with an argument on remove_module (eg: \`make remove_module " \
		"MODULE=<module_name>\`)"; \
		exit 1 ; \
	else \
		sh scripts/remove_module.sh $(MODULE_CL) ./Makefile ; \
		$(PRINT) "removed module: $(MODULE_CL)\n" ; \
	fi
help:
	$(PRINT) "all: compiles every module into it's own lib named lib<module_name>.a (debug via DEBUG_LVL=<value>)"
	$(PRINT) "tests: compiles every module's tests (debug via DEBUG_LVL=<value>)"
	$(PRINT) "tests_run: build and run every module's tests under valgrind"
	$(PRINT) "valgrind: build and run tests under valgrind (module aware via MODULE=<name>)"
	$(PRINT) "static_analysis: run the static analyzer (module aware via MODULE=<name>)"
	$(PRINT) "norm_check: check that every C/H file is clang-format compliant"
	$(PRINT) "single_file: amalgamate MODULE into a single stb style header (via MODULE=<name>)"
	$(PRINT) "coverage: run coverage pass using llvm tooling (debug via DEBUG_LVL=<value>)"
	$(PRINT) "documentation: run documentation generation using doxygen"
	$(PRINT) "fclean: rm output dirs and final targets created by makefile"
	$(PRINT) "clean: rm final targets create by makefile"
	$(PRINT) "re: re-compile the project using rule fclean and all"
	$(PRINT) "remove_module: removes a specified module from the project"
	$(PRINT) "help: print this help message"
.PHONY: re clean fclean tests tests_run valgrind static_analysis norm_check single_file remove_module help
