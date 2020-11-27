CC:=x86_64-os-gcc
CFLAGS:=-Wall -Wextra -Werror -O2 -MMD -Iinclude

CFILES:=main.c
OFILES:=$(patsubst %.c, %.o, $(CFILES))

all: init
init: $(OFILES)
	$(CC) -o init $(OFILES)
install: all
	mkdir -p $(DESTDIR)/boot
	cp init $(DESTDIR)/boot
format:
	clang-format -i $(CFILES) $(wildcard include/*.h)
clean:
	rm -f $(OFILES) $(OFILES:.o=.d) init
-include $(OFILES:.o=.d)
