
#include <bonjour_moderne/service_port.h>

namespace bonjour_moderne
{
    const service_port service_port::placeholder {0};

    service_port::service_port (uint16_t port) noexcept
        : index {port}
    {
    }

    uint16_t service_port::to_uint16() const noexcept
    {
        return index;
    }

    bool operator== (const service_port& lhs, const service_port& rhs) noexcept
    {
        return lhs.to_uint16() == rhs.to_uint16();
    }

    bool operator!= (const service_port& lhs, const service_port& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
