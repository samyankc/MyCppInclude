#ifdef UNIT_TEST
#define main(...) UnitTest_Skip_This_Main(__VA_ARGS__)
#define ParenOpen (
#define ParenClose )
#define UnitTest() main ParenOpen ParenClose
#endif

// #include "EasyMeta.h"
#include <tuple>
#include <string>
// #include <typeinfo>

#include "boost-ext/ut.hpp"
using namespace boost::ut;
using namespace boost::ut::literals;

template<typename T>
constexpr auto TypeName = []<typename U> {
    auto TemplateTypeName = std::string_view{ __PRETTY_FUNCTION__ };
    TemplateTypeName.remove_suffix( 1 );
    TemplateTypeName.remove_prefix( TemplateTypeName.find( '=' ) + 2 );
    return TemplateTypeName;
}.template operator()<T>();

template<typename CharT = char>
auto operator+( const CharT* LHS, std::basic_string_view<CharT> RHS )
{
    return LHS + std::basic_string<CharT>{ RHS };
}

// for easier building of test samples of the same type
// using std::array or std::initializer_list instead?
template<typename T>
constexpr auto MakeHomogeneousTuple( const auto&... Args )
{
    return std::make_tuple( static_cast<T>( Args )... );
}

template<typename T, auto... Args>
constexpr auto IntegralConstantTuple = std::tuple<std::integral_constant<T, static_cast<T>( Args )>...>{};

