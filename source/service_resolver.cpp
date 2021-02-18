
#include <bonjour_moderne/service_resolver.h>
#include "dns_service.h"

#include <dns_sd.h>

class bonjour_moderne::service_resolver::Implementation
{
public:
    Implementation (const discovered_service& service,
                    const resolved_service::handler service_handler)
        : service_handler {service_handler}
    {
        const auto service_type_string {service.type.to_string() + service.protocol.to_string()};
        DNSServiceResolve (shared_dns_service.get(),
                           kDNSServiceFlagsShareConnection,
                           service.interface.to_unint32(),
                           service.name.to_c_str(),
                           service_type_string.c_str(),
                           service.domain.to_c_str(),
                           &dns_service_resolve_reply,
                           this);
    }

private:
    static void dns_service_resolve_reply (DNSServiceRef /*sdRef*/,
                                           DNSServiceFlags flags,
                                           uint32_t interfaceIndex,
                                           DNSServiceErrorType errorCode,
                                           const char* fullname,
                                           const char* hosttarget,
                                           uint16_t port,
                                           uint16_t txtLen,
                                           const unsigned char* txtRecord,
                                           void* context)
    {
        if (const auto implementation = static_cast<Implementation*> (context))
        {
            if (errorCode == kDNSServiceErr_NoError)
            {
                implementation->service_handler ({service_fullname {fullname},
                                                  service_host {service_hostname {hosttarget}, service_port {port}},
                                                  service_txt_record {txtRecord, txtLen},
                                                  service_interface {interfaceIndex}},
                                                 flags & kDNSServiceFlagsMoreComing);
            }
        }
    }

    const resolved_service::handler service_handler;
    dns_service::shared_connection shared_dns_service;
};

namespace bonjour_moderne
{
    service_resolver::service_resolver (const discovered_service& service,
                                        const resolved_service::handler& resolved_service_handler)
        : implementation {std::make_unique<Implementation> (service, resolved_service_handler)}
    {
    }

    service_resolver::~service_resolver()
    {
    }
} // namespace bonjour_moderne
