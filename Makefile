# Configuration
PREFIX?=/usr/local

# Outputs
BIN=example
LIB=libpushfilter.a
HDR=pushfilter.h

# Intermediates
BINOBJ=example.o
LIBOBJ=pushfilter.o

# Rules
all: $(LIB) $(BIN)

$(BIN): $(BINOBJ) $(LIB)
	$(CC) $(LDFLAGS) -o $@ demofilter.o libpushfilter.a

$(LIB): $(LIBOBJ)
	ar rcs $@ pushfilter.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(LIB) $(BIN) $(LIBOBJ) $(BINOBJ)

install:
	# Don't want to install the demo binary.
	mkdir -p $(DESTDIR)/$(PREFIX)/lib
	install $(LIB) $(DESTDIR)/$(PREFIX)/lib
	mkdir -p $(DESTDIR)/$(PREFIX)/include
	install $(HDR) $(DESTDIR)/$(PREFIX)/lib
