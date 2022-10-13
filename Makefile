SOURCEFILES=$(wildcard *.c)
HEADERFILES=$(wildcard *.h)
OBJECTFILES=$(SOURCEFILES:.c=.o)
OUTPUTFILE=indent
CFLAGS=-ansi -pedantic-errors -Wall
LFLAGS=
TESTPROGRAM=pfetch

.PHONY: all test clean

all: $(OUTPUTFILE)

#BINS := $(SRC:%.c=%)

$(OUTPUTFILE): $(OBJECTFILES)
	cc $(LFLAGS) $(OBJECTFILES) -o $@

%.o: %.c $(HEADERFILES)
	cc -c $< $(CFLAGS)

test: $(OUTPUTFILE)
	pfetch | ./indent

clean:
	rm -rf $(OUTPUTFILE) $(OBJECTFILES)

