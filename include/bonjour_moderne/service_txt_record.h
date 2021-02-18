
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace bonjour_moderne
{
    class service_txt_record
    {
    public:
        service_txt_record() noexcept;
        service_txt_record (const service_txt_record& other) noexcept;
        service_txt_record (const std::unordered_map<std::string, std::string>& values) noexcept;
        service_txt_record (const void* data, uint16_t size) noexcept;
        ~service_txt_record() noexcept;

        const void* data() const noexcept;
        uint16_t size() const noexcept;

        bool has_value (const std::string& key) const noexcept;
        std::string get_value (const std::string& key) const noexcept;
        size_t num_values() const noexcept;

        void set_value (const std::string& key,
                        const std::string& value) noexcept;

        void remove_value (const std::string& key) noexcept;

    private:
        class Implementation;
        std::unique_ptr<Implementation> implementation;
    };
} // namespace bonjour_moderne
