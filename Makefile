CC      ?= cc
NAME    := mew

CFLAGS  := -std=c11 -Wall -Wextra -Werror -Wshadow -Wswitch-enum \
           -Wformat=2 -Wwrite-strings -Wvla -Wmissing-prototypes \
		   -Iincludes -Isrc
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

SOURCES := $(shell find . -name '*.c' -not -path './build/*' | sed 's|^\./||')
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
PREFIX  ?= /usr/local
BINDIR  := $(PREFIX)/bin
MANDIR  := $(PREFIX)/share/man/man1

install: $(NAME) docs/mew.1
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)
	cp $(NAME) $(DESTDIR)$(BINDIR)/$(NAME)
	chmod 0755 $(DESTDIR)$(BINDIR)/$(NAME)
	cp docs/mew.1 $(DESTDIR)$(MANDIR)/$(NAME).1
	chmod 0644 $(DESTDIR)$(MANDIR)/$(NAME).1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(NAME)
	rm -f $(DESTDIR)$(MANDIR)/$(NAME).1

man:
	@if command -v mandoc >/dev/null 2>&1; then \
		MANWIDTH=80 mandoc docs/mew.1 | col -b; \
	elif man -l docs/mew.1 >/dev/null 2>&1; then \
		MANWIDTH=80 man -l docs/mew.1 | col -b; \
	else \
		echo "no roff renderer found (need mandoc, groff or man-db)"; \
		exit 1; \
	fi

test: $(NAME)
	@bash tests/run_tests.sh
	@sh tests/check_help_sync.sh

.PHONY: all clean fclean re debug release test install uninstall man
