
#pragma once

#include "service_fullname.h"
#include "service_host.h"
#include "service_txt_record.h"

#include <functional>

namespace bonjour_moderne
{
    class resolved_service
    {
    public:
        const service_fullname fullname;
        const service_host host;
        const service_txt_record txt_record;
        const service_interface interface;

        using handler = std::function<void (const resolved_service& sercice,
                                            const bool more_coming)>;

    private:
        friend class service_resolver;

        resolved_service (const service_fullname& fullname,
                          const service_host& host,
                          const service_txt_record& txt_record,
                          const service_interface& interface) noexcept
            : fullname {fullname}
            , host {host}
            , txt_record {txt_record}
            , interface {interface}
        {
        }
    };
} // namespace bonjour_moderne
