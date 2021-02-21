
#pragma once

#include <string>

namespace bonjour_moderne
{
    class service_fullname
    {
    public:
        explicit service_fullname (const std::string& fullname) noexcept;
        
        std::string to_string() const noexcept;

    private:
        std::string str;
    };

    bool operator== (const service_fullname& lhs, const service_fullname& rhs) noexcept;
    bool operator!= (const service_fullname& lhs, const service_fullname& rhs) noexcept;
} // namespace bonjour_moderne
