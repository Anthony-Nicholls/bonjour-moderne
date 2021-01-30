
#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace bonjour_moderne
{

//==============================================================================
struct service_host
{
    const std::string name {};
    const uint16_t port {};
};

using error_handler = std::function<void(std::exception_ptr)>;

//==============================================================================
class txt_record
{
public:
    txt_record();
    txt_record (const txt_record& other);
    txt_record (const std::unordered_map<std::string, std::string>& values);
    txt_record (const void* data, uint16_t size);
    ~txt_record();

    const void* data() const;
    uint16_t size() const;

    bool has_value (const std::string& key) const;
    std::string get_value (const std::string& key) const;
    size_t num_values() const;

    void set_value (const std::string& key,
                    const std::string& value);

    void remove_value (const std::string& key);

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

//==============================================================================
struct advertisable_service
{
    const std::string name {};
    const std::string type {};
    const std::string domain {};
    const uint32_t interface_index {};
    const service_host host {};
    const txt_record txt_record {};
};

struct advertised_service
{
    const std::string name {};
    const std::string type {};
    const std::string domain {};

    using handler = std::function<void(const advertised_service& service,
                                       const bool was_added)>;
};

class service_advertiser
{
public:
    service_advertiser (const advertisable_service& service,
                        const advertised_service::handler service_handler,
                        const error_handler error_handler);

    ~service_advertiser();

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

//==============================================================================
struct discoverable_service
{
    const std::string type {};
    const std::string domain {};
    const uint32_t interface_index {};
};

struct resolved_service
{
    const std::string name {};
    const service_host host {};
    const txt_record txt_record {};

    using handler = std::function<void(const resolved_service& sercice,
                                       const bool more_coming)>;
};

class discovered_service
{
public:
    discovered_service (const discovered_service&);
    ~discovered_service();

    const std::string name {};
    const std::string type {};
    const std::string domain {};
    const uint32_t interface_index {};

    void resolve (const resolved_service::handler resolved_service_handler,
                  const error_handler error_handler);

    using handler = std::function<void(const discovered_service& service,
                                       const bool was_added,
                                       const bool more_coming)>;

private:
    discovered_service (const std::string& name,
                        const std::string& type,
                        const std::string& domain,
                        const uint32_t interface_index);

    friend class service_browser;

    class impl;
    std::unique_ptr<impl> pimpl;
};

class service_browser
{
public:
    service_browser (const discoverable_service& service,
                     const discovered_service::handler service_handler,
                     const error_handler error_handler);

    ~service_browser();

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

uint32_t daemon_api_version();

} // bonjour_moderne
