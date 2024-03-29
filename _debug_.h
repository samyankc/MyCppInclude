#ifndef CUSTOM_DEBUG_H
#define CUSTOM_DEBUG_H

#ifndef DEBUG
#define DEBUG
#endif

#include <cstddef>
#include <cstdio>
#include <atomic>
#include <mutex>
#include <thread>
#include <map>
#include <format>
#include <iostream>

namespace Debug
{

    struct OnColumn
    {
        int Offset;
        void Print( const auto&... Content )
        {
            auto MakeString = []( const auto& c ) {
                if constexpr( std::is_convertible_v<decltype( c ), std::string> )
                    return c;
                else
                    return std::to_string( c );
            };

            printf( "%*c%s\n", Offset, ' ', ( MakeString( Content ) + ... ).c_str() );
            fflush( stdout );
        }
    };
    // OnColumn MainColumn{ 0 }, SideColumn{ 30 };

    template<typename SizeType>
    SizeType CountDigit( SizeType n )
    {
        if( n < 0 ) return CountDigit( -n ) + 1;
        SizeType ceiling = 10;
        for( SizeType i = 0; ++i < 20; ceiling *= 10 )
            if( n < ceiling ) return i;
        return static_cast<SizeType>( 20 );
    }

    template<typename T>
    void DumpBinary( const T& src, int bytes = 1 )
    {
        bytes *= sizeof( T );

        constexpr int ColumnCount = 8;

        if( bytes > ColumnCount && bytes % ColumnCount > 0 )
            for( int padding = ColumnCount - bytes % ColumnCount; padding-- > 0; ) printf( "         " );

        auto Byte = reinterpret_cast<const unsigned char*>( &src );
        for( int i = bytes; i-- > 0; )
        {
            for( int bit = 8; bit-- > 0; ) putchar( Byte[i] >> bit & 1 ? '1' : 'o' );
            putchar( ' ' );
            if( i % ColumnCount == 0 ) printf( "[%u]\n", i );
        }
        putchar( '\n' );
    }

    template<typename Type>
    void TypeOf( Type&& )
    {
        return +[/* Read Error Message */] {};
    }

    template<typename T>
    constexpr auto TypeName = []<typename U> {
        auto TemplateTypeName = std::string_view{ __PRETTY_FUNCTION__ };
        TemplateTypeName.remove_suffix( 1 );
        TemplateTypeName.remove_prefix( TemplateTypeName.find( '=' ) + 2 );
        return TemplateTypeName;
    }.template operator()<T>();

    enum class Silent : unsigned { None = 0, Dtor = 1, Ctor = 2 };

    constexpr auto operator&( Silent LHS, Silent RHS ) -> bool
    {
        return static_cast<unsigned>( LHS ) & static_cast<unsigned>( RHS );
    }

    struct NoisyIndentation
    {
        inline static std::atomic<std::size_t> CurrentIndentationLayer = 0;

        static auto Width() { return 4 * CurrentIndentationLayer.load(); }

        NoisyIndentation() { ++CurrentIndentationLayer; }
        ~NoisyIndentation() { --CurrentIndentationLayer; }
    };

    template<typename T, Silent SilentFlag = Silent::None>
    struct [[maybe_unused]] NoisyMember
    {
        constexpr auto PrintMessage( std::string_view Message ) const
        {
            std::cout << std::format( "{:{}}[ {} @ {} ] | {}", "", NoisyIndentation::Width(),  //
                                      TypeName<T>, static_cast<const void*>( this ), Message )
                      << std::endl;
        }

        constexpr auto Ctor_PrintMessage( std::string_view Message ) const
        {
            if constexpr( ! ( SilentFlag & Silent::Ctor ) ) PrintMessage( Message );
        }

        constexpr auto Dtor_PrintMessage() const
        {
            if constexpr( ! ( SilentFlag & Silent::Dtor ) ) PrintMessage( "Dtor" );
        }

        constexpr NoisyMember() { Ctor_PrintMessage( "Default Ctor" ); }

        constexpr NoisyMember( const NoisyMember& ) { Ctor_PrintMessage( "Copy Ctor" ); }
        constexpr NoisyMember( NoisyMember&& ) { Ctor_PrintMessage( "Move Ctor" ); }

        constexpr ~NoisyMember() { Dtor_PrintMessage(); }
    };
}  // namespace Debug
#endif