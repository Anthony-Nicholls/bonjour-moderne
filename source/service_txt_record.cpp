
#include <bonjour_moderne/service_txt_record.h>

#include <dns_sd.h>

class bonjour_moderne::service_txt_record::Implementation
{
public:
    Implementation()
    {
        TXTRecordCreate (&txt_record_ref, 0, nullptr);
    }

    Implementation (const Implementation& other)
        : Implementation (other.data(), other.size())
    {
    }

    Implementation (const void* data, const uint16_t size)
        : Implementation()
    {
        for (uint16_t index {0}; index < TXTRecordGetCount (size, data); ++index)
        {
            constexpr uint16_t key_buffer_max_size {256};
            char key_buffer[key_buffer_max_size] {};

            uint8_t value_size {0};
            const void* value_ptr {nullptr};

            TXTRecordGetItemAtIndex (size,
                                     data,
                                     index,
                                     key_buffer_max_size,
                                     key_buffer,
                                     &value_size,
                                     &value_ptr);

            TXTRecordSetValue (&txt_record_ref,
                               key_buffer,
                               value_size,
                               value_ptr);
        }
    }

    ~Implementation()
    {
        TXTRecordDeallocate (&txt_record_ref);
    }

    bool has_value (const std::string& key) const
    {
        return TXTRecordContainsKey (size(), data(), key.c_str()) != 0;
    }

    std::string get_value (const std::string& key) const
    {
        uint8_t value_size {0};
        const void* value_ptr {TXTRecordGetValuePtr (size(), data(), key.c_str(), &value_size)};
        return {static_cast<const char*> (value_ptr), static_cast<size_t> (value_size)};
    }

    size_t num_values() const
    {
        return static_cast<size_t> (TXTRecordGetCount (size(), data()));
    }

    void set_value (const std::string& key,
                    const std::string& value)
    {
        TXTRecordSetValue (&txt_record_ref,
                           key.c_str(),
                           static_cast<uint8_t> (value.size()),
                           value.c_str());
    }

    void remove_value (const std::string& key)
    {
        TXTRecordRemoveValue (&txt_record_ref, key.c_str());
    }

    const void* data() const
    {
        return TXTRecordGetBytesPtr (&txt_record_ref);
    }

    uint16_t size() const
    {
        return TXTRecordGetLength (&txt_record_ref);
    }

private:
    TXTRecordRef txt_record_ref {};
};

namespace bonjour_moderne
{
    service_txt_record::service_txt_record() noexcept
        : implementation {std::make_unique<Implementation>()}
    {
    }

    service_txt_record::service_txt_record (const service_txt_record& other) noexcept
        : implementation {std::make_unique<Implementation> (*other.implementation)}
    {
    }

    service_txt_record::service_txt_record (const std::unordered_map<std::string, std::string>& values) noexcept
        : service_txt_record()
    {
        for (const auto& value : values)
            set_value (value.first, value.second);
    }

    service_txt_record::service_txt_record (const void* data, uint16_t size) noexcept
        : implementation {std::make_unique<Implementation> (data, size)}
    {
    }

    service_txt_record::~service_txt_record() noexcept = default;

    const void* service_txt_record::data() const noexcept
    {
        return implementation->data();
    }

    uint16_t service_txt_record::size() const noexcept
    {
        return implementation->size();
    }

    bool service_txt_record::has_value (const std::string& key) const noexcept
    {
        return implementation->has_value (key);
    }

    std::string service_txt_record::get_value (const std::string& key) const noexcept
    {
        return implementation->get_value (key);
    }

    size_t service_txt_record::num_values() const noexcept
    {
        return implementation->num_values();
    }

    void service_txt_record::set_value (const std::string& key,
                                        const std::string& value) noexcept
    {
        implementation->set_value (key, value);
    }

    void service_txt_record::remove_value (const std::string& key) noexcept
    {
        implementation->remove_value (key);
    }
} // namespace bonjour_moderne
