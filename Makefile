CC = gcc
CFLAGS = -g -O2 -Wall
LIBS = -liconv

all:get_ip_mac	quote	strdel	libpublic.so

get_ip_mac:	get_ip_mac.o
	$(CC) -o $@ get_ip_mac.o $(CFLAGS)

quote:	quote.o
	$(CC) -o $@ quote.o $(CFLAGS)

strdel:	strdel.o
	$(CC) -o $@ strdel.o $(CFLAGS)

libpublic.so:	libs.o
	$(CC) -o $@ -fPIC -shared $(CFLAGS) $(LIBS)

.PHONY:	clean
clean:
	-$(RM) *.*o
	-$(RM) get_ip_mac quote strdel
