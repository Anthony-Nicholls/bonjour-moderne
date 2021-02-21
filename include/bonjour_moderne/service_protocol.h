
#pragma once

#include <string>

namespace bonjour_moderne
{
    class service_protocol
    {
    public:
        static const service_protocol udp;
        static const service_protocol tcp;
        static const service_protocol sctp;
        static const service_protocol dccp;

        explicit service_protocol (const std::string& protocol) noexcept;

        bool is_empty() const noexcept;
        std::string to_string() const noexcept;
        const char* to_c_str() const noexcept;

    private:
        std::string str;
    };

    bool operator== (const service_protocol& lhs, const service_protocol& rhs) noexcept;
    bool operator!= (const service_protocol& lhs, const service_protocol& rhs) noexcept;
} // namespace bonjour_moderne
