CC      ?= cc
NAME    := mew

CFLAGS  := -std=c11 -Wall -Wextra -Werror -Wshadow -Wswitch-enum \
           -Wformat=2 -Wwrite-strings -Wvla -Wmissing-prototypes
DEBUG   := -g3 -gdwarf-4 -O0 -fno-omit-frame-pointer \
           -fsanitize=address,undefined -fno-sanitize-recover=all
RELEASE := -O2 -DNDEBUG

ifeq ($(filter release,$(MAKECMDGOALS)),release)
  BUILD   := release
  CFLAGS  += $(RELEASE)
else
  BUILD   := debug
  CFLAGS  += $(DEBUG)
  LDFLAGS += -fsanitize=address,undefined
endif

SOURCES := $(shell find . -name '*.c' -not -path './build/*')
OBJDIR  := build/$(BUILD)
OBJECTS := $(SOURCES:%.c=$(OBJDIR)/%.o)
DEPS    := $(OBJECTS:.o=.d)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -rf build
fclean: clean
	rm -f $(NAME)
re: fclean all
debug: $(NAME)
release: $(NAME)

.PHONY: all clean fclean re debug release
