
#include <bonjour_moderne.h>
#include <future>
#include <iostream>

void run()
{
    std::promise<void> advertised;

    // given an advertisable service and an advertised service handler
    const bonjour_moderne::advertisable_service advertisable_service {
        "Test Service",
        "_test._udp",
        "local",
        0,
        bonjour_moderne::service_host {"", 12345},
        bonjour_moderne::txt_record {{
            {"key1", "value1"},
            {"key 2", "value 2"}
        }}
    };

    const auto advertised_service_handler = [&advertised](const bonjour_moderne::advertised_service& service,
                                                          const bool was_added)
    {
        if (was_added)
        {
            std::cout << "registered\n";
            try { advertised.set_value(); }
            catch (const std::future_error&) {}
        }
    };

    // when the service is advertised
    const bonjour_moderne::service_advertiser advertiser {
        advertisable_service,
        advertised_service_handler,
        nullptr
    };

    // then the handler is notified
    advertised.get_future().wait();

    //==========================================================================
    std::promise<bonjour_moderne::discovered_service> discovered;

    const bonjour_moderne::discoverable_service discoverable_service {
        advertisable_service.type,
        advertisable_service.domain,
        advertisable_service.interface_index
    };

    const auto discovered_service_handler = [&discovered](const bonjour_moderne::discovered_service& service,
                                                          const bool was_added,
                                                          const bool more_coming)
    {
        if (was_added)
        {
            std::cout << "discovered\n";
            try { discovered.set_value (service); }
            catch (const std::future_error&) {}
        }
    };

    const bonjour_moderne::service_browser service_browser {
        discoverable_service,
        discovered_service_handler,
        nullptr
    };

    //==========================================================================
    std::promise<void> resolved;

    const auto resolved_service_handler = [&resolved](const bonjour_moderne::resolved_service& sercice,
                                                      const bool more_coming)
    {
        std::cout << "resolved\n";
        assert (sercice.txt_record.num_values() == 2);
        assert (sercice.txt_record.has_value ("key1"));
        assert (sercice.txt_record.has_value ("key 2"));
        assert (sercice.txt_record.get_value ("key1") == "value1");
        assert (sercice.txt_record.get_value ("key 2") == "value 2");
        try { resolved.set_value (); }
        catch (const std::future_error&) {}
    };

    auto discovered_service {discovered.get_future().get()};
    discovered_service.resolve (resolved_service_handler, nullptr);

    resolved.get_future().wait();
}

int main()
{
    while (true)
        run();
    
    return 0;
}
