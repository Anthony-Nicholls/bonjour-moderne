
#include <bonjour_moderne/service_browser.h>
#include "dns_service.h"
#include "string.h"

#include <dns_sd.h>

class bonjour_moderne::service_browser::Implementation
{
public:
    Implementation (const discoverable_service& service,
                    const discovered_service::handler& service_handler)
        : service_handler {service_handler}
    {
        const auto service_type_string {service.type.to_string() + service.protocol.to_string()};
        DNSServiceBrowse (shared_dns_service.get(),
                          kDNSServiceFlagsShareConnection,
                          service.interface.to_unint32(),
                          service_type_string.c_str(),
                          service.domain.is_any() ? nullptr : service.domain.to_c_str(),
                          &dns_service_browse_reply,
                          this);
    }

private:
    static void dns_service_browse_reply (DNSServiceRef /*sdRef*/,
                                          DNSServiceFlags flags,
                                          uint32_t interfaceIndex,
                                          DNSServiceErrorType errorCode,
                                          const char* serviceName,
                                          const char* regtype,
                                          const char* replyDomain,
                                          void* context)
    {
        if (const auto implementation = static_cast<Implementation*> (context))
        {
            if (errorCode == kDNSServiceErr_NoError)
            {
                implementation->service_handler ({service_name {serviceName},
                                                  service_type {string {regtype}.split_before ("._")},
                                                  service_protocol {string {regtype}.split_after ("._")},
                                                  service_domain {replyDomain},
                                                  service_interface {interfaceIndex}},
                                                 flags & kDNSServiceFlagsAdd,
                                                 flags & kDNSServiceFlagsMoreComing);
            }
        }
    }

    const discovered_service::handler service_handler;
    dns_service::shared_connection shared_dns_service;
};

namespace bonjour_moderne
{
    service_browser::service_browser (const discoverable_service& service,
                                      const discovered_service::handler& service_handler)
        : implementation {std::make_unique<Implementation> (service, service_handler)}
    {
    }

    service_browser::~service_browser() = default;
} // namespace bonjour_moderne
