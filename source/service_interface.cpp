
#include <bonjour_moderne/service_interface.h>

#include <dns_sd.h>

namespace bonjour_moderne
{
    const service_interface service_interface::any {kDNSServiceInterfaceIndexAny};
    const service_interface service_interface::local {kDNSServiceInterfaceIndexLocalOnly};
    const service_interface service_interface::unicast {kDNSServiceInterfaceIndexUnicast};
    const service_interface service_interface::p2p {kDNSServiceInterfaceIndexP2P};
    const service_interface service_interface::ble {kDNSServiceInterfaceIndexBLE};

    service_interface::service_interface (uint32_t interface_index) noexcept
        : index {interface_index}
    {
    }

    uint32_t service_interface::to_unint32() const noexcept
    {
        return index;
    }

    bool operator== (const service_interface& lhs, const service_interface& rhs) noexcept
    {
        return lhs.to_unint32() == rhs.to_unint32();
    }

    bool operator!= (const service_interface& lhs, const service_interface& rhs) noexcept
    {
        return ! (lhs == rhs);
    }
} // namespace bonjour_moderne
