
#include <bonjour_moderne/service_type.h>

#include "string.h"

namespace bonjour_moderne
{
    service_type::service_type (const std::string& type_name) noexcept
        : str {string {type_name}
                   .with_allowed_characters ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-")
                   .with_maximum_length (15)
                   .with_begining ("_")
                   .with_ending (".")}
    {
    }

    std::string service_type::to_string() const noexcept
    {
        return str;
    }

    const char* service_type::to_c_str() const noexcept
    {
        return str.c_str();
    }

    bool operator== (const service_type& lhs, const service_type& rhs)
    {
        return lhs.to_string() == rhs.to_string();
    }

    bool operator!= (const service_type& lhs, const service_type& rhs)
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
