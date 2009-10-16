## Process this file with automake to produce Makefile.in

if OS_WIN32
LIBWS2_32 = -lws2_32
endif

INCLUDES = 				\
	-DG_LOG_DOMAIN=\"libsoup\" 	\
	-I$(top_srcdir)			\
	$(SOUP_DEBUG_FLAGS)		\
	$(SOUP_MAINTAINER_FLAGS)	\
	$(GLIB_CFLAGS)			\
	$(XML_CFLAGS)			\
	$(GCONF_CFLAGS)			\
	$(LIBPROXY_CFLAGS)		\
	$(SQLITE_CFLAGS)		\
	$(LIBGCRYPT_CFLAGS)		\
	$(LIBGNUTLS_CFLAGS)

MARSHAL_GENERATED = soup-marshal.c soup-marshal.h
MKENUMS_GENERATED = soup-enum-types.c soup-enum-types.h

soup-marshal.h: soup-marshal.list
	( $(GLIB_GENMARSHAL) --prefix=soup_marshal $(srcdir)/soup-marshal.list --header > soup-marshal.tmp \
	&& mv soup-marshal.tmp soup-marshal.h ) \
	|| ( rm -f soup-marshal.tmp && exit 1 )

soup-marshal.c: soup-marshal.h
	( (echo '#include "soup-marshal.h"'; $(GLIB_GENMARSHAL) --prefix=soup_marshal $(srcdir)/soup-marshal.list --body) > soup-marshal.tmp \
	&& mv soup-marshal.tmp soup-marshal.c ) \
	|| ( rm -f soup-marshal.tmp && exit 1 )

soup-enum-types.h: $(soup_headers)
	( cd $(srcdir) && $(GLIB_MKENUMS) --template soup-enum-types.h.tmpl \
		$(soup_headers) ) > soup-enum-types.h.tmp \
	&& mv soup-enum-types.h.tmp soup-enum-types.h \
	|| rm -f soup-enum-type.h.tmp

soup-enum-types.c: $(libsoupinclude_HEADERS)
	( cd $(srcdir) && $(GLIB_MKENUMS) --template soup-enum-types.c.tmpl \
		$(soup_headers) ) > soup-enum-types.c.tmp \
	&& mv soup-enum-types.c.tmp soup-enum-types.c \
	|| rm -f soup-enum-type.c.tmp

BUILT_SOURCES = $(MARSHAL_GENERATED) $(MKENUMS_GENERATED)

CLEANFILES = $(MARSHAL_GENERATED) $(MKENUMS_GENERATED)

libsoupincludedir = $(includedir)/libsoup-2.4/libsoup

soup_headers =			\
	soup.h			\
	soup-address.h		\
	soup-auth.h		\
	soup-auth-domain.h	\
	soup-auth-domain-basic.h  \
	soup-auth-domain-digest.h \
	soup-cookie.h		\
	soup-cookie-jar.h	\
	soup-cookie-jar-text.h	\
	soup-date.h		\
	soup-form.h		\
	soup-headers.h		\
	soup-logger.h		\
	soup-message.h		\
	soup-message-body.h	\
	soup-message-headers.h	\
	soup-method.h		\
	soup-misc.h     	\
	soup-multipart.h     	\
	soup-portability.h	\
	soup-proxy-resolver.h	\
	soup-proxy-uri-resolver.h \
	soup-server.h		\
	soup-session.h		\
	soup-session-async.h	\
	soup-session-feature.h	\
	soup-session-sync.h	\
	soup-socket.h		\
	soup-status.h		\
	soup-types.h		\
	soup-uri.h		\
	soup-value-utils.h	\
	soup-xmlrpc.h

libsoupinclude_HEADERS =	\
	$(soup_headers)		\
	soup-enum-types.h

lib_LTLIBRARIES = libsoup-2.4.la

libsoup_2_4_la_LDFLAGS =	\
	-version-info $(SOUP_CURRENT):$(SOUP_REVISION):$(SOUP_AGE) -no-undefined

libsoup_2_4_la_LIBADD =			\
	$(GLIB_LIBS)			\
	$(XML_LIBS)			\
	$(LIBGNUTLS_LIBS_STATIC)	\
	$(LIBGNUTLS_LIBS)		\
	$(LIBGCRYPT_LIBS)		\
	$(LIBWS2_32)

libsoup_2_4_la_SOURCES =		\
	$(BUILT_SOURCES)		\
	soup-address.c			\
	soup-auth.c			\
	soup-auth-basic.h		\
	soup-auth-basic.c		\
	soup-auth-digest.h		\
	soup-auth-digest.c		\
	soup-auth-ntlm.h		\
	soup-auth-ntlm.c		\
	soup-auth-domain.c		\
	soup-auth-domain-basic.c	\
	soup-auth-domain-digest.c	\
	soup-auth-manager.h		\
	soup-auth-manager.c		\
	soup-auth-manager-ntlm.h	\
	soup-auth-manager-ntlm.c	\
	soup-connection.h		\
	soup-connection.c		\
	soup-cookie.c			\
	soup-cookie-jar.c		\
	soup-cookie-jar-text.c		\
	soup-date.c			\
	soup-dns.h			\
	soup-dns.c			\
	soup-form.c			\
	soup-gnutls.c			\
	soup-headers.c			\
	soup-logger.c			\
	soup-message.c			\
	soup-message-body.c		\
	soup-message-client-io.c	\
	soup-message-headers.c		\
	soup-message-io.c		\
	soup-message-private.h		\
	soup-message-queue.h		\
	soup-message-queue.c		\
	soup-message-server-io.c	\
	soup-method.c     		\
	soup-misc.c     		\
	soup-multipart.c	     	\
	soup-nossl.c     		\
	soup-path-map.h     		\
	soup-path-map.c     		\
	soup-proxy-resolver.c		\
	soup-proxy-resolver-static.h	\
	soup-proxy-resolver-static.c	\
	soup-proxy-uri-resolver.c	\
	soup-server.c			\
	soup-session.c			\
	soup-session-async.c		\
	soup-session-feature.c		\
	soup-session-private.h		\
	soup-session-sync.c		\
	soup-socket.c			\
	soup-ssl.h			\
	soup-status.c			\
	soup-uri.c			\
	soup-value-utils.c		\
	soup-xmlrpc.c

if BUILD_LIBSOUP_GNOME

libsoupgnomeincludedir = $(includedir)/libsoup-gnome-2.4/libsoup

libsoupgnomeinclude_HEADERS =	\
	soup-cookie-jar-sqlite.h\
	soup-gnome.h		\
	soup-gnome-features.h

lib_LTLIBRARIES += libsoup-gnome-2.4.la

libsoup_gnome_2_4_la_LDFLAGS = $(libsoup_2_4_la_LDFLAGS)

libsoup_gnome_2_4_la_LIBADD =		\
	libsoup-2.4.la			\
	$(GLIB_LIBS)			\
	$(GCONF_LIBS)			\
	$(LIBPROXY_LIBS)		\
	$(SQLITE_LIBS)

libsoup_gnome_2_4_la_SOURCES =		\
	soup-cookie-jar-sqlite.c	\
	soup-gnome-features.c		\
	soup-proxy-resolver-gnome.h	\
	soup-proxy-resolver-gnome.c

endif

EXTRA_DIST=				\
	soup-marshal.list		\
	soup-enum-types.h.tmpl		\
	soup-enum-types.c.tmpl

SCANNER_BIN = g-ir-scanner
SCANNER_PYTHONPATH = $(top_builddir):$(top_srcdir):$$PYTHONPATH
SCANNER_ENV = env PYTHONPATH=$(SCANNER_PYTHONPATH) \
        UNINSTALLED_INTROSPECTION_SRCDIR=$(top_srcdir) \
        UNINSTALLED_INTROSPECTION_BUILDDIR=$(top_builddir)
SCANNER_ARGS = -v --add-include-path=.
SCANNER = $(AM_V_GEN) env LPATH=.libs $(CHECK_DEBUG) $(SCANNER_ENV) $(SCANNER_BIN) $(SCANNER_ARGS)
SCANNER_LIBS = \
        $(top_srcdir)/giscanner/*.py \
        $(top_builddir)/giscanner/libgiscanner.la \
        $(top_builddir)/girepository/libgirepository-1.0.la

GLIB_INCLUDEDIR=`pkg-config --variable=includedir glib-2.0`/glib-2.0
GLIB_LIBDIR=`pkg-config --variable=libdir glib-2.0`

GLIB_LIBRARY=glib-2.0


libsoup_head = soup-address.h \
	soup-auth-basic.h \
	soup-auth-digest.h \
	soup-auth-domain-basic.h \
	soup-auth-domain-digest.h \
	soup-auth-domain.h \
	soup-auth-manager-ntlm.h \
	soup-auth-manager.h \
	soup-auth-ntlm.h \
	soup-auth.h \
 	soup-connection.h \
	soup-cookie-jar.h \
	soup-cookie-jar-text.h 
	soup-cookie.h \
	soup-date.h \
	soup-dns.h \
	soup-enum-types.h \
	soup-form.h \
	soup-headers.h \
	soup-logger.h \
	soup-marshal.h \
	soup-message-body.h \
	soup-message-headers.h \
	soup-message-queue.h \
	soup-message.h \
	soup-method.h \
	soup-misc.h \
	soup-multipart.h \
	soup-path-map.h \
	soup-portability.h \
	soup-proxy-resolver-gnome.h \
	soup-proxy-resolver-static.h \
	soup-proxy-resolver.h \
	soup-proxy-uri-resolver.h \
	soup-server.h \
	soup-session-async.h \
	soup-session-feature.h \
	soup-session-sync.h \
	soup-session.h \
	soup-socket.h \
	soup-ssl.h \
	soup-status.h \
	soup-types.h \
	soup-uri.h \
	soup-value-utils.h \
	soup-xmlrpc.h \
	soup.h 


libsoup-2.4.gir:
	env PYTHONPATH=.:.. env LPATH=.libs $(SCANNER_BIN) $(SCANNER_ARGS) \
            --namespace libsoup --nsversion=2.4 \
            --noclosure \
            --output $@ \
            --strip-prefix=g \
            --libtool="$(LIBTOOL)" \
            -I/usr/local/include/gssdp-1.0 \
            -I/usr/include/libxml2 \
            -I/usr/local/include/gobject-introspection-1.0 \
            -I$(GLIB_INCLUDEDIR) \
            -I$(GLIB_LIBDIR)/glib-2.0/include \
            --include=GObject-2.0 \
            --library=soup-2.4 \
            --pkg libsoup-2.4 \
	    $(libsoup_head)

libsoup-2.4.typelib: libsoup-2.4.gir
	g-ir-compiler --includedir=. $(G_IR_COMPILER_OPTS)  -o $@ libsoup-2.4.gir




