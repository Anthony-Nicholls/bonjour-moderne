
#include <bonjour_moderne.h>

#include <atomic>
#include <dns_sd.h>
#include <thread>
#include <vector>

//==============================================================================
class shared_dns_service
{
private:
    class service
    {
    public:
        service()
        {
            DNSServiceCreateConnection (&service_ref);

            thread = std::make_unique<std::thread> ([&]() {
                const auto socket {DNSServiceRefSockFD (service_ref)};
                fd_set readfds {};

                while (! thread_should_exit)
                {
                    FD_ZERO (&readfds);
                    FD_SET (socket, &readfds);

                    if (select (socket + 1, &readfds, nullptr, nullptr, nullptr) > 0 && ! thread_should_exit)
                    {
                        if (FD_ISSET (socket, &readfds) && ! thread_should_exit)
                            DNSServiceProcessResult (service_ref);
                    }
                }
            });
        }

        DNSServiceRef& get_ref()
        {
            return service_ref;
        }

        ~service()
        {
            DNSServiceRefDeallocate (service_ref);
            thread_should_exit = true;
            thread->join();
        }

    private:
        DNSServiceRef service_ref {nullptr};
        std::unique_ptr<std::thread> thread;
        std::atomic<bool> thread_should_exit {false};
    };

public:
    shared_dns_service()
    {
        static std::mutex shared_service_instance_mutex;
        static std::weak_ptr<service> shared_service_instance;

        std::lock_guard<std::mutex> lock {shared_service_instance_mutex};
        shared_service = shared_service_instance.lock();

        if (shared_service == nullptr)
        {
            shared_service = std::make_shared<service>();
            shared_service_instance = shared_service;
        }

        service_ref = shared_service->get_ref();
    }

    DNSServiceRef& get_ref()
    {
        return service_ref;
    }

    ~shared_dns_service()
    {
        DNSServiceRefDeallocate (service_ref);
    }

private:
    DNSServiceRef service_ref;
    std::shared_ptr<service> shared_service;
};

namespace bonjour_moderne
{
    //==============================================================================
    // Advertise a service
    //==============================================================================

    class service_advertiser::impl
    {
    public:
        impl (const advertisable_service& service,
              const advertised_service::handler service_handler,
              const error_handler error_handler)
            : service_handler {service_handler}
            , error_handler {error_handler}
        {
            DNSServiceRegister (&shared_dns_service.get_ref(),
                                kDNSServiceFlagsShareConnection,
                                service.interface_index,
                                service.name.empty() ? nullptr : service.name.c_str(),
                                service.type.c_str(),
                                service.domain.empty() ? nullptr : service.domain.c_str(),
                                service.host.name.empty() ? nullptr : service.host.name.c_str(),
                                service.host.port,
                                service.txt_record.size(),
                                service.txt_record.data(),
                                &dns_service_register_reply,
                                this);
        }

    private:
        static void dns_service_register_reply (DNSServiceRef sdRef,
                                                DNSServiceFlags flags,
                                                DNSServiceErrorType errorCode,
                                                const char* name,
                                                const char* regtype,
                                                const char* domain,
                                                void* context)
        {
            if (const auto pimpl = static_cast<impl*> (context))
            {
                if (errorCode == kDNSServiceErr_NoError)
                {
                    pimpl->service_handler ({name, regtype, domain},
                                            flags & kDNSServiceFlagsAdd);
                }
            }
        }

        const advertised_service::handler service_handler;
        const error_handler error_handler;
        shared_dns_service shared_dns_service;
    };

    service_advertiser::service_advertiser (const advertisable_service& service,
                                            const advertised_service::handler service_handler,
                                            const error_handler error_handler)
        : pimpl {std::make_unique<impl> (service, service_handler, error_handler)}
    {
    }

    service_advertiser::~service_advertiser() = default;

    //==============================================================================
    class txt_record::impl
    {
    public:
        impl()
        {
            TXTRecordCreate (&txt_record_ref, 0, nullptr);
        }

        impl (const impl& other)
            : impl (other.data(), other.size())
        {
        }

        impl (const void* data, const uint16_t size)
            : impl()
        {
            for (uint16_t index {0}; index < TXTRecordGetCount (size, data); ++index)
            {
                constexpr uint16_t key_buffer_max_size {256};
                char key_buffer[key_buffer_max_size] {};

                uint8_t value_size {0};
                const void* value_ptr {nullptr};

                TXTRecordGetItemAtIndex (size,
                                         data,
                                         index,
                                         key_buffer_max_size,
                                         key_buffer,
                                         &value_size,
                                         &value_ptr);

                TXTRecordSetValue (&txt_record_ref,
                                   key_buffer,
                                   value_size,
                                   value_ptr);
            }
        }

        ~impl()
        {
            TXTRecordDeallocate (&txt_record_ref);
        }

        bool has_value (const std::string& key) const
        {
            return TXTRecordContainsKey (size(), data(), key.c_str()) != 0;
        }

        std::string get_value (const std::string& key) const
        {
            uint8_t value_size {0};
            const void* value_ptr {TXTRecordGetValuePtr (size(), data(), key.c_str(), &value_size)};
            return {static_cast<const char*> (value_ptr), static_cast<size_t> (value_size)};
        }

        size_t num_values() const
        {
            return static_cast<size_t> (TXTRecordGetCount (size(), data()));
        }

        void set_value (const std::string& key,
                        const std::string& value)
        {
            TXTRecordSetValue (&txt_record_ref,
                               key.c_str(),
                               static_cast<uint8_t> (value.size()),
                               value.c_str());
        }

        void remove_value (const std::string& key)
        {
            TXTRecordRemoveValue (&txt_record_ref, key.c_str());
        }

        const void* data() const
        {
            return TXTRecordGetBytesPtr (&txt_record_ref);
        }

        uint16_t size() const
        {
            return TXTRecordGetLength (&txt_record_ref);
        }

    private:
        TXTRecordRef txt_record_ref {};
    };

    txt_record::txt_record()
        : pimpl {std::make_unique<impl>()}
    {
    }

    txt_record::txt_record (const txt_record& other)
        : pimpl {std::make_unique<impl> (*other.pimpl)}
    {
    }

    txt_record::txt_record (const std::unordered_map<std::string, std::string>& values)
        : txt_record()
    {
        for (const auto& value : values)
            set_value (value.first, value.second);
    }

    txt_record::txt_record (const void* data, uint16_t size)
        : pimpl {std::make_unique<impl> (data, size)}
    {
    }

    txt_record::~txt_record() = default;

    const void* txt_record::data() const
    {
        return pimpl->data();
    }

    uint16_t txt_record::size() const
    {
        return pimpl->size();
    }

    bool txt_record::has_value (const std::string& key) const
    {
        return pimpl->has_value (key);
    }

    std::string txt_record::get_value (const std::string& key) const
    {
        return pimpl->get_value (key);
    }

    size_t txt_record::num_values() const
    {
        return pimpl->num_values();
    }

    void txt_record::set_value (const std::string& key,
                                const std::string& value)
    {
        pimpl->set_value (key, value);
    }

    void txt_record::remove_value (const std::string& key)
    {
        pimpl->remove_value (key);
    }

    //==============================================================================
    // Discover advertised services
    //==============================================================================

    class service_browser::impl
    {
    public:
        impl (const discoverable_service& service,
              const discovered_service::handler service_handler,
              const error_handler error_handler)
            : service_handler {service_handler}
            , error_handler {error_handler}
        {
            DNSServiceBrowse (&shared_dns_service.get_ref(),
                              kDNSServiceFlagsShareConnection,
                              service.interface_index,
                              service.type.c_str(),
                              service.domain.empty() ? nullptr : service.domain.c_str(),
                              &dns_service_browse_reply,
                              this);
        }

    private:
        static void dns_service_browse_reply (DNSServiceRef sdRef,
                                              DNSServiceFlags flags,
                                              uint32_t interfaceIndex,
                                              DNSServiceErrorType errorCode,
                                              const char* serviceName,
                                              const char* regtype,
                                              const char* replyDomain,
                                              void* context)
        {
            if (const auto pimpl = static_cast<impl*> (context))
            {
                if (errorCode == kDNSServiceErr_NoError)
                {
                    pimpl->service_handler ({serviceName, regtype, replyDomain, interfaceIndex},
                                            flags & kDNSServiceFlagsAdd,
                                            flags & kDNSServiceFlagsMoreComing);
                }
            }
        }

        const discovered_service::handler service_handler;
        const error_handler error_handler;
        shared_dns_service shared_dns_service;
    };

    service_browser::service_browser (const discoverable_service& service,
                                      const discovered_service::handler service_handler,
                                      const error_handler error_handler)
        : pimpl {std::make_unique<impl> (service, service_handler, error_handler)}
    {
    }

    service_browser::~service_browser() = default;

    //==============================================================================
    // Resolve a discovered service
    //==============================================================================

    class discovered_service::impl
    {
    public:
        impl (const discovered_service& service,
              const resolved_service::handler service_handler,
              const error_handler error_handler)
            : service_handler {service_handler}
            , error_handler {error_handler}
        {
            DNSServiceResolve (&shared_dns_service.get_ref(),
                               kDNSServiceFlagsShareConnection,
                               service.interface_index,
                               service.name.c_str(),
                               service.type.c_str(),
                               service.domain.c_str(),
                               &dns_service_resolve_reply,
                               this);
        }

    private:
        static void dns_service_resolve_reply (DNSServiceRef sdRef,
                                               DNSServiceFlags flags,
                                               uint32_t interfaceIndex,
                                               DNSServiceErrorType errorCode,
                                               const char* fullname,
                                               const char* hosttarget,
                                               uint16_t port,
                                               uint16_t txtLen,
                                               const unsigned char* txtRecord,
                                               void* context)
        {
            if (const auto pimpl = static_cast<impl*> (context))
            {
                if (errorCode == kDNSServiceErr_NoError)
                {
                    pimpl->service_handler ({fullname, {hosttarget, port}, {txtRecord, txtLen}},
                                            flags & kDNSServiceFlagsMoreComing);
                }
            }
        }

        const resolved_service::handler service_handler;
        const error_handler error_handler;
        shared_dns_service shared_dns_service;
    };

    discovered_service::discovered_service (const std::string& name,
                                            const std::string& type,
                                            const std::string& domain,
                                            const uint32_t interface_index)
        : name {name}
        , type {type}
        , domain {domain}
        , interface_index {interface_index}
    {
    }

    discovered_service::discovered_service (const discovered_service& other)
        : name {other.name}
        , type {other.type}
        , domain {other.domain}
        , interface_index {other.interface_index}
    {
    }

    discovered_service::~discovered_service() = default;

    void discovered_service::resolve (const resolved_service::handler service_handler,
                                      const error_handler error_handler)
    {
        pimpl = std::make_unique<impl> (*this, service_handler, error_handler);
    }

    //==============================================================================
    uint32_t daemon_api_version()
    {
        uint32_t api_version {0};
        uint32_t api_version_size {static_cast<uint32_t> (sizeof (api_version))};
        DNSServiceGetProperty (kDNSServiceProperty_DaemonVersion, &api_version, &api_version_size);
        return api_version;
    }

} // namespace bonjour_moderne
