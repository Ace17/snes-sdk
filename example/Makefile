PREFIX=/tmp/snes-sdk
OPTIMIZE=1

BIN?=bin

CFLAGS=-Wall
ifeq ($(OPTIMIZE),1)
CFLAGS += -O
endif

BINDIR=$(PREFIX)/bin
AS=$(BINDIR)/wla-65816
LD=$(BINDIR)/wlalink
CC=$(BINDIR)/816-tcc

LIBDIR=$(PREFIX)/lib

ASMOBJ = $(BIN)/data.obj
COBJ = $(BIN)/snesc.obj $(BIN)/input.obj $(BIN)/init.obj $(BIN)/graph.obj $(BIN)/str.obj

all: $(BIN)/snesc.smc

$(BIN)/snesc.smc: $(ASMOBJ) $(COBJ)
	@mkdir -p $(dir $@)
	$(LD) -dvSo $(ASMOBJ) $(COBJ) "$@"

$(BIN)/%.s: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I. -o $@ -c $<
	@gcc -MM -MT "$@" -I. -o "$(BIN)/$*.dep" -c $<

$(BIN)/%.obj: $(BIN)/%.s
	@mkdir -p $(dir $@)
	$(AS) -io $< $@

$(BIN)/%.obj: %.asm
	@mkdir -p $(dir $@)
	$(AS) -io $< $@

clean:
	rm -rf $(BIN)

-include $(shell test -d $(BIN) && find $(BIN) -name "*.dep")
