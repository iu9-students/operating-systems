build:
	cp lab4.c /usr/src/sys/dev/lab4.c && \
	mkdir /usr/src/sys/modules/lab4 && \
	cp MakefileNetBSD /usr/src/sys/modules/lab4/Makefile && \
	cd /usr/src/sys/modules/lab4/ && \
	make

run:
	cd /usr/src/sys/modules/lab4/ && \
	modload ./lab4.kmod

clean:
	rm /usr/src/sys/dev/lab4.c && \
	rm -r /usr/src/sys/modules/lab4
	