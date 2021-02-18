
#include "dns_service.h"

#include <mutex>
#include <thread>

namespace bonjour_moderne
{
    dns_service::dns_service()
    {
        DNSServiceCreateConnection (&ref);

        thread = std::thread ([this]() {
            const auto socket {DNSServiceRefSockFD (ref)};
            fd_set readfds {};

            while (! thread_should_exit)
            {
                FD_ZERO (&readfds);
                FD_SET (socket, &readfds);

                if (select (socket + 1, &readfds, nullptr, nullptr, nullptr) > 0 && ! thread_should_exit)
                {
                    if (FD_ISSET (socket, &readfds) && ! thread_should_exit)
                        DNSServiceProcessResult (ref);
                }
            }
        });
    }

    dns_service::~dns_service()
    {
        thread_should_exit = true;
        DNSServiceRefDeallocate (ref);
        thread.join();
    }

    dns_service::shared_connection::~shared_connection()
    {
        DNSServiceRefDeallocate (ref);
    }

    DNSServiceRef* dns_service::shared_connection::get()
    {
        return &ref;
    }
} // namespace bonjour_moderne
