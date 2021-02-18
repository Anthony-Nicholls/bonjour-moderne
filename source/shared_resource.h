
#pragma once

#include "string.h"

#include <dns_sd.h>

#include <atomic>
#include <memory>
#include <thread>

namespace bonjour_moderne
{
    template<typename Type>
    class shared_resource
    {
    private:
        struct resource_wrapper
        {
            Type resource {};
        };

    public:
        shared_resource()
        {
            static std::mutex shared_mutex;
            static std::weak_ptr<resource_wrapper> shared_instance;

            const std::lock_guard<std::mutex> lock {shared_mutex};
            instance = shared_instance.lock();

            if (instance == nullptr)
            {
                instance = std::make_shared<resource_wrapper>();
                shared_instance = instance;
            }
        }

        Type const* operator->() const
        {
            return &instance->resource;
        }

    private:
        std::shared_ptr<resource_wrapper> instance;
    };
} // namespace bonjour_moderne
