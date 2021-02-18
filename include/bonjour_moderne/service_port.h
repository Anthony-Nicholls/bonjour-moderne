
#pragma once

#include <cstdint>

namespace bonjour_moderne
{
    class service_port
    {
    public:
        static const service_port placeholder;

        explicit service_port (uint16_t port) noexcept;

        uint16_t to_uint16() const noexcept;

    private:
        const uint16_t index;
    };

    bool operator== (const service_port& lhs, const service_port& rhs) noexcept;
    bool operator!= (const service_port& lhs, const service_port& rhs) noexcept;
} // namespace bonjour_moderne
