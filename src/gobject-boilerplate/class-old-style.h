#ifndef NAMESPACE_CLASSNAME_H
#define NAMESPACE_CLASSNAME_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAMESPACE_TYPE_CLASSNAME             (namespace_classname_get_type ())
#define NAMESPACE_CLASSNAME(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAMESPACE_TYPE_CLASSNAME, NamespaceClassname))
#define NAMESPACE_CLASSNAME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NAMESPACE_TYPE_CLASSNAME, NamespaceClassnameClass))
#define NAMESPACE_IS_CLASSNAME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAMESPACE_TYPE_CLASSNAME))
#define NAMESPACE_IS_CLASSNAME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NAMESPACE_TYPE_CLASSNAME))
#define NAMESPACE_CLASSNAME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NAMESPACE_TYPE_CLASSNAME, NamespaceClassnameClass))

typedef struct _NamespaceClassname         NamespaceClassname;
typedef struct _NamespaceClassnameClass    NamespaceClassnameClass;
typedef struct _NamespaceClassnamePrivate  NamespaceClassnamePrivate;

struct _NamespaceClassname
{
	GObject parent;

	NamespaceClassnamePrivate *priv;
};

struct _NamespaceClassnameClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

GType namespace_classname_get_type (void);

G_END_DECLS

#endif /* NAMESPACE_CLASSNAME_H */
