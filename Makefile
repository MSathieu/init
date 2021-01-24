CC:=x86_64-os-gcc
CFLAGS:=-Wall -Wextra -Werror -O2

CFILES:=main.c
OFILES:=$(patsubst %.c, %.o, $(CFILES))

all: init
init: $(OFILES)
	$(CC) -s -o init $(OFILES)
install: all
	mkdir -p $(DESTDIR_BOOT)/boot
	cp init $(DESTDIR_BOOT)/boot
format:
	clang-format -i $(CFILES)
clean:
	rm -f $(OFILES) init
