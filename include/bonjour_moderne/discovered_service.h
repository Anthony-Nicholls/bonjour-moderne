
#pragma once

#include "service_name.h"
#include "service_type.h"
#include "service_protocol.h"
#include "service_domain.h"
#include "service_interface.h"

#include <functional>

namespace bonjour_moderne
{
    struct discovered_service
    {
        service_name name;
        service_type type;
        service_protocol protocol;
        service_domain domain;
        service_interface interface {service_interface::any};

        using handler = std::function<void (const discovered_service& service,
                                            const bool was_added,
                                            const bool more_coming)>;
    };
} // namespace bonjour_moderne
