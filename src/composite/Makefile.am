
EXTRA_DIST = 

INCLUDES = -DDATADIR=\"$(DATADIR)\" -DCONFDIR=\"$(CONFDIR)\" -DPKGDATADIR=\"$(PKGDATADIR)\" -DPREFIX=\"$(PREFIXDIR)\" $(LIBMB_CFLAGS) $(COMPO_CFLAGS) $(EXPAT_CFLAGS) $(SN_CFLAGS) $(GCONF_CFLAGS) $(XFIXES_CFLAGS) $(XCURSOR_CFLAGS) -I$(top_srcdir)/src/

noinst_LTLIBRARIES = libmbcomposite.la

libmbcomposite_la_SOURCES = composite-engine.c composite-engine.h \
			    timeline.h timeline.c

clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov || true
