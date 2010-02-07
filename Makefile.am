SUBDIRS = gnulib lib bin
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = m4/gnulib-cache.m4

doxygen-doc:
	make -C lib doxygen-doc

doxygen-run:
	make -C lib doxygen-run

doxygen-man:
	make -C lib doxygen-man

doxygen-ps:
	make -C lib doxygen-ps

doxygen-pdf:
	make -C lib doxygen-pdf

windows: all
	BININST=@PACKAGE_NAME@-@PACKAGE_VERSION@-w32 && \
	mkdir -p $${BININST} && \
	cp bin/.libs/cdctool.exe $${BININST} && \
	cp lib/.libs/cygcdctool-0.dll $${BININST} && \
	cp /bin/cygpopt-0.dll $${BININST} && \
	cp /bin/cygwin1.dll $${BININST} && \
	zip -r $${BININST}.zip $${BININST}/* && \
	rm -rf $${BININST}
