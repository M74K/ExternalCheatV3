#pragma once

#include <cstdint>
#include <string_view>

class Memory
{
public:
    Memory(const std::string_view processName) noexcept
    {
        (void)processName;
    }

    ~Memory() = default;

    std::uintptr_t GetModuleAddress(
        const std::string_view moduleName
    ) const noexcept
    {
        (void)moduleName;
        return 0;
    }

    template <typename T>
    T read(const std::uintptr_t& address) const noexcept
    {
        (void)address;
        return T{};
    }

    template <typename T>
    void write(
        const std::uintptr_t& address,
        const T& value
    ) const noexcept
    {
        (void)address;
        (void)value;
    }
};
