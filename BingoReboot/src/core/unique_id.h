#pragma once

#include "common.h"
#include <atomic>

template <Integral T = u64>
class UniqueId
{
public:
    using ValueType = T;
    class Generator;

    friend class Generator;

private:
    static inline constexpr ValueType InvalidValue = static_cast<ValueType>(0);

    ValueType _value = InvalidValue;

public:
    constexpr UniqueId() = default;
    constexpr UniqueId(const UniqueId&) = default;
    constexpr UniqueId(UniqueId&&) noexcept = default;
    constexpr ~UniqueId() = default;

    constexpr UniqueId& operator=(const UniqueId&) = default;
    constexpr UniqueId& operator=(UniqueId&&) noexcept = default;

    constexpr bool operator==(const UniqueId& other) const = default;
    constexpr auto operator<=>(const UniqueId& other) const = default;

public:
    constexpr ValueType value() const noexcept { return _value; }

    constexpr HashCode hashCode() const noexcept { return static_cast<HashCode>(_value); }

public:
    constexpr explicit operator bool() const noexcept { return _value != InvalidValue; }
    constexpr bool operator!() const noexcept { return _value == InvalidValue; }

private:
    explicit constexpr UniqueId(ValueType value) : _value(value) {}

public:
    static consteval UniqueId invalid() { return UniqueId(); }

    class Generator
    {
    private:
        std::atomic<ValueType> _nextValue = static_cast<ValueType>(1);

    public:
        constexpr Generator() = default;
        constexpr Generator(const Generator&) = delete;
        constexpr Generator(Generator&&) noexcept = delete;
        constexpr ~Generator() = default;

        constexpr Generator& operator=(const Generator&) = delete;
        constexpr Generator& operator=(Generator&&) noexcept = delete;

        inline UniqueId operator()() noexcept
        {
            ValueType value = _nextValue.fetch_add(static_cast<ValueType>(1), std::memory_order_relaxed);
            return UniqueId(value);
        }
    };
};

template <Integral T>
struct std::hash<UniqueId<T>>
{
    std::size_t operator()(const UniqueId<T>& id) const noexcept
    {
        return std::hash<T>()(static_cast<T>(id.hashCode()));
    }
};
