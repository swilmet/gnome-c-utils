#ifndef NAMESPACE_INTERFACENAME_H
#define NAMESPACE_INTERFACENAME_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAMESPACE_TYPE_INTERFACENAME (namespace_interfacename_get_type ())
G_DECLARE_INTERFACE (NamespaceInterfacename, namespace_interfacename,
                     NAMESPACE, INTERFACENAME,
                     GObject)

struct _NamespaceInterfacenameInterface
{
  GTypeInterface parent_interface;

  void (* do_something) (NamespaceInterfacename *self);
};

void namespace_interfacename_do_something (NamespaceInterfacename *self);

G_END_DECLS

#endif /* NAMESPACE_INTERFACENAME_H */
