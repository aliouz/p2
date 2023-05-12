## TODO: Phase 1
lib := libuthread.a
CC := gcc
CFLAGS := -Wall -Wextra -Werror -MMD
CFLAGS += -g
target := queue.o uthread.o context.o sem.o preempt.o

ifneq ($(V),1)
Q = @
V = 0
endif

deps := $(patsubst %.o,%.d,$(target))
-include $(deps)

all: $(lib)

$(lib): $(target)
	ar rcs $(lib) $(target)

%.o: %.c
	@echo "CC $@"
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	@echo "clean"
		rm -rf $(target) $(lib) $(deps)

