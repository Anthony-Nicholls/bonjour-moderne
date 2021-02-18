
#pragma once

#include <bonjour_moderne/service_type.h>
#include <bonjour_moderne/service_protocol.h>

namespace bonjour_moderne
{
    class string
    {
    public:
        explicit string (const std::string& str) noexcept;

        bool ends_with (const std::string& ending) const noexcept;

        bool begins_with (const std::string& begining) const noexcept;

        string with_ending (const std::string& ending) const noexcept;

        string with_begining (const std::string& begining) const noexcept;

        string with_allowed_characters (const std::string& allowed_characters) const noexcept;

        string with_maximum_length (const int maximum_length) const noexcept;

        string split_before (const std::string& seperator) const noexcept;

        string split_after (const std::string& seperator) const noexcept;

        operator std::string() const noexcept;

    private:
        const std::string str;
    };
} // namespace bonjour_moderne
