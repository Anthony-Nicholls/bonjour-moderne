
#pragma once

#include "shared_resource.h"

#include <dns_sd.h>

#include <atomic>
#include <thread>

namespace bonjour_moderne
{
    class dns_service
    {
    public:
        ~dns_service();

        class shared_connection
        {
        public:
            ~shared_connection();

            DNSServiceRef* get();

        private:
            shared_resource<dns_service> dns_service {};
            DNSServiceRef ref {dns_service->ref};
        };

    private:
        friend shared_resource<dns_service>;

        dns_service();
        DNSServiceRef ref {nullptr};
        std::atomic<bool> thread_should_exit {false};
        std::thread thread {};
    };
} // namespace bonjour_moderne
