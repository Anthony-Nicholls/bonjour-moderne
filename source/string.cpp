
#include "string.h"

namespace bonjour_moderne
{
    string::string (const std::string& str) noexcept
        : str {str}
    {
    }

    bool string::ends_with (const std::string& ending) const noexcept
    {
        if (ending.length() > str.length())
            return false;

        return std::equal (ending.rbegin(), ending.rend(), str.rbegin());
    }

    bool string::begins_with (const std::string& begining) const noexcept
    {
        if (begining.length() > str.length())
            return false;

        return std::equal (begining.begin(), begining.end(), str.begin());
    }

    string string::with_ending (const std::string& ending) const noexcept
    {
        if (ends_with (ending))
            return string {str};

        return string {str + ending};
    }

    string string::with_begining (const std::string& begining) const noexcept
    {
        if (begins_with (begining))
            return string {str};

        return string {begining + str};
    }

    string string::with_allowed_characters (const std::string& allowed_characters) const noexcept
    {
        std::string result {};

        for (const auto& character : str)
        {
            if (allowed_characters.find (character) != std::string::npos)
                result += character;
        }

        return string {result};
    }

    string string::with_maximum_length (const int maximum_length) const noexcept
    {
        if (str.length() <= static_cast<size_t> (maximum_length))
            return string {str};

        return string {str.substr (0, maximum_length)};
    }

    string string::split_before (const std::string& seperator) const noexcept
    {
        if (const auto pos {str.find (seperator)}; pos != std::string::npos)
            return string {str.substr (0, pos)};

        return string {str};
    }

    string string::split_after (const std::string& seperator) const noexcept
    {
        if (const auto pos {str.find (seperator)}; pos != std::string::npos)
            return string {str.substr (pos + seperator.length(), str.length())};

        return string {""};
    }

    string::operator std::string() const noexcept
    {
        return str;
    }
} // namespace bonjour_moderne
