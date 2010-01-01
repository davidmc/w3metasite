########################################################################
#                         WEBAPI CLIENT LIBRARY Makefile               #
########################################################################
# Needs the API libraries to link with.

ROOTDIR = ./

CC=gcc

LOCAL_DEFINES= -DUNIX -D_REENTRANT
INCLUDES= -I. -I../include
CFLAGS= -g
LIBS= -L../lib -lfedexapi_r -llblexts_r -lctsparser_r -lnsl -lstdc++ -static -lcstp

all: webetest

webetest: webetest.c
	$(CC) -x c++ -o $@ $(CFLAGS)  webetest.c $(INCLUDES) $(LOCAL_DEFINES) $(LIBS)
