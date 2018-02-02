/*!
    \file
    \brief Основной файл программы
*/
#include <array>
#include <iostream>
#include <vector>
#include <list>

#include "print_ip.h"

/// Макрос для вывода тестируемого образца и сам вывод
#define PRINT( x )  std::cout << #x"\n"; \
                    print_ip( std::cout, x );\
                    std::cout << std::endl << std::endl;

int main()
{
    PRINT( char( -1 ) );
    PRINT( short( 0 ) );
    PRINT( int( 2130706433 ) );
    PRINT( long( 8875824491850138409 ) );
    PRINT( std::string( "192.168.0.1" ) );
    PRINT( std::vector<int>( { 2130706433,15,6,7 } ) );
    PRINT( std::list<char>( { 12,15,6,7 } ) );
    PRINT( ( std::make_tuple( char(13), char(-1), short( 0 ), int(2130706433) ) ) );
    PRINT( ( std::make_tuple( std::vector<char>{ 127, 0, 0, 1 }, std::string( "777" ) ) ) );

    return 0;
}