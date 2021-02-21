
#include <bonjour_moderne/service_domain.h>
#include "string.h"

#include <dns_sd.h>

namespace bonjour_moderne
{
    const service_domain service_domain::any {""};
    const service_domain service_domain::local {"local."};

    service_domain::service_domain (const std::string& domain) noexcept
        : str {domain.empty() ? domain : string {domain}.with_ending (".")}
    {
    }

    bool service_domain::is_any() const noexcept
    {
        return str.empty() || str == ".";
    }

    std::string service_domain::to_string() const noexcept
    {
        return str;
    }

    const char* service_domain::to_c_str() const noexcept
    {
        return str.c_str();
    }

    bool operator== (const service_domain& lhs,
                     const service_domain& rhs) noexcept
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_domain& lhs,
                     const service_domain& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
