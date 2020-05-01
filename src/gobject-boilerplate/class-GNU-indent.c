#include "filename.h"

typedef struct _NamespaceClassnamePrivate NamespaceClassnamePrivate;

struct _NamespaceClassnamePrivate
{
  gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (NamespaceClassname, namespace_classname, G_TYPE_OBJECT)

static void
namespace_classname_finalize (GObject *object)
{

  G_OBJECT_CLASS (namespace_classname_parent_class)->finalize (object);
}

static void
namespace_classname_class_init (NamespaceClassnameClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = namespace_classname_finalize;
}

static void
namespace_classname_init (NamespaceClassname *self)
{
}
