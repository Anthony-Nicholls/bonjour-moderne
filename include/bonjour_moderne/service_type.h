
#pragma once

#include <string>

namespace bonjour_moderne
{
    /** The service type is an IANA-registered application protocol name.

        To find existing services or register new services visit
        http://www.dns-sd.org/ServiceTypes.html.

        An identifier should be 1-15 characters in length (additional charcters
        will be truncated). Only alphanumeric characters and hypens are
        supported (other characters will be stripped).
     */
    class service_type
    {
    public:
        explicit service_type (const std::string& type_name) noexcept;

        std::string to_string() const noexcept;
        const char* to_c_str() const noexcept;

    private:
        const std::string str;
    };

    bool operator== (const service_type& lhs, const service_type& rhs);
    bool operator!= (const service_type& lhs, const service_type& rhs);
} // namespace bonjour_moderne
