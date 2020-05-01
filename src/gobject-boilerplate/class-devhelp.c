#include "filename.h"

struct _NamespaceClassnamePrivate {
};

enum {
        PROP_0,
        PROP_ENABLE_ME,
        N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_WITH_PRIVATE (NamespaceClassname, namespace_classname, G_TYPE_OBJECT)

static void
namespace_classname_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
        NamespaceClassname *self = NAMESPACE_CLASSNAME (object);

        switch (prop_id) {
                case PROP_ENABLE_ME:
                        g_value_set_boolean (value, namespace_classname_get_enable_me (self));
                        break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                        break;
        }
}

static void
namespace_classname_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
        NamespaceClassname *self = NAMESPACE_CLASSNAME (object);

        switch (prop_id) {
                case PROP_ENABLE_ME:
                        namespace_classname_set_enable_me (self, g_value_get_boolean (value));
                        break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                        break;
        }
}

static void
namespace_classname_finalize (GObject *object)
{

        G_OBJECT_CLASS (namespace_classname_parent_class)->finalize (object);
}

static void
namespace_classname_class_init (NamespaceClassnameClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->get_property = namespace_classname_get_property;
        object_class->set_property = namespace_classname_set_property;
        object_class->finalize = namespace_classname_finalize;

        properties[PROP_ENABLE_ME] =
                g_param_spec_boolean ("enable-me",
                                      "Enable Me",
                                      "",
                                      FALSE,
                                      G_PARAM_READWRITE |
                                      G_PARAM_CONSTRUCT |
                                      G_PARAM_STATIC_STRINGS);

        g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

static void
namespace_classname_init (NamespaceClassname *self)
{
        self->priv = namespace_classname_get_instance_private (self);
}
