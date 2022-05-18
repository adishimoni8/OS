CC=g++
CXX=g++
LD=g++
RANLIB=ranlib

EXESRC=Barrier.cpp Barrier.h JobContext.cpp JobContext.h MapReduceFramework.cpp ThreadContext.cpp ThreadContext.h Exception.h
EXEOBJ=$(EXESRC:.cpp=.o)

INCS=-I.
CFLAGS = -Wall -std=c++11 -g $(INCS)
CXXFLAGS = -Wall -std=c++11 -g $(INCS)
LDFLAGS = -pthread

MAPREDUCELIB = libMapReduceFramework.a
TARGETS = $(MAPREDUCELIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex3.tar
TARSRCS=$(EXESRC) Makefile README

all: $(TARGETS)

$(TARGETS): $(EXEOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGETS) $(MAPREDUCELIB) $(OBJ) $(EXEOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
