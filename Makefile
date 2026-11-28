# define variable for all modules dir
.DEFAULT_GOAL	:= all

# =============================================================================
# CONST VARIABLES
# Owned by the Makefile: not meant to be overridden from the environment or
# the command line. Use the "USER VARIABLES" section below for configuration.
# =============================================================================

# main vars
override PROJECT_NAME	:= c_mini

## list of targets / modules
override MODULES		:= da dsu hp ill lru_cache rb sptr tls tpm

## TOOLS
override CC				:= clang
override AR				:= ar
override COV			:= llvm-cov
override PRD			:= llvm-profdata
override CPPCHECK		:= cppcheck
override DOXYGEN		:= doxygen
override CLANG_FORMAT	:= clang-format
override RM				:= rm -f
override PRINT			:= printf

## DIRS
override BUILD_DIR		:= build
override DOC_DIR		:= documentation
override TST_DIR		:= tests
override COV_DIR		:= coverage
override SINGLE_DIR		:= single

## generated artifacts
override PRD_TARGET			:= $(PROJECT_NAME).profdata
override CPPCHECK_LOG_LOC	:= $(BUILD_DIR)/cppcheck_checks_ran.txt
override DOXYCHECK_LOG_LOC	:= $(BUILD_DIR)/doxygen_check.log

## fixed linker flags
override TLINKER_FLAGS	:= \
		-Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc \
		-Wl,--wrap=clock_gettime -Wl,--wrap=pthread_create \
		-Wl,--wrap=pthread_cond_init

## fixed valgrind flags
override VFLAGS		:= \
		--leak-check=full --show-leak-kinds=all --track-fds=yes \
		--trace-children=yes --track-origins=yes --read-var-info=yes \
		--show-reachable=yes --undef-value-errors=yes

## TEXT
### FG COLORS
override FG_BLACK		:= \033[0;30m
override FG_RED			:= \033[0;31m
override FG_GREEN		:= \033[0;32m
override FG_YELLOW		:= \033[0;33m
override FG_BLUE		:= \033[0;34m
override FG_PURPLE		:= \033[0;35m
override FG_CYAN		:= \033[0;36m
override FG_WHITE		:= \033[0;37m

### BG COLORS
override BG_BLACK		:= \033[40m
override BG_RED			:= \033[41m
override BG_GREEN		:= \033[42m
override BG_YELLOW		:= \033[43m
override BG_BLUE		:= \033[44m
override BG_PURPLE		:= \033[45m
override BG_CYAN		:= \033[46m
override BG_WHITE		:= \033[47m

### EFFECT
override TXT_BOLD		:= \033[1m
override TXT_DIM		:= \033[2m
override TXT_UNDERLINE	:= \033[4m
override TXT_BLINK		:= \033[5m
override TXT_REVERSE	:= \033[7m
override TXT_HIDDEN		:= \033[8m

override TXT_RESET		:= \033[0m

### MESSAGE STYLES
override DEFAULT		:= $(TXT_RESET)
override INFO			:= $(TXT_RESET)$(FG_YELLOW)
override INFO_IMPORTANT	:= $(TXT_RESET)$(FG_YELLOW)$(TXT_UNDERLINE)
override SUCCESS		:= $(TXT_RESET)$(FG_GREEN)
override FAIL			:= $(TXT_RESET)$(FG_RED)
override CRITICAL_FAIL	:= $(TXT_RESET)$(FG_RED)$(TXT_UNDERLINE)

# =============================================================================
# USER VARIABLES
# Can be overridden from the environment or the command line.
# =============================================================================

## module selector (when left empty, every module is targeted)
MODULE		?=

# Compilation flags
DEBUG_LVL	?= 0
STD			?= gnu17
CHECK_STD	?= c17
CFLAGS		:= -Wall -Wextra -MMD -MP --std=$(STD) -Werror

ifeq ($(DEBUG_LVL), 0)
	CFLAGS += -DC_MINI__DEBUG_LEVEL=0 -O2
else
	CFLAGS += -g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=$(DEBUG_LVL)
endif

# tests deliberately hand uninitialized payloads to the *_init() wrappers, so
# silence clang's const-pointer false positive for this build only.
TFLAGS	?= $(filter-out -O2 -O3 -O1, $(CFLAGS))	-O0 -I./	\
			-fprofile-instr-generate -ftest-coverage -fcoverage-mapping \
			-fno-inline -fno-inline-functions -DC_MINI__TESTS		\
			-Wno-uninitialized-const-pointer
# optional compile-time path: set to 1 to build the da tests with auto-shrink on
DA_AUTO_SHRINK	?= 0
ifeq ($(DA_AUTO_SHRINK), 1)
	TFLAGS += -DC_MINI__DA_AUTO_SHRINK
endif
VGFLAGS		?= $(filter-out -fprofile-instr-generate -ftest-coverage \
				-fcoverage-mapping -DC_MINI__DEBUG_LEVEL=0,$(TFLAGS)) \
				-g3 -DC_MINI__TESTS -DC_MINI__DEBUG_LEVEL=99
## per-test valgrind timeout, prevents a stuck threaded test from hanging CI
VTIMEOUT	?= 10m
# Tools flags
CHECK_FLAGS	?= --enable=all --std=$(CHECK_STD) --language=c --inline-suppr \
				-I. --suppress=missingIncludeSystem --suppress=unusedFunction \
				--suppress=staticFunction --suppress=variableScope \
				--check-level=exhaustive --quiet \
				--checkers-report=$(CPPCHECK_LOG_LOC)

# =============================================================================
# DERIVED VARIABLES
# =============================================================================

## selected modules (fallback to every module when MODULE is empty)
override MODULE_CL			:= $(filter $(MODULES),$(MODULE))
override TARGET_MODULES		:= $(if $(strip $(MODULE_CL)),$(MODULE_CL),$(MODULES))
override TARGET_LIBS		:= $(addprefix lib,$(addsuffix .a,$(TARGET_MODULES)))
override TARGET_TESTS		:= $(addprefix test_,$(addsuffix .out,$(TARGET_MODULES)))
override TARGET_VG_TESTS	:= $(addprefix test_,$(addsuffix .vg.out,$(TARGET_MODULES)))

ifneq ($(strip $(MODULE)),)
ifeq ($(strip $(MODULE_CL)),)
$(error Unknown module(s) in MODULE="$(MODULE)". Valid modules: $(MODULES))
endif
endif

# FILES COLLECTOR
D_FILES		:=
LIB_FILES	:=
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
	@$(PRINT) "$(SUCCESS)static analysis$(DEFAULT): %s done (checks available at $(CPPCHECK_LOG_LOC))$(TXT_RESET)\n" "$1"

# TARGETS
# base target
lib$1.a: $$($1_OBJ)
	$(AR) rcs $$@ $$^
# test target
test_$1.out: $$($1_TOBJSRC) $$($1_TOBJ)
	$(CC) $(TFLAGS) $$($1_TOBJ) $$($1_TOBJSRC) -o $$@ $$(TLINKER_FLAGS)
# valgrind test target
test_$1.vg.out: $$($1_TOBJSRCVG) $$($1_TOBJVG)
	$(CC) $(VGFLAGS) $$($1_TOBJVG) $$($1_TOBJSRCVG) -o $$@ $$(TLINKER_FLAGS)
# cov target
./$(BUILD_DIR)/test_$1.profraw: test_$1.out
	@LLVM_PROFILE_FILE=$$@ ./$$^ 1> /dev/null && \
		$(PRINT) "$(SUCCESS)%s ok$(TXT_RESET)\n" "$$^" || \
		$(PRINT) "$(FAIL)%s failed$(TXT_RESET)\n" "$$^";
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
	@DOXYGEN_INPUT="$(MODULES)" $(DOXYGEN) $^ 1> /dev/null &&	\
	$(PRINT) "$(INFO)documentation$(DEFAULT): file://$(shell pwd)/$@$(TXT_RESET)\n"

# auto-recompile on change
-include:$(D_FILES)


# The `help` rule reads the documentation block below.
# Add one `<target>: <description>` line per target; no help rule change needed.
#
# RULES DOCUMENTATION
#
# all: compiles every module into it's own lib named lib<module_name>.a (debug via DEBUG_LVL=<value>)
# tests: compiles every module's tests (debug via DEBUG_LVL=<value>)
# module: compiles selected module's lib (defaults to every module via MODULE=<name>)
# test_module: compiles selected module's tests (defaults to every module via MODULE=<name>)
# tests_run: build and run selected modules' tests under valgrind (defaults to every module)
# valgrind: build and run selected modules' tests under valgrind (defaults to every module)
# static_analysis: run the static analyzer (defaults to every module via MODULE=<name>)
# norm_check: check that every C/H file is clang-format compliant (file aware via FILES=<name>)
# doxygen_check: fail on doxygen warnings/errors (defaults to every module, module aware via MODULE=<name>, file aware via FILES=<name>)
# single_file: amalgamate selected modules into single stb style headers (defaults to every module)
# coverage: run coverage pass using llvm tooling (debug via DEBUG_LVL=<value>)
# documentation: run documentation generation using doxygen
# examples: compile examples for selected modules (defaults to every module)
# fclean: rm output dirs and final targets created by makefile
# clean: rm final targets create by makefile
# re: re-compile the project using rule fclean and all
# remove_module: removes explicit modules from the project (MODULE required)
# help: print this help message

all:$(LIB_FILES)
tests:$(LIB_FILES)
module: $(TARGET_LIBS)
test_module: $(TARGET_TESTS)
tests_run: $(TARGET_VG_TESTS)
	@for t in $^; do \
		timeout $(VTIMEOUT) valgrind $(VFLAGS) -q ./$$t; \
	done
valgrind: $(TARGET_VG_TESTS)
	@for t in $^; do \
		timeout $(VTIMEOUT) valgrind $(VFLAGS) -q ./$$t; \
	done
static_analysis:
	@for m in $(TARGET_MODULES) ; do \
		$(MAKE) --no-print-directory analyze_$$m ; \
	done ;
norm_check:
	@files="$(if $(strip $(FILES)),$(FILES),$$(fd '.*\.[ch]$$' ./ ))" ; \
	if ! bad=$$(sh scripts/norm_check.sh "$(CLANG_FORMAT)" $$files) ; then \
		$(PRINT) "$(FAIL)norm_checking error$(DEFAULT): the following files are not clang-format compliant:$(TXT_RESET)\n" ; \
		printf '%s\n' "$$bad" ; \
		exit 1 ; \
	fi
	@$(PRINT) "$(SUCCESS)norm_checking$(DEFAULT): all files are clang-format compliant$(TXT_RESET)\n"
doxygen_check:
	@inputs="$(strip $(FILES))" ; \
	if [ -z "$$inputs" ] ; then \
		inputs="$(TARGET_MODULES)" ; \
	fi ; \
	mkdir -p $(dir $(DOXYCHECK_LOG_LOC)) $(BUILD_DIR)/doxygen_check ; \
	$(PRINT) "$(INFO)doxygen_check$(DEFAULT): checking [$$inputs] for documentation errors$(TXT_RESET)\n" ; \
	{ cat Doxyfile ; \
		$(PRINT) 'WARN_AS_ERROR = FAIL_ON_WARNINGS\n' ; \
		$(PRINT) 'INPUT = %s\n' "$$inputs" ; \
		$(PRINT) 'OUTPUT_DIRECTORY = $(BUILD_DIR)/doxygen_check\n' ; \
		$(PRINT) 'QUIET = YES\n' ; \
	} | $(DOXYGEN) - > $(DOXYCHECK_LOG_LOC) 2>&1 ; \
	if [ $$? -ne 0 ] ; then \
		$(PRINT) "$(FAIL)doxygen_check error$(DEFAULT): documentation issues found:$(TXT_RESET)\n" ; \
		grep -E 'error:|warning:' $(DOXYCHECK_LOG_LOC) || cat $(DOXYCHECK_LOG_LOC) ; \
		exit 1 ; \
	fi ; \
	$(PRINT) "$(SUCCESS)doxygen_check$(DEFAULT): no documentation errors found$(TXT_RESET)\n"
single_file:
	@mkdir -p $(SINGLE_DIR)
	@for m in $(TARGET_MODULES) ; do \
		sh scripts/amalgamate.sh $$m $(SINGLE_DIR)/$$m.h $$m ; \
		$(PRINT) "$(SUCCESS)single file$(DEFAULT): $(SINGLE_DIR)/%s.h$(TXT_RESET)\n" "$$m" ; \
	done
coverage: $(COV_DIR)/index.html
documentation:$(DOC_DIR)/html/index.html
examples: $(TARGET_LIBS)
	@mkdir -p $(BUILD_DIR)
	@for m in $(TARGET_MODULES) ; do \
		for e in $$(find examples/$$m/*.c) ; do \
			$(CC) -Wall -Wextra -I. $$e \
				-o $(BUILD_DIR)/"$${m}_$$(basename $$e).example.out" \
				-L. -l:lib$$m.a ; \
			$(PRINT) "$(SUCCESS)made$(DEFAULT): $(BUILD_DIR)/%s.example.out$(TXT_RESET)\n" "$${m}_$$(basename $$e)" ; \
		done; \
	done;
fclean:clean
	$(RM) -r $(BUILD_DIR) $(DOC_DIR) $(COV_DIR) $(SINGLE_DIR)
clean:
	$(RM) $(LIB_FILES) $(TST_PROGS) $(VG_PROGS) $(PRD_TARGET)
re:fclean
	@$(MAKE) --no-print-directory all
remove_module:
	@if [ -z "$(MODULE_CL)" ] ; then \
		$(PRINT) '$(FAIL)Invalid$(TXT_RESET). Please use `remove_module` with an explicit module (eg: `make remove_module "MODULE=module1[ module2...]"`)\n'; \
		exit 1 ; \
	else \
		for m in $(MODULE_CL) ; do \
			sh scripts/remove_module.sh $$m ./Makefile ; \
			$(PRINT) "$(SUCCESS)removed module$(DEFAULT): %s$(TXT_RESET)\n" "$$m" ; \
		done ; \
	fi
help:
	@sed -n '/^# RULES DOCUMENTATION$$/,/^[^#]/p' $(firstword $(MAKEFILE_LIST)) \
		| sed -n 's/^# \([A-Za-z0-9_.-]*:\)/\1/p' \
		| while IFS= read -r line ; do \
			entry=$${line%%:*} ; \
			desc=$${line#*:} ; \
			desc=$${desc# } ; \
			$(PRINT) "$(INFO_IMPORTANT)%s$(DEFAULT): %s$(TXT_RESET)\n" "$$entry" "$$desc" ; \
		done
.PHONY: all re clean fclean module test_module tests tests_run valgrind static_analysis norm_check doxygen_check single_file remove_module help examples coverage documentation $(addprefix analyze_,$(MODULES))
