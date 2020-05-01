#ifndef NAMESPACE_INTERFACENAME_H
#define NAMESPACE_INTERFACENAME_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAMESPACE_TYPE_INTERFACENAME               (namespace_interfacename_get_type ())
#define NAMESPACE_INTERFACENAME(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), NAMESPACE_TYPE_INTERFACENAME, NamespaceInterfacename))
#define NAMESPACE_IS_INTERFACENAME(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NAMESPACE_TYPE_INTERFACENAME))
#define NAMESPACE_INTERFACENAME_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), NAMESPACE_TYPE_INTERFACENAME, NamespaceInterfacenameInterface))

typedef struct _NamespaceInterfacename          NamespaceInterfacename;
typedef struct _NamespaceInterfacenameInterface NamespaceInterfacenameInterface;

struct _NamespaceInterfacenameInterface
{
	GTypeInterface parent_interface;

	void (*do_something) (NamespaceInterfacename *self);
};

GType namespace_interfacename_get_type (void);

void namespace_interfacename_do_something (NamespaceInterfacename *self);

G_END_DECLS

#endif /* NAMESPACE_INTERFACENAME_H */
