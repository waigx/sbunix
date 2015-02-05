CC=gcc
AS=as
CFLAGS=-O1 -std=c99 -Wall -Werror -nostdinc -Iinclude -msoft-float -mno-sse -mno-red-zone -fno-builtin -fPIC -march=amdfam10 -g3
LD=gcc
LDLAGS=
AR=ar

ROOTFS=rootfs
ROOTBIN=$(ROOTFS)/bin

BIN_SRCS:=$(wildcard bin/*/*.c)
BINS:=$(addprefix $(ROOTFS)/,$(wildcard bin/*))

.PHONY: all binary

all: $(BINS)

$(BINS): $(shell find bin/ -type f -name *.c) $(wildcard include/*.h include/*/*.h) libstr.o libcommon.o libio.o
	@$(MAKE) --no-print-directory BIN=$@ binary
	@cp -r etc $(ROOTFS)

binary: $(patsubst %.c,obj/%.o,$(wildcard $(BIN:rootfs/%=%)/*.c))
	$(LD) $(LDLAGS) -o $(BIN) $^ obj/lib/*.o

obj/%.o: %.c $(wildcard include/*.h include/*/*.h)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

libstr.o: lib/libstr.c
	@mkdir -p obj/lib
	$(CC) -c $(CFLAGS) -o obj/lib/libstr.o lib/libstr.c

libio.o: lib/libio.c
	@mkdir -p obj/lib
	$(CC) -c $(CFLAGS) -o obj/lib/libio.o lib/libio.c

libcommon.o: lib/libcommon.c
	@mkdir -p obj/lib
	$(CC) -c $(CFLAGS) -o obj/lib/libcommon.o lib/libcommon.c


.PHONY: submit clean

SUBMITTO:=~mferdman/cse506-submit/

submit: clean
	tar -czvf $(USER).tgz --exclude=.empty --exclude=.*.sw? --exclude=*~ LICENSE README Makefile linker.script sys bin crt libc newfs ld include $(ROOTFS) $(USER).img
	@gpg --quiet --import cse506-pubkey.txt
	gpg --yes --encrypt --recipient 'CSE506' $(USER).tgz
	rm -fv $(SUBMITTO)$(USER)=*.tgz.gpg
	cp -v $(USER).tgz.gpg $(SUBMITTO)$(USER)=`date +%F=%T`.tgz.gpg

clean:
	find $(ROOTBIN) -type f ! -name .empty -print -delete
	rm -rfv obj kernel newfs.506 $(ROOTBOOT)/kernel/kernel
