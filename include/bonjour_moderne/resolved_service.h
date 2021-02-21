
#pragma once

#include "service_fullname.h"
#include "service_host.h"
#include "service_txt_record.h"

#include <functional>

namespace bonjour_moderne
{
    struct resolved_service
    {
        service_fullname fullname;
        service_host host;
        service_txt_record txt_record;
        service_interface interface;

        using handler = std::function<void (const resolved_service& sercice,
                                            const bool more_coming)>;
    };
} // namespace bonjour_moderne
