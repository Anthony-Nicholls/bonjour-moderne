
#include <bonjour_moderne/service_name.h>

namespace bonjour_moderne
{
    service_name::service_name (const std::string& name) noexcept
        : str {name}
    {
    }

    bool service_name::is_empty() const noexcept
    {
        return str.empty();
    }

    std::string service_name::to_string() const noexcept
    {
        return str;
    }

    const char* service_name::to_c_str() const noexcept
    {
        return str.c_str();
    }

    bool operator== (const service_name& lhs, const service_name& rhs) noexcept
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_name& lhs, const service_name& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
