
#include <bonjour_moderne/daemon_api_version.h>

#include <dns_sd.h>

namespace bonjour_moderne
{
    uint32_t daemon_api_version() noexcept
    {
        uint32_t api_version {0};
        uint32_t api_version_size {static_cast<uint32_t> (sizeof (api_version))};
        DNSServiceGetProperty (kDNSServiceProperty_DaemonVersion, &api_version, &api_version_size);
        return api_version;
    }
} // namespace bonjour_moderne
