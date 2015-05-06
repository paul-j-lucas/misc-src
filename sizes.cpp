/*
**      sizes -- print C++ types' sizes and limits
**      sizes.cpp
**
**      Copyright (C) 2014  Paul J. Lucas
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

/* system */
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

int const limit_width  = 22;
int const name_width   = 21;
int const sizeof_width =  2;

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

template<typename T>
void print_type_info( char const *type_name, T ) {
  cout << left << setw( name_width ) << type_name << right
       << setw( sizeof_width ) << sizeof( T )
       << setw( limit_width ) << number_limit( numeric_limits<T>::min() )
       << setw( limit_width ) << number_limit( numeric_limits<T>::max() )
       << endl;
}

#define PRINT_TYPE_INFO(T) print_type_info( #T, static_cast<T>(0) )

int main() {
  PRINT_TYPE_INFO( bool );

  PRINT_TYPE_INFO( char );
  PRINT_TYPE_INFO( signed char );
  PRINT_TYPE_INFO( unsigned char );
  PRINT_TYPE_INFO( wchar_t );

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
  PRINT_TYPE_INFO( ptrdiff_t );
  PRINT_TYPE_INFO( time_t );

  return 0;
}

/* vim:set et sw=2 ts=2: */
