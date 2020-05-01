#ifndef NAMESPACE_CLASSNAME_H
#define NAMESPACE_CLASSNAME_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAMESPACE_TYPE_CLASSNAME (namespace_classname_get_type ())
G_DECLARE_DERIVABLE_TYPE (NamespaceClassname, namespace_classname,
                          NAMESPACE, CLASSNAME,
                          GObject)

struct _NamespaceClassnameClass
{
  GObjectClass parent_class;

  gpointer padding[12];
};

G_END_DECLS

#endif /* NAMESPACE_CLASSNAME_H */
