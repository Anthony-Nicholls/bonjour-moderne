
#include <future>

#include <bonjour_moderne.h>
#include <catch2/catch_all.hpp>

SCENARIO ("advertise, discover, and resolve a service", "[bonjour-moderne]")
{
    using namespace bonjour_moderne;
    using namespace std::chrono_literals;

    GIVEN ("a service to advertise")
    {
        const advertisable_service service_to_advertise
        {
            service_name {"Test Service"},
            service_type::udp {"test"},
            service_port::placeholder,
            service_domain::local,
            service_interface::local
        };

        struct advertised_service_handler_parameters
        {
            const advertised_service service;
            const bool was_added {};
        };

        std::promise<advertised_service_handler_parameters> advertised_service_handler_promise {};

        const auto advertised_service_handler = [&advertised_service_handler_promise](auto service, auto was_added)
        {
            try
            {
                advertised_service_handler_promise.set_value ({service, was_added});
            }
            catch (const std::future_error&)
            {
                // ignore multiple callbacks
            }
        };

        WHEN ("the service is advertised")
        {
            const service_advertiser service_advertiser
            {
                service_to_advertise,
                advertised_service_handler
            };

            auto advertised_service_handler_parameters_future {advertised_service_handler_promise.get_future()};

            THEN ("the handler is notified in <= one second")
            {
                REQUIRE (advertised_service_handler_parameters_future.wait_for (1s) == std::future_status::ready);
            }

            auto advertised_service_handler_parameters {advertised_service_handler_parameters_future.get()};

            THEN ("the was_added parameter in the handler is true")
            {
                REQUIRE (advertised_service_handler_parameters.was_added == true);
            }

            const auto advertised_service = advertised_service_handler_parameters.service;

            THEN ("the advertised name exactly matches")
            {
                REQUIRE (advertised_service.name == service_to_advertise.name);
            }

            THEN ("the advertised type approximately matches")
            {
                REQUIRE (advertised_service.type.to_string().rfind (service_to_advertise.type.to_string(), 0) == 0);
            }

            THEN ("the advertised domain approximately matches")
            {
                REQUIRE (advertised_service.domain.to_string().rfind (service_to_advertise.domain.to_string(), 0) == 0);
            }

            GIVEN ("a matching service to discover")
            {
                const discoverable_service service_to_discover
                {
                    service_to_advertise.type,
                    service_to_advertise.domain,
                    service_to_advertise.interface
                };

                struct discovered_service_handler_parameters
                {
                    const discovered_service service;
                    const bool was_added {};
                    const bool more_coming {};
                };

                std::promise<discovered_service_handler_parameters> discovered_service_handler_promise {};

                const auto discovered_service_handler = [&discovered_service_handler_promise] (auto service,
                                                                                               auto was_added,
                                                                                               auto more_coming)
                {
                    try
                    {
                        discovered_service_handler_promise.set_value ({service, was_added, more_coming});
                    }
                    catch (const std::future_error&)
                    {
                        // ignore multiple callbacks
                    }
                };

                WHEN ("the service is browsed for")
                {
                    service_browser service_browser
                    {
                        service_to_discover,
                        discovered_service_handler
                    };

                    auto discovered_service_handler_parameters_future {discovered_service_handler_promise.get_future()};

                    THEN ("the handler is notified in <= one second")
                    {
                        REQUIRE (discovered_service_handler_parameters_future.wait_for (1s) == std::future_status::ready);
                    }

                    auto discovered_service_handler_parameters {discovered_service_handler_parameters_future.get()};

                    THEN ("the was_added parameter in the handler is true")
                    {
                        REQUIRE (discovered_service_handler_parameters.was_added == true);
                    }

                    THEN ("the service name exactly matches")
                    {
                        REQUIRE (discovered_service_handler_parameters.service.name == service_to_advertise.name);
                    }

                    const auto discovered_service = discovered_service_handler_parameters.service;

                    THEN ("the service type approximately matches")
                    {
                        REQUIRE (discovered_service.type.to_string().rfind (service_to_advertise.type.to_string(), 0) == 0);
                    }

                    THEN ("the service domain approximately matches")
                    {
                        REQUIRE (discovered_service.domain.to_string().rfind (service_to_advertise.domain.to_string(), 0) == 0);
                    }

                    GIVEN ("the discovered service")
                    {
                        auto discovered_service {discovered_service_handler_parameters.service};

                        WHEN ("the service is resolved")
                        {
                            struct resolved_service_handler_parameters
                            {
                                const resolved_service service;
                                const bool more_coming {};
                            };

                            std::promise<resolved_service_handler_parameters> resolved_service_handler_promise {};

                            const auto resolved_service_handler = [&resolved_service_handler_promise](auto service,
                                                                                                      auto more_coming)
                            {
                                try
                                {
                                    resolved_service_handler_promise.set_value ({service, more_coming});
                                }
                                catch (const std::future_error&)
                                {
                                    // ignore multiple callbacks
                                }
                            };

                            discovered_service.resolve (resolved_service_handler);

                            auto resolved_service_handler_parameters_future {resolved_service_handler_promise.get_future()};

                            THEN ("the handler is notified in <= one second")
                            {
                                REQUIRE (resolved_service_handler_parameters_future.wait_for (1s) == std::future_status::ready);
                            }

                            auto resolved_service_handler_parameters {resolved_service_handler_parameters_future.get()};
                            const auto resolved_service = resolved_service_handler_parameters.service;

                            THEN ("the service type contains the advertised service type")
                            {
                                REQUIRE (resolved_service.fullname.to_string().find (service_to_advertise.type.to_string()) != std::string::npos);
                            }

                            THEN ("the service domain contains the advertised service domain")
                            {
                                REQUIRE (resolved_service.fullname.to_string().find (service_to_advertise.domain.to_string()) != std::string::npos);
                            }

                            THEN ("the service host name is not empty")
                            {
                                REQUIRE (resolved_service.host.name.to_string().empty() == false);
                            }

                            THEN ("the service port is a placeholder")
                            {
                                REQUIRE (resolved_service.host.port == service_port::placeholder);
                            }
                        }
                    }
                }
            }
        }
    }
}
