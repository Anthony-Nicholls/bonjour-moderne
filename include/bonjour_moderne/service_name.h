
#pragma once

#include <string>

namespace bonjour_moderne
{
    class service_name
    {
    public:
        explicit service_name (const std::string& name) noexcept;

        bool is_empty() const noexcept;
        std::string to_string() const noexcept;
        const char* to_c_str() const noexcept;

    private:
        std::string str;
    };

    bool operator== (const service_name& lhs, const service_name& rhs) noexcept;
    bool operator!= (const service_name& lhs, const service_name& rhs) noexcept;
} // namespace bonjour_moderne
