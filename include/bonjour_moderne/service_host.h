
#pragma once

#include "service_hostname.h"
#include "service_port.h"

namespace bonjour_moderne
{
    class service_host
    {
    public:
        service_host (const service_hostname& hostname,
                      const service_port& port) noexcept;

        service_host (const service_port& port) noexcept;

        const service_hostname name {service_hostname::auto_resolve};
        const service_port port;
    };

    bool operator== (const service_host& lhs, const service_host& rhs) noexcept;
    bool operator!= (const service_host& lhs, const service_host& rhs) noexcept;
} // namespace bonjour_moderne
