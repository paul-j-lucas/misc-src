BIN=		$(HOME)/bin

CFLAGS=		-O
LDFLAGS=	

RM=		rm -fr

ARGS=		$(BIN)/args
TARGETS=	$(ARGS)

all: $(TARGETS)

$(ARGS): args.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(TARGETS)

# vim:set noet sw=8 ts=8:
