ILL_SRC	=	$(wildcard ill/*.c)
RB_SRC	=	$(wildcard rb/*.c)
DA_SRC	=	$(wildcard da/*.c)
SS_SRC	=	$(wildcard ss/*.c)
DSU_SRC	=	$(wildcard dsu/*.c)
BUILD_DIR=build
OBJ_DA	=	$(patsubst %.c, %.o, $(addprefix $(BUILD_DIR)/,$(DA_SRC)))
OBJ_RB	=	$(patsubst %.c, %.o, $(addprefix $(BUILD_DIR)/,$(RB_SRC)))
OBJ_SS	=	$(patsubst %.c, %.o, $(addprefix $(BUILD_DIR)/,$(SS_SRC)))
OBJ_DSU	=	$(patsubst %.c, %.o, $(addprefix $(BUILD_DIR)/,$(DSU_SRC)))
OBJ_ILL	=	$(patsubst %.c, %.o, $(addprefix $(BUILD_DIR)/,$(ILL_SRC)))
D_FILES	=	$(patsubst %.o, %.d, $(OBJ_ILL) $(OBJ_DSU) $(OBJ_RB) $(OBJ_DA) $(OBJ_SS))

CC	=	clang
CFLAGS = -Wall -Wextra -g3 -MMD -MP

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all:libdynamic_array.a libround_buffer.a libdisjointed_array_uninon.a libsparse_set.a libintrusive_lists.a
tests:rb_test.out da_test.out dsu_test.out ss_test.out ill_test.out

libdisjointed_array_uninon.a: $(OBJ_DSU)
	$(AR) rcs $@ $(OBJ_DSU)
libdynamic_array.a: $(OBJ_DA)
	$(AR) rcs $@ $(OBJ_DA)
libround_buffer.a: $(OBJ_RB)
	$(AR) rcs $@ $(OBJ_RB)
libintrusive_lists.a:$(OBJ_ILL)
	$(AR) rcs $@ $(OBJ_ILL)
libsparse_set.a:$(OBJ_SS)
	$(AR) rcs $@ $(OBJ_SS)

ill_test.out:libintrusive_lists.a test_main_ill.c
	$(CC) $(CFLAGS) -I./rb test_main_ill.c -L ./ -lintrusive_lists -o $@
rb_test.out:libround_buffer.a test_main_rb.c
	$(CC) $(CFLAGS) -I./rb test_main_rb.c -L ./ -lround_buffer -o $@
da_test.out:libdynamic_array.a test_main_da.c
	$(CC) $(CFLAGS) -I./da test_main_da.c -L ./ -ldynamic_array -o $@
dsu_test.out:libdisjointed_array_uninon.a test_main_dsu.c
	$(CC) $(CFLAGS) -I./dsu test_main_dsu.c -L ./ -ldisjointed_array_uninon -o $@
ss_test.out:libsparse_set.a test_main_ss.c
	$(CC) $(CFLAGS) -I./ss test_main_ss.c -L ./ -lsparse_set -o $@

fclean:clean
	$(RM) $(OBJ_DA) $(OBJ_RB) $(OBJ_DSU) $(D_FILES)
clean:
	$(RM) ./*.out lib*.a ./*.d
-include:$(D_FILES) rb_test.d dsu_test.d da_test.d ss_test.d ill_test.d
re:fclean tests
