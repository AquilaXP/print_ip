/*!
    \file
    \brief Функции печати ip

    Данный файл содержит реализации функций print_ip и функцию main
*/
#pragma once

#include <tuple>
#include <type_traits>
#include <string>
#include <iosfwd>

/**
    \brief Определяет порядок байт в системе
*/
bool IsLittleEndian();

/// True если порядок байт little-endian
const bool LittleEndian = IsLittleEndian();

template<std::size_t...> struct seq{};

template<std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>{};

template<std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...>{};

/// Шаблон, определяющий, есть ли у типа итератор
template< typename T >
struct has_iterators
{
private:
    template<
        typename U,
        typename U::iterator( U::* )( ) = &U::begin,
        typename U::iterator( U::* )( ) = &U::end
    >
        static constexpr std::true_type check( const U& );
    static constexpr std::false_type check( ... );

public:
    static constexpr bool value = decltype( check( std::declval< T >() ) )::value;
};

/// Шаблон, определяющий, есть ли у типа константный итератор
template< typename T >
struct has_const_iterators
{
private:
    template<
        typename U,
        typename U::const_iterator( U::* )( ) const = &U::begin,
        typename U::const_iterator( U::* )( ) const = &U::end
    >
    static constexpr std::true_type check( const U& );
    static constexpr std::false_type check( ... );

public:
    static constexpr bool value = decltype( check( std::declval< T >() ) )::value;
};

/// Вспомогательный тип
struct cont_type
{};

/// Вспомогательный тип
struct other_type
{};

/// Функция печати числовых типов побайтов как ip
template< class ChStream, class TrStream, class  T>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v, other_type );

/// Функция печати текста как... текста
template< class ChStream, class TrStream, class ChStr, class TrStr, class AllocStr >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::basic_string<ChStr, TrStr, AllocStr>& str );

/// Функция печати элементов коллекций как ip
template< class ChStream, class TrStream, class TCont>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const TCont& v, cont_type );

/// Функция печати ip
template< class ChStream, class TrStream, class T >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v );

/// Функция печати tuple как ip
template< class ChStream, class TrStream, class... Args >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::tuple<Args...>& t );

#include "print_ip.inl"