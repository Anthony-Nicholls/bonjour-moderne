
#pragma once

#include <string>

namespace bonjour_moderne
{
    class service_hostname
    {
    public:
        static const service_hostname auto_resolve;

        explicit service_hostname (const std::string& hostname) noexcept;

        bool is_empty() const noexcept;
        std::string to_string() const noexcept;
        const char* to_c_str() const noexcept;

    private:
        const std::string str;
    };

    bool operator== (const service_hostname& lhs, const service_hostname& rhs) noexcept;
    bool operator!= (const service_hostname& lhs, const service_hostname& rhs) noexcept;
} // namespace bonjour_moderne
