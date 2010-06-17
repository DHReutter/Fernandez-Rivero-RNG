SYSTEM=$(shell uname)

ifeq ($(SYSTEM), CYGWIN_NT-5.1)
	EXECSUFFIX=.exe
	RM=-del /F
else
	EXECSUFFIX=
	RM=rm -f
endif

C=gcc
CFLAGS=-O3 -Wall

SOURCE=nrand.c
HEADER=nrand.h
OBJECT=nrand.o

TESTSOURCE=test.c
TESTEXEC=runtest$(EXECSUFFIX)

.PHONY: install clean distclean test

all: $(OBJECT)

$(OBJECT): $(SOURCE)
	$(C) -fPIC -c -g $(CFLAGS) $(SOURCE) -o $(OBJECT)

test: $(TESTEXEC)

$(TESTEXEC): $(TESTSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(TESTSOURCE) $(OBJECT) -o $(TESTEXEC)

install:
	# TODO: Implement install routine
	false

clean:
	$(RM) $(OBJECT)
	$(RM) $(TESTEXEC)

distclean: clean

