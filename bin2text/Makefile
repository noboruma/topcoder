# Makefile for bin2text
#
# Defines the build configuration targets and recipes thereof
#
# MIT License
# 
# Copyright (c) 2016 Thomas Legris, Edmund Higham
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CLFLAGS=-std=c11 $(INLINE_WARN) $(INLINE_LIMIT) -pthread -Wall
OPTIM = -O3 -march=native -DNDEBUG 
DEBUG = -O0 -g -Wall -Wextra -Wconversion
GPP=gcc
BASEDIR=.
OBJDIR=$(BASEDIR)/obj
BIN=$(BASEDIR)
SRCDIR=src
RESDIR=$(SRCDIR)/resource
LIBCINC= -I.
LIBLINC= -L. 
LIBLINK= -lm #-lgsl -lgslcblas #-lgmp -lmpfr

CFLAGS=$(CLFLAGS)  -Wall  $(LIBCINC) 
LFLAGS=$(CLFLAGS)  -Wall  $(LIBLINC)

#############################################################################
#add non-executables .c files to OBJS 
#if .c file reside in a dir other than the current one, add .o rule below
OBJS=$(OBJDIR)/strings.o \
		 $(OBJDIR)/tuple.o \
		 $(OBJDIR)/var.o \
		 $(OBJDIR)/run.o 


##############################################################################
#EXECUTABLES: add executables rules here

.PHONY: release all test debug

release: CFLAGS +=$(OPTIM)
release: search
	@echo "release: build successful"

all: CFLAGS += $(OPTIM)
all: search
all: test
	@echo "all: build successful"

debug: CFLAGS += $(DEBUG)
debug: search
	@echo "debug: build successful"


test:
	make -f test/Makefile

search: $(OBJDIR)/search.oo
	cp $(OBJDIR)/search.oo $(BIN)/run

%.oo: %.o $(OBJS) $(LIBS)
	echo $<
	$(GPP) $(LFLAGS) -o $@ $(OBJS) $< $(LIBLINK)

$(OBJDIR)/search.o: $(SRCDIR)/main.c
	mkdir -p obj
	$(GPP) $(CFLAGS) -o $(OBJDIR)/search.o -c $(SRCDIR)/main.c


##############################################################################
# add non-executables .o rules here

$(OBJDIR)/strings.o: $(RESDIR)/strings.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@

$(OBJDIR)/tuple.o: $(SRCDIR)/tuple.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@

$(OBJDIR)/var.o: $(SRCDIR)/var.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@


$(OBJDIR)/run.o: $(SRCDIR)/run.c
	$(GPP) $(CFLAGS) $ -c $ $^ $ $ -o $ $@

###############################################################################

clean:
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.oo $(SRCDIR)/*~ 
