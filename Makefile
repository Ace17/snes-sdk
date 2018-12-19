PREFIX = /tmp/snes-sdk
_OS = $(shell uname -s)

SUBDIRS = wla_dx wla_dx/wlalink wla_dx/wlab tcc-65816 libs

all: $(SUBDIRS)

.PHONY: dummy $(SUBDIRS)
libs wla_dx wla_dx/wlalink wla_dx/wlab: dummy
	$(MAKE) -C $@ PREFIX=$(PREFIX)

tcc-65816: tcc-65816/config.h
	$(MAKE) -C $@ 816-tcc

tcc-65816/config.h: Makefile
	cd tcc-65816 && ./configure --prefix=$(PREFIX) --enable-cross

libs: wla_dx tcc-65816

clean:
	(for i in $(SUBDIRS) ; do $(MAKE) -C $$i clean || true ; done)

install:
	mkdir -p $(DESTDIR)$(PREFIX)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -p libs/*.obj $(DESTDIR)$(PREFIX)/lib/
	cp -p libs/hdr.asm $(DESTDIR)$(PREFIX)/include/
	cp -p tcc-65816/include/* $(DESTDIR)$(PREFIX)/include/
	cp -p tcc-65816/816-tcc $(DESTDIR)$(PREFIX)/bin/
	install -m 755 tcc-65816/816-opt.py $(DESTDIR)$(PREFIX)/bin/816-opt
	cp -p wla_dx/wla-65816 $(DESTDIR)$(PREFIX)/bin/
	cp -p wla_dx/wlalink/wlalink $(DESTDIR)$(PREFIX)/bin/
	cp -p wla_dx/wlab/wlab $(DESTDIR)$(PREFIX)/bin/
