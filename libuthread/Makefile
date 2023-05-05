## TODO: Phase 1
CC := gcc
CFLAGS := -Wall -Wextra -Werror -MMD
CFLAGS += -g

targets := queue
objs := queue.o

lib := libuthread.a

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

all: $(lib)
deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)
# Avoid builtin rules and variables
MAKEFLAGS += -rR

# Static library target
libuthread.a: $(OBJS)
	ar rcs $@ $^

# Compile source files
$(targets): $(objs)
	@echo "CC $@"
	$(Q) $(CC) $(CFLAGS) -o $@ $<
%.o: %.c
	@echo "CC $@"
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
	
$(lib): $(objs)
	@echo "CC $@"
	$(Q) ar rcs $@ $^
clean:
	@echo "clean"
	$(Q) rm -f $(targets) $(objs) $(deps) $(lib)

