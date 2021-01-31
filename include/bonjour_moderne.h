
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace bonjour_moderne
{
    //==============================================================================
    class service_name
    {
    public:
        explicit service_name (const std::string& name) : value {name} {}
        bool is_empty() const { return value.empty(); }
        std::string to_string() const { return value; }
        const char* to_c_str() const { return value.c_str(); }

    private:
        const std::string value;
    };

    inline bool operator==(const service_name& lhs, const service_name& rhs){ return lhs.to_string() == rhs.to_string(); }
    inline bool operator!=(const service_name& lhs, const service_name& rhs){ return !(lhs == rhs); }

    class service_type
    {
    public:
        class udp
        {
        public:
            explicit udp (const std::string& indentifier);
            std::string to_string() const { return value; }

        private:
            const std::string value;
        };

        class tcp
        {
        public:
            explicit tcp (const std::string& indentifier);
            std::string to_string() const { return value; }
        private:
            const std::string value;
        };

        service_type (const udp& udp) : value {udp.to_string()} {}
        service_type (const tcp& tcp) : value {tcp.to_string()} {}
        explicit service_type (const std::string& type) : value {type} {}

        std::string to_string() const { return value; }
        const char* to_c_str() const { return value.c_str(); }

    private:
        const std::string value;
    };

    inline bool operator==(const service_type& lhs, const service_type& rhs){ return lhs.to_string() == rhs.to_string(); }
    inline bool operator!=(const service_type& lhs, const service_type& rhs){ return !(lhs == rhs); }

    class service_hostname
    {
    public:
        static const service_hostname auto_resolve;

        explicit service_hostname (const std::string& name) : value {name} {}
        bool is_empty() const { return value.empty(); }
        std::string to_string() const { return value; }
        const char* to_c_str() const { return value.c_str(); }

    private:
        const std::string value;
    };

    inline bool operator==(const service_hostname& lhs, const service_hostname& rhs){ return lhs.to_string() == rhs.to_string(); }
    inline bool operator!=(const service_hostname& lhs, const service_hostname& rhs){ return !(lhs == rhs); }

    class service_port
    {
    public:
        static const service_port placeholder;

        explicit service_port (uint16_t port) : value {port} {}
        uint16_t to_uint16() const { return value; }

    private:
        const uint16_t value;
    };

    inline bool operator==(const service_port& lhs, const service_port& rhs){ return lhs.to_uint16() == rhs.to_uint16(); }
    inline bool operator!=(const service_port& lhs, const service_port& rhs){ return !(lhs == rhs); }

    class service_host
    {
    public:
        service_host (service_port port) : port {port} {}
        service_host (service_hostname name, service_port port) : name {name}, port {port} {}

        const service_hostname name {service_hostname::auto_resolve};
        const service_port port;
    };

    inline bool operator==(const service_host& lhs, const service_host& rhs){ return lhs.name == rhs.name && lhs.port == rhs.port; }
    inline bool operator!=(const service_host& lhs, const service_host& rhs){ return !(lhs == rhs); }

    class service_domain
    {
    public:
        static const service_domain any;
        static const service_domain local;

        explicit service_domain (const std::string& name) : value {name} {}
        bool is_empty() const { return value.empty(); }
        std::string to_string() const { return value; }
        const char* to_c_str() const { return value.c_str(); }

    private:
        const std::string value;
    };

    inline bool operator==(const service_domain& lhs, const service_domain& rhs){ return lhs.to_string() == rhs.to_string(); }
    inline bool operator!=(const service_domain& lhs, const service_domain& rhs){ return !(lhs == rhs); }

    class service_interface
    {
    public:
        static const service_interface any;
        static const service_interface local;
        static const service_interface unicast;
        static const service_interface p2p;
        static const service_interface ble;

        explicit service_interface (uint32_t interface_index) : value {interface_index} {}
        uint32_t to_unint32() const { return value; }

    private:
        uint32_t value;
    };

    inline bool operator==(const service_interface& lhs, const service_interface& rhs){ return lhs.to_unint32() == rhs.to_unint32(); }
    inline bool operator!=(const service_interface& lhs, const service_interface& rhs){ return !(lhs == rhs); }

    class service_fullname
    {
    public:
        explicit service_fullname (const std::string& name) : value {name} {}
        std::string to_string() const { return value; }

    private:
        const std::string value;
    };

    inline bool operator==(const service_fullname& lhs, const service_fullname& rhs){ return lhs.to_string() == rhs.to_string(); }
    inline bool operator!=(const service_fullname& lhs, const service_fullname& rhs){ return !(lhs == rhs); }

    class service_txt_record
    {
    public:
        service_txt_record();
        service_txt_record (const service_txt_record& other);
        service_txt_record (const std::unordered_map<std::string, std::string>& values);
        service_txt_record (const void* data, uint16_t size);
        ~service_txt_record();

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
        const service_name name;
        const service_type type;
        const service_host host;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};
        const service_txt_record txt_record {};
    };

    struct advertised_service
    {
        const service_name name;
        const service_type type;
        const service_domain domain {service_domain::any};

        using handler = std::function<void (const advertised_service& service,
                                            const bool was_added)>;
    };

    class service_advertiser
    {
    public:
        service_advertiser (const advertisable_service& service,
                            const advertised_service::handler service_handler);

        ~service_advertiser();

    private:
        class impl;
        std::unique_ptr<impl> pimpl;
    };

    //==============================================================================
    struct discoverable_service
    {
        const service_type type;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};
    };

    struct resolved_service
    {
        const service_fullname fullname;
        const service_host host;
        const service_txt_record txt_record {};

        using handler = std::function<void (const resolved_service& sercice,
                                            const bool more_coming)>;
    };

    class discovered_service
    {
    public:
        discovered_service (const discovered_service& other);
        ~discovered_service();

        const service_name name;
        const service_type type;
        const service_domain domain {service_domain::any};
        const service_interface interface {service_interface::any};

        void resolve (const resolved_service::handler resolved_service_handler);

        using handler = std::function<void (const discovered_service& service,
                                            const bool was_added,
                                            const bool more_coming)>;

    private:
        discovered_service (const service_name& name,
                            const service_type& type,
                            const service_domain& domain,
                            const service_interface& interface);

        friend class service_browser;

        class impl;
        std::unique_ptr<impl> pimpl;
    };

    class service_browser
    {
    public:
        service_browser (const discoverable_service& service,
                         const discovered_service::handler service_handler);

        ~service_browser();

    private:
        class impl;
        std::unique_ptr<impl> pimpl;
    };

    //==========================================================================
    uint32_t daemon_api_version();

} // namespace bonjour_moderne
