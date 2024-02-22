/*
**      sizes -- print C++ types' sizes and limits
**      sizes.cpp
**
**      Copyright (C) 2014-2020  Paul J. Lucas
**
**      This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2 of the Licence, or
**      (at your option) any later version.
** 
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
** 
**      You should have received a copy of the GNU General Public License
**      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// standard
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <unistd.h>

using namespace std;

int const name_width          = 18;
int const sizeof_width_bits   =  3;
int const sizeof_width_bytes  =  2;
int const limit_width         = 20;

template<typename T>
inline T number_limit( T t ) {
  return t;
}

// Specialization for char so the limits print as int rather than char.
inline int number_limit( char c ) {
  return static_cast<int>( c );
}

// Specialization for signed char so the limits print as int rather than char.
inline int number_limit( signed char c ) {
  return static_cast<int>( c );
}

// Specialization for unsigned char so the limits print as int rather than char.
inline int number_limit( unsigned char c ) {
  return static_cast<int>( c );
}

// ostream<char> doesn't know how to << wchar_t
inline int number_limit( wchar_t c ) {
  return static_cast<int>( c );
}

#if __cplusplus > 201703L
// ostream<char> doesn't know how to << char8_t
inline unsigned number_limit( char8_t c ) {
  return static_cast<unsigned>( c );
}
#endif

#if __cplusplus >= 201103L
// ostream<char> doesn't know how to << char16_t
inline unsigned number_limit( char16_t c ) {
  return static_cast<unsigned>( c );
}

// ostream<char> doesn't know how to << char32_t
inline unsigned long number_limit( char32_t c ) {
  return static_cast<unsigned long>( c );
}
#endif

template<typename T>
void print_type_info( char const *type_name, T ) {
  cout << left << setw( name_width ) << type_name << right
       << ' ' << setw( sizeof_width_bytes ) << sizeof( T )
       << ' ' << setw( sizeof_width_bits  ) << sizeof( T ) * 8
       << ' ' << setw( limit_width ) << number_limit( numeric_limits<T>::min() )
       << ' ' << setw( limit_width ) << number_limit( numeric_limits<T>::max() )
       << endl;
}

#define PRINT_TYPE_INFO(T) print_type_info( #T, static_cast<T>(0) )

int main() {
  PRINT_TYPE_INFO( bool );

  PRINT_TYPE_INFO( char );
  PRINT_TYPE_INFO( signed char );
  PRINT_TYPE_INFO( unsigned char );
  PRINT_TYPE_INFO( wchar_t );
#if __cplusplus > 201703L
  PRINT_TYPE_INFO( char8_t );
#endif
#if __cplusplus >= 201103L
  PRINT_TYPE_INFO( char16_t );
  PRINT_TYPE_INFO( char32_t );
#endif

  PRINT_TYPE_INFO( short );
  PRINT_TYPE_INFO( int );
  PRINT_TYPE_INFO( long );
  PRINT_TYPE_INFO( long long );

  PRINT_TYPE_INFO( unsigned short );
  PRINT_TYPE_INFO( unsigned int );
  PRINT_TYPE_INFO( unsigned long );
  PRINT_TYPE_INFO( unsigned long long );

  PRINT_TYPE_INFO( float );
  PRINT_TYPE_INFO( double );
  PRINT_TYPE_INFO( long double );

  PRINT_TYPE_INFO( void* );
  PRINT_TYPE_INFO( size_t );
  PRINT_TYPE_INFO( ssize_t );
  PRINT_TYPE_INFO( off_t );
  PRINT_TYPE_INFO( ptrdiff_t );
  PRINT_TYPE_INFO( time_t );

  return 0;
}

/* vim:set et sw=2 ts=2: */
