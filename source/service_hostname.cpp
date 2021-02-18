
#include <bonjour_moderne/service_hostname.h>

namespace bonjour_moderne
{
    const service_hostname service_hostname::auto_resolve {""};

    service_hostname::service_hostname (const std::string& hostname) noexcept
        : str {hostname}
    {
    }

    bool service_hostname::is_empty() const noexcept
    {
        return str.empty();
    }

    std::string service_hostname::to_string() const noexcept
    {
        return str;
    }

    const char* service_hostname::to_c_str() const noexcept
    {
        return str.c_str();
    }

    bool operator== (const service_hostname& lhs, const service_hostname& rhs) noexcept
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_hostname& lhs, const service_hostname& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
