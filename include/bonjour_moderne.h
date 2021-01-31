
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace bonjour_moderne
{
    namespace service
    {
        class name
        {
        public:
            explicit name (const std::string& name) : value {name} {}
            bool is_empty() const { return value.empty(); }
            std::string to_string() const { return value; }
            const char* to_c_str() const { return value.c_str(); }

        private:
            const std::string value;
        };

        inline bool operator==(const name& lhs, const name& rhs){ return lhs.to_string() == rhs.to_string(); }
        inline bool operator!=(const name& lhs, const name& rhs){ return !(lhs == rhs); }

        class type
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

            type (const udp& udp) : value {udp.to_string()} {}
            type (const tcp& tcp) : value {tcp.to_string()} {}
            explicit type (const std::string& type) : value {type} {}

            std::string to_string() const { return value; }
            const char* to_c_str() const { return value.c_str(); }

        private:
            const std::string value;
        };

        inline bool operator==(const type& lhs, const type& rhs){ return lhs.to_string() == rhs.to_string(); }
        inline bool operator!=(const type& lhs, const type& rhs){ return !(lhs == rhs); }

        class hostname
        {
        public:
            static const hostname auto_resolve;

            explicit hostname (const std::string& name) : value {name} {}
            bool is_empty() const { return value.empty(); }
            std::string to_string() const { return value; }
            const char* to_c_str() const { return value.c_str(); }

        private:
            const std::string value;
        };

        inline bool operator==(const hostname& lhs, const hostname& rhs){ return lhs.to_string() == rhs.to_string(); }
        inline bool operator!=(const hostname& lhs, const hostname& rhs){ return !(lhs == rhs); }

        class port
        {
        public:
            static const port placeholder;

            explicit port (uint16_t port) : value {port} {}
            uint16_t to_uint16() const { return value; }

        private:
            const uint16_t value;
        };

        inline bool operator==(const port& lhs, const port& rhs){ return lhs.to_uint16() == rhs.to_uint16(); }
        inline bool operator!=(const port& lhs, const port& rhs){ return !(lhs == rhs); }

        class host
        {
        public:
            host (port port) : port {port} {}
            host (hostname name, port port) : name {name}, port {port} {}

            const hostname name {hostname::auto_resolve};
            const port port;
        };

        inline bool operator==(const host& lhs, const host& rhs){ return lhs.name == rhs.name && lhs.port == rhs.port; }
        inline bool operator!=(const host& lhs, const host& rhs){ return !(lhs == rhs); }

        class domain
        {
        public:
            static const domain any;
            static const domain local;

            explicit domain (const std::string& name) : value {name} {}
            bool is_empty() const { return value.empty(); }
            std::string to_string() const { return value; }
            const char* to_c_str() const { return value.c_str(); }

        private:
            const std::string value;
        };

        inline bool operator==(const domain& lhs, const domain& rhs){ return lhs.to_string() == rhs.to_string(); }
        inline bool operator!=(const domain& lhs, const domain& rhs){ return !(lhs == rhs); }

        class interface
        {
        public:
            static const interface any;
            static const interface local;
            static const interface unicast;
            static const interface p2p;
            static const interface ble;

            explicit interface (uint32_t interface_index) : value {interface_index} {}
            uint32_t to_unint32() const { return value; }

        private:
            uint32_t value;
        };

        inline bool operator==(const interface& lhs, const interface& rhs){ return lhs.to_unint32() == rhs.to_unint32(); }
        inline bool operator!=(const interface& lhs, const interface& rhs){ return !(lhs == rhs); }

        class fullname
        {
        public:
            explicit fullname (const std::string& name) : value {name} {}
            type type() const;
            domain domain() const;
            std::string to_string() const { return value; }

        private:
            const std::string value;
        };

        inline bool operator==(const fullname& lhs, const fullname& rhs){ return lhs.to_string() == rhs.to_string(); }
        inline bool operator!=(const fullname& lhs, const fullname& rhs){ return !(lhs == rhs); }

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
    } // namespace service

    using service_name = service::name;
    using service_type = service::type;
    using service_domain = service::domain;
    using service_fullname = service::fullname;
    using service_hostname = service::hostname;
    using service_port = service::port;
    using service_host = service::host;
    using service_interface = service::interface;
    using service_txt_record = service::txt_record;

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
