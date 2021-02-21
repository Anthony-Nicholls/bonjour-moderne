
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
        service_name name;
        service_type type;
        service_protocol protocol;
        service_host host;
        service_domain domain {service_domain::any};
        service_interface interface {service_interface::any};
        service_txt_record txt_record {};
    };
} // namespace bonjour_moderne
