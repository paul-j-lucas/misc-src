/*
**      utf8 -- Convert to/from UTF-8
**      utf8.c
**
**      Copyright (C) 2001  Paul J. Lucas
**
**      This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2 of the License, or
**      (at your option) any later version.
** 
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
** 
**      You should have received a copy of the GNU General Public License
**      along with this program; if not, write to the Free Software
**      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// standard
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <unistd.h>

using namespace std;

#define ERROR cerr << me << ": "

typedef unsigned char   ucs1;
typedef unsigned long   ucs4;

char        buf[ 80 ];
char const* me;

ucs4        decode_utf8( ucs1 const*& );
char*       encode_utf8( ucs4 );
inline char to_lower( char c )          { return tolower( c ); }
ostream&    usage( ostream& );

//*****************************************************************************
//
// SYNOPSIS
//
      int main( int argc, char *argv[] )
//
// DESCRIPTION
//
//      Parse the command line, initialize, call other functions ... the usual
//      things that are done in main().
//
// PARAMETERS
//
//  argc    The number of arguments.
//
//  argv    A vector of the arguments; argv[argc] is null.  Aside from the
//          options below, the arguments are the names of the files and
//          directories to be indexed.
//
// SEE ALSO
//
//      Bjarne Stroustrup.  "The C++ Programming Language, 3rd ed."
//      Addison-Wesley, Reading, MA, 1997.  pp. 116-118.
//
//*****************************************************************************
{
  extern char*    optarg;
  extern int      optind, opterr;
  int             opt;
  char const*     bytes = 0;
  bool            decode = false, encode = false;

  me = ::strrchr( argv[0], '/' );       // determine base name...
  me = me ? me + 1 : argv[0];           // ...of executable

  opterr = 1;
  while ( ( opt = ::getopt( argc, argv, "b:de" ) ) != EOF )
    switch ( opt ) {
      case 'b': bytes = ::optarg; break;
      case 'd': decode = true;    break;
      case 'e': encode = true;    break;
      case '?': cerr << usage;
    }
  argc -= optind, argv += optind - 1;

  if ( !decode && !encode )
    ERROR << "one of -d or -e is required" << endl;
  if ( decode && encode )
    ERROR << "-d and -e are mutually exclusive" << endl;

  if ( bytes ) {
    int const bytes_len = ::strlen( bytes );
    char *end;
    if ( decode ) {
      if ( bytes_len % 2 ) {
        ERROR << "-b arg length must multiple of 2" << endl;
        ::exit( 2 );
      }
      ucs1 xbuf[ 80 ];
      ucs1 *p = xbuf;
      for ( ; *bytes; bytes += 2, ++p ) {
        char ubuf[ 3 ];
        ::strncpy( ubuf, bytes, 2 );
        ubuf[ 2 ] = '\0';
        *p = (ucs1)::strtol( ubuf, &end, 16 );
        if ( *end )
          ERROR << "-b arg has bad character" << endl;
      }
      *p = '\0';
      for ( ucs1 const *p = xbuf; *p; )
        cout << hex << setfill('0') << setw(4) << decode_utf8( p ) << endl;
    } else {
      if ( bytes_len % 4 ) {
        ERROR << "-b arg length must multiple of 4" << endl;
        ::exit( 2 );
      }
      for ( ; *bytes; bytes += 4 ) {
        char ubuf[ 5 ];
        ::strncpy( ubuf, bytes, 4 );
        ubuf[ 4 ] = '\0';
        ucs4 const u = ::strtol( ubuf, &end, 16 );
        if ( *end )
          ERROR << "-b arg has bad character" << endl;
        cout << hex << setfill('0') << setw(2);
        for ( char const *e = encode_utf8( u ); *e; ++e )
          cout << (unsigned short)*e;
      }
      cout << endl;
    }
  }

  return 0;
}

//*****************************************************************************
//
// SYNOPSIS
//
        ucs4 decode_utf8( ucs1 const *&begin )
//
// DESCRIPTION
//
//      Decode a UTF-8 character to a UTF-16 character.
//
// PARAMETERS
//
//      begin   A pointer to a UTF-8 character sequence.
//
// RETURN VALUE
//
//      A UTF-16 character.
//
// SEE ALSO
//
//      Francois Yergeau.  "UTF-8, a transformation format of ISO 10646,"
//      Request for Comments 2279, Network Working Group of the Internet
//      Engineering Taskforce, January 1998.
//
//*****************************************************************************
{
  register ucs1 const *c = begin;

  //
  // If the byte value is in the ASCII range, we can simply use the standard C
  // library tolower() function.
  //
  if ( *c <= 127u )
    return *begin++;

  ////////// Convert to UCS-2 ///////////////////////////////////////////////

  static unsigned char const trailing_bytes_table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // C0-CF
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // D0-DF
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // E0-EF
    3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, // F0-FF
  };
  int const trailing_bytes = trailing_bytes_table[ *c ];

  register ucs4 u = 0;
  switch ( trailing_bytes ) {
    case 5: u |= *c++; u <<= 6; if ( !*c ) break;
    case 4: u |= *c++; u <<= 6; if ( !*c ) break;
    case 3: u |= *c++; u <<= 6; if ( !*c ) break;
    case 2: u |= *c++; u <<= 6; if ( !*c ) break;
    case 1: u |= *c++; u <<= 6; if ( !*c ) break;
    case 0: u |= *c++;
  }

  static unsigned long const offset_table[] = {
    0x0, 0x3080, 0xE2080, 0x3C82080, 0xFA082080, 0x82082080
  };
  begin += trailing_bytes + 1;
  return u - offset_table[ trailing_bytes ];
}

//*****************************************************************************
//
// SYNOPSIS
//
        char *encode_utf8( ucs4 u )
//
// DESCRIPTION
//
//      Encode a single UTF-16 character to UTF-8.
//
// PARAMETERS
//
//      u   The Unicode character.
//
// RETURN VALUE
//
//      A pointer to the beginning of the UTF-8 character sequence.
//
// SEE ALSO
//
//      Francois Yergeau.  "UTF-8, a transformation format of ISO 10646,"
//      Request for Comments 2279, Network Working Group of the Internet
//      Engineering Taskforce, January 1998.
//
//*****************************************************************************
{
#if 0
  if ( u >= 0xD800 && u <= 0xDBFF ) {
    if ( c == uend )
      break;
    u = (u - 0xD800) << 10 + *++c - 0xDC00 + 0x10000;
  }
#endif
  //
  // Detemine the number of bytes the character will encode to.
  //
  int encoded_bytes;
  if      ( u <        0x80 ) encoded_bytes = 1;
  else if ( u <       0x800 ) encoded_bytes = 2;
  else if ( u <     0x10000 ) encoded_bytes = 3;
  else if ( u <    0x200000 ) encoded_bytes = 4;
  else if ( u <   0x4000000 ) encoded_bytes = 5;
  else if ( u <= 0x7FFFFFFF ) encoded_bytes = 6;

  //
  // Stuff the encoded bytes into the output buffer in reverse order.
  //
  static ucs1 const first_byte_table[] = {
    0, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
  };
  register char *p = buf + encoded_bytes;
  switch ( encoded_bytes ) {            // yes, no breaks
    case 6: *--p = ucs1( (u | 0x80u) & 0xBFu ); u >>= 6;
    case 5: *--p = ucs1( (u | 0x80u) & 0xBFu ); u >>= 6;
    case 4: *--p = ucs1( (u | 0x80u) & 0xBFu ); u >>= 6;
    case 3: *--p = ucs1( (u | 0x80u) & 0xBFu ); u >>= 6;
    case 2: *--p = ucs1( (u | 0x80u) & 0xBFu ); u >>= 6;
    case 1: *--p = ucs1(  u | first_byte_table[encoded_bytes] );
  }
  p += encoded_bytes;

  *p = '\0';
  return buf;
}

////////// Miscellaneous //////////////////////////////////////////////////////

ostream& usage( ostream &o ) {
  o << "usage: " << me << " {-d|-e} xxx" << endl;
  ::exit( 1 );
  return o;                             // just to make compiler happy
}

/* vim:set et sw=2 ts=2: */
