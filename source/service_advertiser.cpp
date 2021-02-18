
#include <bonjour_moderne/service_advertiser.h>
#include "dns_service.h"
#include "string.h"

#include <dns_sd.h>

class bonjour_moderne::service_advertiser::Implementation
{
public:
    Implementation (const advertisable_service& service,
                    const advertised_service::handler& service_handler) noexcept
        : service_handler {service_handler}
    {
        const auto service_type_string {service.type.to_string() + service.protocol.to_string()};
        DNSServiceRegister (shared_dns_service.get(),
                            kDNSServiceFlagsShareConnection,
                            service.interface.to_unint32(),
                            service.name.is_empty() ? nullptr : service.name.to_c_str(),
                            service_type_string.c_str(),
                            service.domain.is_empty() ? nullptr : service.domain.to_c_str(),
                            service.host.name.is_empty() ? nullptr : service.host.name.to_c_str(),
                            service.host.port.to_uint16(),
                            service.txt_record.size(),
                            service.txt_record.data(),
                            &dns_service_register_reply,
                            this);
    }

private:
    static void dns_service_register_reply (DNSServiceRef /*sdRef*/,
                                            DNSServiceFlags flags,
                                            DNSServiceErrorType errorCode,
                                            const char* name,
                                            const char* regtype,
                                            const char* domain,
                                            void* context)
    {
        if (const auto implementation = static_cast<Implementation*> (context))
        {
            if (errorCode == kDNSServiceErr_NoError)
            {
                implementation->service_handler ({service_name {name},
                                                  service_type {string {regtype}.split_before ("._")},
                                                  service_protocol {string {regtype}.split_after ("._")},
                                                  service_domain {domain}},
                                                 flags & kDNSServiceFlagsAdd);
            }
        }
    }

    const advertised_service::handler service_handler;
    dns_service::shared_connection shared_dns_service;
};

namespace bonjour_moderne
{
    service_advertiser::service_advertiser (const advertisable_service& service,
                                            const advertised_service::handler& service_handler) noexcept
        : implementation {std::make_unique<Implementation> (service, service_handler)}
    {
    }

    service_advertiser::~service_advertiser()
    {
    }
} // namespace bonjour_moderne
