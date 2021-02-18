
#pragma once

#include <cstdint>

namespace bonjour_moderne
{
    class service_interface
    {
    public:
        static const service_interface any;
        static const service_interface local;
        static const service_interface unicast;
        static const service_interface p2p;
        static const service_interface ble;

        explicit service_interface (uint32_t interface_index) noexcept;

        uint32_t to_unint32() const noexcept;

    private:
        uint32_t index;
    };

    bool operator== (const service_interface& lhs, const service_interface& rhs) noexcept;
    bool operator!= (const service_interface& lhs, const service_interface& rhs) noexcept;
} // namespace bonjour_moderne
