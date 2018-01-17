#include <tuple>
#include <type_traits>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <list>

bool IsLittleEndian()
{
    const uint16_t v = 0xFEFF;
    auto v1 = reinterpret_cast<const uint8_t*>( &v );
    return v1[1] == 0xFE;
}

const bool LittleEndian = IsLittleEndian();

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

template< class ChStream, class TrStream, class Tuple, std::size_t... Is >
void print_tuple_impl( std::basic_ostream<ChStream, TrStream>& os, const Tuple& t, std::index_sequence<Is...> )
{
    auto b = { ( ( os << ( Is == 0 ? "" : "." ) << std::get<Is>( t ) ), 0 )... };
}

template< class ChStream, class TrStream, class... Args >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::tuple<Args...>& t )
{
    print_tuple_impl( os, t, std::index_sequence_for<Args...>{} );
}

struct cont_type
{};

struct other_type
{};

template< class ChStream, class TrStream, class TCont>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const TCont& v,  cont_type )
{
    auto iter = v.begin();
    auto iter_end = v.end();
    if( iter == iter_end )
        return;

    while( true )
    {
        os << ( *iter );
        ++iter;
        if( iter == iter_end )
            break;
        os << '.';
    }
}

template< class ChStream, class TrStream, class  T>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v, other_type )
{
    static_assert( std::is_integral<T>::value, "print ip by type: tuple, containter and integer type" );
    auto v1 = reinterpret_cast<const uint8_t*>( &v );

    size_t i = 0;
    size_t step = 1;
    size_t barrier = sizeof( T ) - 1;
    if( LittleEndian )
    {
        i = sizeof( T ) - 1;
        barrier = 0;
        step = -1;
    }

    while( true )
    {
        os << +v1[i];
        if( i == barrier )
            break;

        os << '.';
        i+= step;
    }
}

template< class ChStream, class TrStream, class ChStr, class TrStr, class AllocStr >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::basic_string<ChStr, TrStr, AllocStr>& str )
{
    os << str;
}

template< class ChStream, class TrStream, class T >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v )
{
    print_ip( os, v, std::conditional<has_const_iterators<T>::value, cont_type, other_type>::type{} );
}

#define PRINT( x )  std::cout << #x##"\n"; \
                    print_ip( std::cout, x ); \
                    std::cout << '\n';
int main()
{
    PRINT( char( -1 ) );
    PRINT( short( 0 ) );
    PRINT( int( 2130706433 ) );
    PRINT( long( 8875824491850138409 ) );
    PRINT( std::string( "192.168.0.1" ) );
    PRINT( std::vector<int>({ 12,15,6,7 }) );
    PRINT( std::list<int>( { 12,15,6,7 } ) );
    PRINT( std::make_tuple( 13, 16, 7, 8 ) );
    return 0;
}