
#pragma once

#include <string>

namespace bonjour_moderne
{
    class service_domain
    {
    public:
        static const service_domain any;
        static const service_domain local;

        explicit service_domain (const std::string& domain) noexcept;

        bool is_empty() const noexcept;
        std::string to_string() const noexcept;
        const char* to_c_str() const noexcept;

    private:
        std::string str;
    };

    bool operator== (const service_domain& lhs, const service_domain& rhs) noexcept;
    bool operator!= (const service_domain& lhs, const service_domain& rhs) noexcept;
} // namespace bonjour_moderne
