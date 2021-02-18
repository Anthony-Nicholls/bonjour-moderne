
#pragma once

#include "discovered_service.h"
#include "resolved_service.h"

#include <memory>

namespace bonjour_moderne
{
    class service_resolver
    {
    public:
        service_resolver (const discovered_service& service,
                          const resolved_service::handler& resolved_service_handler);

        ~service_resolver();

    private:
        class Implementation;
        std::unique_ptr<Implementation> implementation;
    };
} // namespace bonjour_moderne
