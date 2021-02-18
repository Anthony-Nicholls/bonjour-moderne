
#pragma once

#include "advertisable_service.h"
#include "advertised_service.h"

#include <memory>

namespace bonjour_moderne
{
    class service_advertiser
    {
    public:
        service_advertiser (const advertisable_service& service,
                            const advertised_service::handler& service_handler) noexcept;

        ~service_advertiser();

    private:
        class Implementation;
        std::unique_ptr<Implementation> implementation;
    };
} // namespace bonjour_moderne
