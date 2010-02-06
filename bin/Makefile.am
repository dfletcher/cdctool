
bin_PROGRAMS    = cdctool
cdctool_CFLAGS  = -I@top_srcdir@/lib
cdctool_LDFLAGS = -L@top_builddir@/lib -lcdctool @POPT@
cdctool_SOURCES = CDC_tool.c

bin_PROGRAMS       += cdcdiscover
cdcdiscover_CFLAGS  = -I@top_srcdir@/lib
cdcdiscover_LDFLAGS = -L@top_builddir@/lib -lcdctool @POPT@
cdcdiscover_SOURCES = CDC_discover.c
