
#include <bonjour_moderne/service_fullname.h>

namespace bonjour_moderne
{
    service_fullname::service_fullname (const std::string& fullname) noexcept
        : str {fullname}
    {
    }

    std::string service_fullname::to_string() const noexcept
    {
        return str;
    }

    bool operator== (const service_fullname& lhs, const service_fullname& rhs) noexcept
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_fullname& lhs, const service_fullname& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
