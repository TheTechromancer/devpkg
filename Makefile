PREFIX?=/usr/local
CFLAGS=-g -Wall -l apr-1
LDFLAGS=-pthread -l aprutil-1

all: devpkg

devpkg: bstrlib.o db.o shell.o commands.o

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin/
	install devpkg $(DESTDIR)/$(PREFIX)/bin/

uninstall:
	rm $(DESTDIR)/$(PREFIX)/bin/devpkg

clean:
	rm -rf *.o
	rm -f devpkg