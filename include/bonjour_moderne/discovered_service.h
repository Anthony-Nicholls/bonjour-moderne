
#pragma once

#include "service_name.h"
#include "service_type.h"
#include "service_protocol.h"
#include "service_domain.h"
#include "service_interface.h"

#include <functional>

namespace bonjour_moderne
{
    class discovered_service
    {
    public:
        const service_name name;
        const service_type type;
        const service_protocol protocol;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};

        using handler = std::function<void (const discovered_service& service,
                                            const bool was_added,
                                            const bool more_coming)>;

    private:
        friend class service_browser;

        discovered_service (const service_name& name,
                            const service_type& type,
                            const service_protocol& protocol,
                            const service_domain& domain,
                            const service_interface& interface) noexcept
            : name {name}
            , type {type}
            , protocol {protocol}
            , domain {domain}
            , interface {interface}
        {
        }
    };
} // namespace bonjour_moderne
