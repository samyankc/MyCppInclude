#include "../EasyTest.h"
#include "../EasyString.h"
#include <format>

using namespace boost::ut;
using namespace boost::ut::literals;

using namespace EasyString;

constexpr auto StrViewStrongEquality = []( StrView LHS, StrView RHS ) {
    return LHS == RHS && LHS.begin() == RHS.begin() && LHS.end() == RHS.end();
};

constexpr auto PrepareSearchData = []( StrView Text, StrView Input ) {
    return std::tuple( Text, Input, Search( Text ).In( Input ), Input | Search( Text ) );
};
auto TestSearch()
{
    "Search bcd In abcde [common case]"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareSearchData( "bcd", "abcde" );
        expect( StrViewStrongEquality( Result, PipeResult ) );

        expect( Result == Text );
        expect( Result.begin() == Input.begin() + 1 );
    };

    "Search empty string in abcde [empty text case]"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareSearchData( "", "abcde" );
        expect( StrViewStrongEquality( Result, PipeResult ) );

        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };

    "Search bcd in empty string [empty input case]"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareSearchData( "bcd", "" );
        expect( StrViewStrongEquality( Result, PipeResult ) );

        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };

    "Search empty string in empty string [empty input & text case]"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareSearchData( "", "" );
        expect( StrViewStrongEquality( Result, PipeResult ) );

        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };

    "Search fgh in abcde [not found case]"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareSearchData( "fgh", "abcde" );
        expect( StrViewStrongEquality( Result, PipeResult ) );

        expect( Result.empty() );
        expect( Result.begin() == Input.end() );
        expect( Result.end() == Input.end() );
    };
}

constexpr auto PrepareBeforeData = []( StrView Text, StrView Input ) {
    return std::tuple( Text, Input, Before( Text )( Input ), Input | Before( Text ) );
};
auto TestBefore()
{
    "abcdef | Before( cde ) is ab"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareBeforeData( "cde", "abcdef" );
        expect( StrViewStrongEquality( Result, PipeResult ) );
        expect( Result == "ab" );
        expect( Result.begin() == Input.begin() );
    };

    "abcdef | Before( ab ) is empty"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareBeforeData( "ab", "abcdef" );
        expect( StrViewStrongEquality( Result, PipeResult ) );
        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };

    "abcdef | Before( gh ) is empty"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareBeforeData( "gh", "abcdef" );
        expect( StrViewStrongEquality( Result, PipeResult ) );
        expect( Result.empty() );
        expect( Result.begin() == Input.end() );
    };

    "abcdef | Before( empty ) is empty at abcdef begin"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareBeforeData( "", "abcdef" );
        expect( StrViewStrongEquality( Result, PipeResult ) );
        expect( Text.empty() );
        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };

    "empty | Before( abc ) is empty"_test = [] {
        auto [Text, Input, Result, PipeResult] = PrepareBeforeData( "abc", "" );
        expect( StrViewStrongEquality( Result, PipeResult ) );
        expect( Input.empty() );
        expect( Result.empty() );
        expect( Result.begin() == Input.begin() );
    };
}

auto AcceptConstCharPtr( const char* Ptr ) { return Ptr; }

auto TestStrView_c_str()
{
    "TestStrView_c_str"_test = [] {};
    auto abc = std::string_view( "abc" );
    auto kkk = std::array{ 'a', 'b', 'c', 'd' };

    std::cout << std::format( "{} is {}\n", "std::c_str( abc )", AcceptConstCharPtr( std::c_str( abc ) ) );

    auto ab = std::string_view{ abc.begin(), abc.begin() + 2 };
    std::cout << std::format( "{} is {}\n", "std::c_str( ab )", AcceptConstCharPtr( std::c_str( ab ) ) );
    std::cout << std::format( "{} is {}\n", "std::data( ab )", AcceptConstCharPtr( std::data( ab ) ) );

}

int main()
{
    // TestSearch();
    // TestBefore();
    TestStrView_c_str();
    return 0;
}