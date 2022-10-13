SOURCEFILES=$(wildcard *.c)
HEADERFILES=$(wildcard *.h)
OBJECTFILES=$(SOURCEFILES:.c=.o)
OUTPUTFILE=indent
CFLAGS=-ansi -pedantic-errors -Wall -O3
LFLAGS=
TESTPROGRAM=printf "Hello,\nWorld\n"
INSTALLDIR=/usr/bin
INSTALLPATH=$(INSTALLDIR)/$(OUTPUTFILE)

.PHONY: all install uninstall test clean

all: $(OUTPUTFILE)

#BINS := $(SRC:%.c=%)

$(OUTPUTFILE): $(OBJECTFILES)
	cc $(LFLAGS) $(OBJECTFILES) -o $@

%.o: %.c $(HEADERFILES) Makefile
	cc -c $< $(CFLAGS)

install: $(INSTALLPATH)

$(INSTALLPATH): $(OUTPUTFILE)
	sudo cp $(OUTPUTFILE) $(INSTALLDIR)

uninstall:
	sudo rm -f $(INSTALLPATH)

test: $(OUTPUTFILE)
	@$(TESTPROGRAM) | ./$(OUTPUTFILE)

clean:
	rm -rf $(OUTPUTFILE) $(OBJECTFILES)

