
#pragma once

#include "service_type.h"
#include "service_protocol.h"
#include "service_domain.h"
#include "service_interface.h"

namespace bonjour_moderne
{
    struct discoverable_service
    {
        const service_type type;
        const service_protocol protocol;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};
    };
} // namespace bonjour_moderne
