#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <concepts>
#include <compare>
#include <ranges>
#include <allegro5/allegro.h>


using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using usize = std::size_t;
using isize = std::ptrdiff_t;

using HashCode = std::size_t;


/* Concepts */
template <typename T>
concept Integral = std::integral<T>;

template <typename T>
concept SignedIntegral = Integral<T> && std::signed_integral<T>;

template <typename T>
concept UnsignedIntegral = Integral<T> && std::unsigned_integral<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <typename T>
concept Arithmetic = Integral<T> || FloatingPoint<T>;


template <typename T1, typename T2>
concept SameAs = std::same_as<T1, T2>;

template <typename DerivedT, typename BaseT>
concept DerivedFrom = std::derived_from<DerivedT, BaseT>;