
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
        service_name name;
        service_type type;
        service_protocol protocol;
        service_domain domain;

        using handler = std::function<void (const advertised_service& service,
                                            const bool was_added)>;
    };
} // namespace bonjour_moderne
