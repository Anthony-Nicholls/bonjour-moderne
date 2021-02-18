
#pragma once

#include "service_name.h"
#include "service_type.h"
#include "service_protocol.h"
#include "service_host.h"
#include "service_domain.h"
#include "service_interface.h"
#include "service_txt_record.h"

namespace bonjour_moderne
{
    struct advertisable_service
    {
        const service_name name;
        const service_type type;
        const service_protocol protocol;
        const service_host host;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};
        const service_txt_record txt_record {};
    };
} // namespace bonjour_moderne
