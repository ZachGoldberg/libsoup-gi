/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * soup-proxy-resolver-static.c: Static proxy "resolution"
 *
 * Copyright (C) 2008 Red Hat, Inc.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "soup-proxy-resolver-static.h"
#include "soup-address.h"
#include "soup-dns.h"
#include "soup-message.h"
#include "soup-misc.h"
#include "soup-session-feature.h"

typedef struct {
	SoupURI *proxy_uri;

} SoupProxyResolverStaticPrivate;
#define SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SOUP_TYPE_PROXY_RESOLVER_STATIC, SoupProxyResolverStaticPrivate))

static void soup_proxy_resolver_static_interface_init (SoupProxyResolverInterface *proxy_resolver_interface);

G_DEFINE_TYPE_EXTENDED (SoupProxyResolverStatic, soup_proxy_resolver_static, G_TYPE_OBJECT, 0,
			G_IMPLEMENT_INTERFACE (SOUP_TYPE_SESSION_FEATURE, NULL)
			G_IMPLEMENT_INTERFACE (SOUP_TYPE_PROXY_RESOLVER, soup_proxy_resolver_static_interface_init))

enum {
	PROP_0,

	PROP_PROXY_URI,

	LAST_PROP
};

static void set_property (GObject *object, guint prop_id,
			  const GValue *value, GParamSpec *pspec);
static void get_property (GObject *object, guint prop_id,
			  GValue *value, GParamSpec *pspec);

static void get_proxy_async (SoupProxyResolver  *proxy_resolver,
			     SoupMessage        *msg,
			     GMainContext       *async_context,
			     GCancellable       *cancellable,
			     SoupProxyResolverCallback callback,
			     gpointer            user_data);
static guint get_proxy_sync (SoupProxyResolver  *proxy_resolver,
			     SoupMessage        *msg,
			     GCancellable       *cancellable,
			     SoupAddress       **addr);

static void
soup_proxy_resolver_static_init (SoupProxyResolverStatic *resolver_static)
{
}

static void
finalize (GObject *object)
{
	SoupProxyResolverStaticPrivate *priv =
		SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE (object);

	if (priv->proxy_uri)
		soup_uri_free (priv->proxy_uri);

	G_OBJECT_CLASS (soup_proxy_resolver_static_parent_class)->finalize (object);
}

static void
soup_proxy_resolver_static_class_init (SoupProxyResolverStaticClass *static_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (static_class);

	g_type_class_add_private (static_class, sizeof (SoupProxyResolverStaticPrivate));

	object_class->set_property = set_property;
	object_class->get_property = get_property;
	object_class->finalize = finalize;

	g_object_class_install_property (
		object_class, PROP_PROXY_URI,
		g_param_spec_boxed (SOUP_PROXY_RESOLVER_STATIC_PROXY_URI,
				    "Proxy URI",
				    "The HTTP Proxy to use",
				    SOUP_TYPE_URI,
				    G_PARAM_READWRITE));
}

static void
set_property (GObject *object, guint prop_id,
	      const GValue *value, GParamSpec *pspec)
{
	SoupProxyResolverStaticPrivate *priv =
		SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE (object);
	SoupURI *uri;

	switch (prop_id) {
	case PROP_PROXY_URI:
		uri = g_value_get_boxed (value);
		if (priv->proxy_uri)
			soup_uri_free (priv->proxy_uri);

		priv->proxy_uri = uri ? soup_uri_copy (uri) : NULL;
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
get_property (GObject *object, guint prop_id,
	      GValue *value, GParamSpec *pspec)
{
	SoupProxyResolverStaticPrivate *priv =
		SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE (object);

	switch (prop_id) {
	case PROP_PROXY_URI:
		g_value_set_boxed (value, priv->proxy_uri);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
soup_proxy_resolver_static_interface_init (SoupProxyResolverInterface *proxy_resolver_interface)
{
	proxy_resolver_interface->get_proxy_async = get_proxy_async;
	proxy_resolver_interface->get_proxy_sync = get_proxy_sync;
}

SoupProxyResolver *
soup_proxy_resolver_static_new (SoupURI *proxy_uri)
{
	return g_object_new (SOUP_TYPE_PROXY_RESOLVER_STATIC,
			     SOUP_PROXY_RESOLVER_STATIC_PROXY_URI, proxy_uri,
			     NULL);
}

typedef struct {
	SoupProxyResolver *proxy_resolver;
	SoupMessage *msg;
	SoupAddress *addr;
	SoupProxyResolverCallback callback;
	gpointer user_data;
} SoupStaticAsyncData;

static void
resolved_address (SoupAddress *addr, guint status, gpointer data)
{
	SoupStaticAsyncData *ssad = data;

	ssad->callback (ssad->proxy_resolver, ssad->msg,
			soup_status_proxify (status), addr,
			ssad->user_data);
	g_object_unref (ssad->proxy_resolver);
	g_object_unref (ssad->msg);
	if (addr)
		g_object_unref (addr);
	g_slice_free (SoupStaticAsyncData, ssad);
}

static void
get_proxy_async (SoupProxyResolver  *proxy_resolver,
		 SoupMessage        *msg,
		 GMainContext       *async_context,
		 GCancellable       *cancellable,
		 SoupProxyResolverCallback callback,
		 gpointer            user_data)
{
	SoupProxyResolverStaticPrivate *priv =
		SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE (proxy_resolver);
	SoupStaticAsyncData *ssad;

	ssad = g_slice_new0 (SoupStaticAsyncData);
	ssad->proxy_resolver = g_object_ref (proxy_resolver);
	ssad->msg = g_object_ref (msg);
	ssad->callback = callback;
	ssad->user_data = user_data;
	ssad->addr = soup_address_new (priv->proxy_uri->host,
				       priv->proxy_uri->port);

	soup_address_resolve_async (ssad->addr, async_context,
				    cancellable, resolved_address,
				    ssad);
}

static guint
get_proxy_sync (SoupProxyResolver  *proxy_resolver,
		SoupMessage        *msg,
		GCancellable       *cancellable,
		SoupAddress       **addr)
{
	SoupProxyResolverStaticPrivate *priv =
		SOUP_PROXY_RESOLVER_STATIC_GET_PRIVATE (proxy_resolver);
	guint status;

	*addr = soup_address_new (priv->proxy_uri->host,
				  priv->proxy_uri->port);
	status = soup_status_proxify (soup_address_resolve_sync (*addr, cancellable));
	if (!SOUP_STATUS_IS_SUCCESSFUL (status)) {
		g_object_unref (*addr);
		*addr = NULL;
	}
	return status;
}
