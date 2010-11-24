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

GENSOURCE=nrandgen.c
GENEXEC=nrandgen$(EXECSUFFIX)

.PHONY: install clean distclean test

all: $(OBJECT) $(GENEXEC)

$(OBJECT): $(SOURCE)
	$(C) -fPIC -c -g $(CFLAGS) $(SOURCE) -o $(OBJECT)

$(GENEXEC): $(GENSOURCE) $(OBJECT)
	$(C) $(CFLAGS) $(GENSOURCE) $(OBJECT) -o $(GENEXEC)

install:
	# TODO: Implement install routine
	false

clean:
	$(RM) $(OBJECT)
	$(RM) $(GENEXEC)

distclean: clean

