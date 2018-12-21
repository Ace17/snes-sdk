OBJS = crt0_snes.obj libm.obj libtcc.obj libc.obj
C_SRC = malloc.c vsprintf.c ctype.c string.c

%.asm: %.c
	../tcc-65816/816-tcc -O $(CFLAGS) -I. -I./../tcc-65816/include -o $@.tmp -c $<
	sed -i 's/.include "hdr.asm"//' $@.tmp
	mv $@.tmp $@

%.obj: %.asm
	../wla_dx/wla-65816 -io $< $@

all: $(OBJS)

libc.obj: libc.asm libc_c.asm
libc_c.asm: libc_c.c $(C_SRC)
libc_c.c: $(C_SRC)
libm.obj: libm.asm libm_c.asm
clean:
	rm -f $(OBJS) libc_c.asm libm_c.asm
