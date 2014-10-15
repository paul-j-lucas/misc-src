BIN=		$(HOME)/bin

CFLAGS=		-O
CXXFLAGS=	$(CFLAGS)
LDFLAGS=	

RM=		rm -fr

ARGS=		$(BIN)/args
SIZES=		$(BIN)/sizes
TARGETS=	$(ARGS) $(SIZES)

all: $(TARGETS)

$(ARGS): args.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(SIZES): sizes.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(TARGETS)

# vim:set noet sw=8 ts=8:
