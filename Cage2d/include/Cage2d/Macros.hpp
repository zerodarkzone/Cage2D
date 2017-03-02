#pragma once

#ifdef NDEBUG
//#define USE_CNTD
#define USE_SHARED
#else
#define USE_SHARED
#endif //NDEBUG

#ifdef USE_CNTD
#include "cntd_ptr.hpp"
template <typename T>
using SmrPtr = Smr_ptr::cntd_ptr< T >;
#else
#ifdef USE_SHARED
template <typename T>
using SmrPtr = std::shared_ptr< T >;
#endif // USE_SHARED
#endif // USE_CNTD

#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::cout << (str) << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


/* #define CREATE_FUNC(_type)                       \
		static SmrPtr<_type> Create()                  \
		{                                              \
		_type *_new_type = new (std::nothrow) _type(); \
		if(_new_type)                                  \
				if(_new_type->Init())                      \
				return SmrPtr<_type>(_new_type);           \
		return nullptr;                                \
		}
 */

#define CREATE_FUNC(_type)                                      \
		template<typename ...Args>                              \
		static SmrPtr<_type> Create(Args ...args)               \
		{                                                       \
		_type *_new_type = new (std::nothrow) _type(args...);   \
		if(_new_type)                                           \
				if(_new_type->Init())                           \
				return SmrPtr<_type>(_new_type);                \
		return SmrPtr<_type>(nullptr);                          \
		}


#define INSTANCE(_type)       \
		static _type &GetInstance()     \
		{        \
				static _type* _instance = new _type();           \
				return *_instance;     \
		}

#include "Game.hpp"
#define GAME Cage2d::Game::GetInstance()
#define GameApp Cage2d::Game::GetInstance()

#include "BitmapFileCache.hpp"
#define BFC BitmapFileCache::GetInstance()

#ifndef PI
#define PI ALLEGRO_PI
#endif //PI

#ifndef PI2
#define PI2 6.28318530718
#endif // PI2


#ifdef __MINGW32__
#define sprintf_s sprintf
#endif // __MINGW32__

#include <malloc.h>

#ifdef __MINGW32__
#define malloca _malloca
#define freea _freea
#else
#ifdef _MSC_BUILD
#define malloca _malloca
#define freea _freea
#endif // _MFC_VER
#endif // __MINGW32__

#ifdef UNREFERENCED_PARAMETER
#undef UNREFERENCED_PARAMETER
#endif // UNREFERENCED_PARAMETER

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) {(void)(P);}
#endif // UNREFERENCED_PARAMETER

#ifdef UNREFERENCED_LOCAL_VARIABLE
#undef UNREFERENCED_LOCAL_VARIABLE
#endif // UNREFERENCED_LOCAL_VARIABLE

#ifndef UNREFERENCED_LOCAL_VARIABLE
#define UNREFERENCED_LOCAL_VARIABLE(L) {(void)(L);}
#endif // UNREFERENCED_LOCAL_VARIABLE

inline uint16_t Bit( uint16_t pos )
{
	uint16_t i = static_cast<uint16_t> (1);
	return uint16_t( i << pos );
}

template<typename T, size_t _size> inline size_t arraySize( const T( &array )[_size] )
{
	UNREFERENCED_PARAMETER( array )
	return _size;
}

template<typename T> inline T matrixToArrayIndex( T i, T j, T width )
{
	return ( i * width + j);
}

inline void get_desktop_resolution( int adapter, int *w, int *h )
{
	ALLEGRO_MONITOR_INFO info;
	al_get_monitor_info( adapter, &info );

	*w = info.x2 - info.x1;
	*h = info.y2 - info.y1;
}

inline void fade_out( int speed, const alx::Color& color = { "black" } )
{
	int op, src, dst;
	al_get_blender( &op, &src, &dst );
	alx::Bitmap bmp_orig( GAME.SCREEN_WIDTH, GAME.SCREEN_HEIGHT );
	bmp_orig.setTarget( );
	GAME.GetMainWindow( ).getBackbuffer( ).draw( 0, 0 );
	alx::Display::setTarget( GAME.GetMainWindow( ) );
	al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );
	if (bmp_orig)
	{
		DEBUG_MSG( "Fading" );
		int a;
		if (speed <= 0)
		{
			speed = 8;
		}

		for (a = 255 - speed; a > 0; a -= speed)
		{
			al_clear_to_color( color );
			bmp_orig.drawTinted( al_map_rgba( 255, 255, 255, (unsigned char) (a) ), 0, 0 );
			al_flip_display( );
		}
	}
	al_set_blender( op, src, dst );
}
#include <cctype>
/// Returns a new string converted to uppercase

inline std::string to_upper( std::string str )
{
	for (auto& c : str)
		c = std::toupper( c );
	return str;
}

/// Returns a new string converted to lowercase

inline std::string to_lower( std::string str )
{
	for (auto& c : str)
		c = std::tolower( c );
	return str;
}

#ifdef __MINGW32__
namespace std
{

	template<typename T> inline string to_string( const T& n )
	{
		ostringstream stm;
		stm << n;
		return stm.str( );
	}
}
#endif //__MINGW32__

#define MOUSE_LEFT_BUTTON 1
#define MOUSE_MIDDLE_BUTTON 2
#define MOUSE_RIGHT_BUTTON 3

#include <iomanip>
#include <type_traits>

#include <cstdint>

namespace util
{
	typedef std::uint64_t hash_t;

	constexpr hash_t prime = 0x100000001B3ull;
	constexpr hash_t basis = 0xCBF29CE484222325ull;

	constexpr hash_t hash_compile_time( char const *str, hash_t last_value = basis )
	{
		return hash_t(*str) ? hash_compile_time( 1ull + str, hash_t( hash_t(*str) ^ last_value) * prime ) : last_value;
	}

	inline hash_t hash( const std::string& _string )
	{
		auto str = _string.c_str( );
		hash_t ret{ basis };

		while (*str)
		{
			ret ^= *str;
			ret *= prime;
			str++;
		}

		return ret;
	}
}

constexpr util::hash_t operator "" _hash( char const* p, size_t )
{
	return util::hash_compile_time( p );
}
