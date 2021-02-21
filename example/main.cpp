
#include <bonjour_moderne/service_advertiser.h>
#include <bonjour_moderne/service_browser.h>
#include <bonjour_moderne/service_resolver.h>

#include <array>
#include <chrono>
#include <csignal>
#include <ctime>
#include <exception>
#include <future>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


std::string timestamp()
{
    using namespace std::chrono;
    using clock = system_clock;

    const auto current_time_point {clock::now()};
    const auto current_time {clock::to_time_t (current_time_point)};
    const auto current_localtime {*std::localtime (&current_time)};
    const auto current_time_since_epoch {current_time_point.time_since_epoch()};
    const auto current_milliseconds {duration_cast<milliseconds>(current_time_since_epoch).count() % 1000};

    std::ostringstream stream;
    stream << std::put_time (&current_localtime, "%T");
    stream << '.' << std::setw (3) << std::setfill ('0') << current_milliseconds;
    return stream.str();
}

void print_usage()
{
    std::cout << "--advertise <Name> <Type> <Protocol> <Port> [<Domain>]\n";
    std::cout << "--browse <Type> <Protocol> [<Domain>]\n";
    std::cout << "--resolve <Name> <Type> <Protocol> <Domain>\n";
}

void throw_if_arguments_out_of_range (const std::vector<std::string>& arguments,
                                      const size_t minimum_size,
                                      const size_t maximum_size)
{
    if (arguments.size() < minimum_size)
        throw std::invalid_argument {"Not enough arguments"};

    if (arguments.size() > maximum_size)
        throw std::invalid_argument {"Too many arguments"};
}

void wait_for_interrupt()
{
    static const auto signal = []() -> auto&
    {
        static std::promise<void> instance;
        return instance;
    };

    const auto handler = [](int) { signal().set_value(); };
    std::signal (SIGINT, handler);
    std::signal (SIGTERM, handler);
    
    signal().get_future().wait();
}

struct table_column
{
    table_column (const std::string& title,
                  int minimum_width = 0)
        : title {title}
        , width {std::max (minimum_width, static_cast<int>(title.length()))}
    {
    }

    std::string title {};
    int width {};
};

template <int num_columns>
class table
{
public:
    using row = std::array<std::string, num_columns>;
    using columns = std::array<table_column, num_columns>;

    table (const columns& columns)
    {
        for (const auto& column : columns)
        {
            column_titles.push_back (column.title);
            column_widths.push_back (column.width);
        }

        print_row (column_titles, ' ');
        print_line();
    }

    void print_row (const row& row) const
    {
        print_row ({row.cbegin(), row.cend()}, ' ');
    }

private:
    void print_line()
    {
        print_row ({}, '-');
    }

    void print_row (const std::vector<std::string>& strings, const char pad) const
    {
        for (auto index {0}; index < num_columns; ++index)
        {
            std::cout << seperator << pad;
            std::cout << std::left << std::setw (column_widths[index]) << std::setfill (pad);
            std::cout << (static_cast<int>(strings.size()) > index ? strings[index] : "");
            std::cout << pad;
        }

        std::cout << seperator << "\n";
    }

    std::vector<std::string> column_titles;
    std::vector<int> column_widths;
    static constexpr auto seperator {'|'};
};

void run_advertiser (const std::vector<std::string>& arguments)
{
    throw_if_arguments_out_of_range (arguments, 4, 5);

    std::cout << timestamp() << " : Starting advertiser...\n\n";

    const table<6> table {{
        table_column {"Timestamp", 12},
        table_column {"Was added", 5},
        table_column {"Service name", 32},
        table_column {"Type", 17},
        table_column {"Protocol", 5},
        table_column {"Domain", 17}
    }};

    const auto handler = [&table](auto service,
                                  auto was_added)
    {
        table.print_row ({
            timestamp(),
            (was_added ? "True" : "False"),
            service.name.to_string(),
            service.type.to_string(),
            service.protocol.to_string(),
            service.domain.to_string()
        });
    };

    using namespace bonjour_moderne;
    const service_name name {arguments[0]};
    const service_type type {arguments[1]};
    const service_protocol protocol {arguments[2]};
    const service_port port {static_cast<uint16_t>(std::stoi (arguments[3]))};
    const auto domain {(arguments.size() > 4 ?
                        service_domain {arguments[4]} :
                        service_domain::any)};

    const service_advertiser advertiser ({name, type, protocol, port, domain}, handler);
    wait_for_interrupt();
}

void run_browser (const std::vector<std::string>& arguments)
{
    throw_if_arguments_out_of_range (arguments, 2, 3);

    std::cout << timestamp() << " : Starting browser...\n\n";

    const table<8> table {{
        table_column {"Timestamp", 12},
        table_column {"Was added", 5},
        table_column {"More coming", 5},
        table_column {"Service name", 32},
        table_column {"Type", 17},
        table_column {"Protocol", 5},
        table_column {"Domain", 17},
        table_column {"Interface", 2}
    }};

    const auto handler = [&table](auto service,
                                  auto was_added,
                                  auto more_coming)
    {
        table.print_row ({
            timestamp(),
            (was_added ? "True" : "False"),
            (more_coming ? "True" : "False"),
            service.name.to_string(),
            service.type.to_string(),
            service.protocol.to_string(),
            service.domain.to_string(),
            std::to_string (service.interface.to_unint32())
        });
    };

    using namespace bonjour_moderne;
    const service_type type {arguments[0]};
    const service_protocol protocol {arguments[1]};;
    const auto domain {(arguments.size() > 2 ?
                        service_domain {arguments[2]} :
                        service_domain::any)};

    const service_browser browser ({type, protocol, domain}, handler);
    wait_for_interrupt();
}

void run_resolver (const std::vector<std::string>& arguments)
{
    throw_if_arguments_out_of_range (arguments, 3, 4);

    std::cout << timestamp() << " : Starting resolver...\n\n";

    const table<6> table {{
        table_column {"Timestamp", 12},
        table_column {"More coming", 5},
        table_column {"Full service name", 100},
        table_column {"Host name", 32},
        table_column {"Port", 6},
        table_column {"Interface", 2}
    }};

    const auto handler = [&table](auto service,
                                  auto more_coming)
    {
        table.print_row ({
            timestamp(),
            (more_coming ? "True" : "False"),
            service.fullname.to_string(),
            service.host.name.to_string(),
            std::to_string (service.host.port.to_uint16()),
            std::to_string (service.interface.to_unint32())
        });
    };

    using namespace bonjour_moderne;
    const service_name name {arguments[0]};
    const service_type type {arguments[1]};
    const service_protocol protocol {arguments[2]};
    const auto domain {(arguments.size() > 3 ?
                     service_domain {arguments[3]} :
                     service_domain::any)};

    const service_resolver resolver ({name, type, protocol, domain}, handler);
    wait_for_interrupt();
}

void run (const std::string& command, const std::vector<std::string>& arguments)
{
    if (command.empty() || command == "--help")
        return print_usage();

    if (command == "--advertise")
        return run_advertiser (arguments);

    if (command == "--browse")
        return run_browser (arguments);

    if (command == "--resolve")
        return run_resolver (arguments);

    throw std::invalid_argument {"Unknown command: \"" + command + "\""};
}

int main (int argc, char* argv[])
{
    try
    {
        std::string command {};
        std::vector<std::string> arguments {};

        if (argc > 1)
            command = argv[1];

        if (argc > 2)
            arguments = {argv + 2, argv + argc};

        run (command, arguments);
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << "\n";
        return 1;
    }

    return 0;
}
