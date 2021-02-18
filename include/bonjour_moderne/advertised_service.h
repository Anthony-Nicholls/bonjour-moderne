
#pragma once

#include "service_name.h"
#include "service_type.h"
#include "service_protocol.h"
#include "service_domain.h"

#include <functional>

namespace bonjour_moderne
{
    class advertised_service
    {
    public:
        const service_name name;
        const service_type type;
        const service_protocol protocol;
        const service_domain domain;

        using handler = std::function<void (const advertised_service& service,
                                            const bool was_added)>;

    private:
        friend class service_advertiser;

        advertised_service (const service_name& name,
                            const service_type& type,
                            const service_protocol& protocol,
                            const service_domain& domain) noexcept
            : name {name}
            , type {type}
            , protocol {protocol}
            , domain {domain}
        {
        }
    };
} // namespace bonjour_moderne
