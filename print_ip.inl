
bool IsLittleEndian()
{
    const uint16_t v = 0xFEFF;
    auto v1 = reinterpret_cast<const uint8_t*>( &v );
    return v1[1] == 0xFE;
}

template< class ChStream, class TrStream, class  T>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v, other_type )
{
    static_assert( std::is_integral<T>::value, "print ip by type: tuple, containter, integer type and std::string" );
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
        i += step;
    }
}

template< class ChStream, class TrStream, class ChStr, class TrStr, class AllocStr >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::basic_string<ChStr, TrStr, AllocStr>& str )
{
    os << str;
}

template< class ChStream, class TrStream, class TCont>
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const TCont& v, cont_type )
{
    auto iter = v.begin();
    auto iter_end = v.end();
    if( iter == iter_end )
        return;

    while( true )
    {
        print_ip( os, *iter );
        ++iter;
        if( iter == iter_end )
            break;
        os << '.';
    }
}

template< class ChStream, class TrStream, class T >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const T& v )
{
    print_ip( os, v, typename std::conditional<has_const_iterators<T>::value, cont_type, other_type>::type() );
}

template< class ChStream, class TrStream, class Tuple, std::size_t... Is >
void print_tuple_impl( std::basic_ostream<ChStream, TrStream>& os, const Tuple& t, seq<Is...> )
{
    auto b = { ( ( os << ( Is == 0 ? "" : "." ) , print_ip( os, std::get<Is>( t ) ) ), 0 )... };
}

template< class ChStream, class TrStream, class... Args >
void print_ip( std::basic_ostream<ChStream, TrStream>& os, const std::tuple<Args...>& t )
{
    print_tuple_impl( os, t, gen_seq<sizeof...( Args )>{} );
}
