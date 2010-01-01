#
# Makefile for TCLink C library API.
# Copyright (c) 2001 TrustCommerce.
#

#### Tools

CC = gcc
AR = ar sr

#### Options

MYFLAGS = -Wall -O2 -fPIC
CFLAGS =  -I/usr/include $(MYFLAGS)
LDFLAGS =  -L/usr/lib -lssl -lcrypto -lnsl 

OBJS = tclink.o tctest.o
OBJS2 = tclink.o tcexample.o

BIN = tctest
BIN2 = tcexample

OUTPUTLIBS = libtclink.a libtclink.so

#### Make targets

all: $(BIN) $(BIN2) dynamic static

tctest: Makefile $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)

tcexample: Makefile $(OBJS2)
	$(CC) -o $(BIN2) $(OBJS2) $(LDFLAGS)

dynamic: Makefile tclink.o
	$(CC) -shared -o libtclink.so tclink.o $(LDFLAGS)

static: Makefile tclink.o
	$(AR) libtclink.a tclink.o

install:
	cp libtclink.so /usr/lib
	cp tclink.h /usr/include

clean:
	rm -f $(OUTPUTLIBS) $(OBJS) $(OBJS2) $(BIN)

