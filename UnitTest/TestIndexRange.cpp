// #include "../index_range_deducing_this.h"
#include "../index_range.h"
#include "../EasyTest.h"
#include <print>

using namespace IndexRange;

int main()
{
    for( auto i : Range( 10 ) ) std::println( "{},", i );

    for( auto i : Range( 1, 10 )|Reverse ) std::println( "{},", i );

    for( auto i : Range( 3, 20 )   //
                      | Drop( 2 )  //
                      | Reverse    //
                      | Drop( 3 )  //
                      | Reverse    //
                      | Take( 5 ) //
                      | Reverse)
        std::println( "{},", i );

    return 0;
}

auto UnitTest() -> void
{
    "Dummy"_test = [] {  //
        expect( 1_i == 1 );
        expect( 1_i == 2 );
    };
}
