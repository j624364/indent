SOURCEFILES=$(wildcard *.c)
HEADERFILES=$(wildcard *.h)
OBJECTFILES=$(SOURCEFILES:.c=.o)
OUTPUTFILE=indent
CFLAGS=-ansi -pedantic-errors -Wall
LFLAGS=
TESTPROGRAM=printf "Hello,\nWorld\n"

.PHONY: all test clean

all: $(OUTPUTFILE)

#BINS := $(SRC:%.c=%)

$(OUTPUTFILE): $(OBJECTFILES)
	cc $(LFLAGS) $(OBJECTFILES) -o $@

%.o: %.c $(HEADERFILES)
	cc -c $< $(CFLAGS)

test: $(OUTPUTFILE)
	@$(TESTPROGRAM) | ./$(OUTPUTFILE)

clean:
	rm -rf $(OUTPUTFILE) $(OBJECTFILES)

