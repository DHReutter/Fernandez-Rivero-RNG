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
CFLAGS=-O3 -Wall $(CLIBS)

SOURCE=nrand.c
HEADER=nrand.h
OBJECT=nrand.o

TESTSOURCE=unittest.c
TESTEXEC=unittest$(EXECSUFFIX)

GENSOURCE=nrandgen.c
GENEXEC=nrandgen$(EXECSUFFIX)

.PHONY: install clean distclean test

all: $(OBJECT) $(GENEXEC)

$(OBJECT): $(SOURCE)
	$(C) -fPIC -c -g $(CFLAGS) $(SOURCE) -o $(OBJECT)

test: $(TESTEXEC)

$(TESTEXEC): $(TESTSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(TESTSOURCE) $(OBJECT) -o $(TESTEXEC)

$(GENEXEC): $(GENSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(GENSOURCE) $(OBJECT) -o $(GENEXEC)

install:
	# TODO: Implement install routine
	false

clean:
	$(RM) $(OBJECT)
	$(RM) $(TESTEXEC)
	$(RM) $(GENEXEC)

distclean: clean

