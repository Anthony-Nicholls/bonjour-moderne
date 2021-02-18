
#pragma once

#include "discoverable_service.h"
#include "discovered_service.h"

#include <memory>

namespace bonjour_moderne
{
    class service_browser
    {
    public:
        service_browser (const discoverable_service& service,
                         const discovered_service::handler& service_handler);

        ~service_browser();

    private:
        class Implementation;
        std::unique_ptr<Implementation> implementation;
    };
} // namespace bonjour_moderne
