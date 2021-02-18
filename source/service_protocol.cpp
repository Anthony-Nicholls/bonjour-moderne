
#include <bonjour_moderne/service_protocol.h>

#include "string.h"

namespace bonjour_moderne
{
    const service_protocol service_protocol::udp {"_udp."};
    const service_protocol service_protocol::tcp {"_tcp."};
    const service_protocol service_protocol::sctp {"_sctp."};
    const service_protocol service_protocol::dccp {"_dccp."};

    service_protocol::service_protocol (const std::string& protocol) noexcept
        : str {string {protocol}
                   .with_begining ("_")
                   .with_ending (".")}
    {
    }

    bool service_protocol::is_empty() const noexcept
    {
        return str.empty();
    }

    std::string service_protocol::to_string() const noexcept
    {
        return str;
    }

    const char* service_protocol::to_c_str() const noexcept
    {
        return str.c_str();
    }

    bool operator== (const service_protocol& lhs, const service_protocol& rhs) noexcept
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_protocol& lhs, const service_protocol& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
