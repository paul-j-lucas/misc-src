#ifndef ENUM_BITS_H
#define ENUM_BITS_H

/**
 * @file
 * Provides utility functions for enumerations.
 */

#include <type_traits>

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator|( EnumType lhs, EnumType rhs ) {
    using U = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>( static_cast<U>(lhs) | static_cast<U>(rhs) );
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator&( EnumType lhs, EnumType rhs ) {
    using U = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>( static_cast<U>(lhs) & static_cast<U>(rhs) );
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator^( EnumType lhs, EnumType rhs ) {
    using U = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>( static_cast<U>(lhs) ^ static_cast<U>(rhs) );
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator~( EnumType e ) {
    using U = std::underlying_type_t<EnumType>;
    return static_cast<EnumType>( ~static_cast<U>( e ) );
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator|=( EnumType &lhs, EnumType rhs ) {
    lhs = lhs | rhs;
    return lhs;
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator&=( EnumType &lhs, EnumType rhs ) {
    lhs = lhs & rhs;
    return lhs;
}

template<typename EnumType>
    requires std::is_enum_v<EnumType>
constexpr EnumType operator^=( EnumType &lhs, EnumType rhs ) {
    lhs = lhs ^ rhs;
    return lhs;
}

#endif /* ENUM_BITS_H */
/* vim:set et sw=4 ts=4: */
