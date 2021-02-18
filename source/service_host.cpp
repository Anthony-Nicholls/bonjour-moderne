
#include <bonjour_moderne/service_host.h>

namespace bonjour_moderne
{
    service_host::service_host (const service_hostname& hostname,
                                const service_port& port) noexcept
        : name {hostname}
        , port {port}
    {
    }

    service_host::service_host (const service_port& port) noexcept
        : port {port}
    {
    }

    bool operator== (const service_host& lhs, const service_host& rhs) noexcept
    {
        return lhs.name == rhs.name
               && lhs.port == rhs.port;
    }

    bool operator!= (const service_host& lhs, const service_host& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
