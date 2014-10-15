#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

int const limit_width  = 22;
int const name_width   = 13;
int const sizeof_width =  2;

template<typename T>
void print_type_info( char const *type_name, T ) {
  cout << left << setw( name_width ) << type_name << right
       << setw( sizeof_width ) << sizeof( T )
       << setw( limit_width ) << numeric_limits<T>::min()
       << setw( limit_width ) << numeric_limits<T>::max()
       << endl;
}

// Specialization for char so the limits print as int rather than char.
void print_type_info( char const *type_name, char ) {
  cout << left << setw( name_width ) << type_name << right
       << setw( sizeof_width ) << sizeof( char )
       << setw( limit_width ) << (int)numeric_limits<char>::min()
       << setw( limit_width ) << (int)numeric_limits<char>::max()
       << endl;
}

#define PRINT(T) print_type_info( #T, static_cast<T>(0) )

int main() {
  PRINT( bool );
  PRINT( char );
  PRINT( wchar_t );
  PRINT( short );
  PRINT( int );
  PRINT( long );
  PRINT( long long );
  PRINT( float );
  PRINT( double );
  PRINT( long double );
  PRINT( void* );
  PRINT( size_t );
  PRINT( ptrdiff_t );
  return 0;
}

/* vim:set et sw=2 ts=2: */
