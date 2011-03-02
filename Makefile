SYSTEM=$(shell uname)

ifeq ($(SYSTEM), CYGWIN_NT-5.1)
	EXECSUFFIX=.exe
	RM=-del /F
else
	EXECSUFFIX=
	RM=rm -f
endif

C=gcc
CLIBS=-lm

CFLAGS=-O3 -Wall

SOURCE=nrand.c
HEADER=nrand.h
OBJECT=nrand.o

ifdef NO_HASH
	CDEFS=-DNO_HASH
endif

TESTSOURCE=unittest.c
TESTEXEC=unittest$(EXECSUFFIX)

GENSOURCE=nrandgen.c
GENEXEC=nrandgen$(EXECSUFFIX)

.PHONY: install clean distclean test

all: $(OBJECT) $(GENEXEC)

$(OBJECT): $(SOURCE)
	$(C) -fPIC -c -g $(CFLAGS) $(SOURCE) $(CDEFS) -o $(OBJECT)

test: $(TESTEXEC)

$(TESTEXEC): $(TESTSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(CLIBS) $(TESTSOURCE) $(OBJECT) -o $(TESTEXEC)

$(GENEXEC): $(GENSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(CLIBS) $(GENSOURCE) $(OBJECT) -o $(GENEXEC)

install:
	# TODO: Implement install routine
	false

clean:
	$(RM) $(OBJECT)
	$(RM) $(TESTEXEC)
	$(RM) $(GENEXEC)

distclean: clean

