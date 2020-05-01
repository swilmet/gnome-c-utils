#include "filename.h"

G_DEFINE_INTERFACE (NamespaceInterfacename, namespace_interfacename, G_TYPE_OBJECT)

static void
namespace_interfacename_default_init (NamespaceInterfacenameInterface *interface)
{
  /* Add properties and signals to the interface here. */
}

void
namespace_interfacename_do_something (NamespaceInterfacename *self)
{
  g_return_if_fail (NAMESPACE_IS_INTERFACENAME (self));

  NAMESPACE_INTERFACENAME_GET_IFACE (self)->do_something (self);
}
