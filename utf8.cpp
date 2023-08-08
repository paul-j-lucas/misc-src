/*
**      utf8 -- Convert to/from UTF-8
**      utf8.cpp
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

// local
#include "omanip.h"

// standard
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
#include <unistd.h>

using namespace std;

#define ERROR cerr << me << ": "

inline bool is_big_endian() {
  int const x = 1;
  return !*reinterpret_cast<char const*>( &x );
}

////////// UTF-8 //////////////////////////////////////////////////////////////

namespace utf8 {

/**
 * The byte type that variable-length encoded UTF-8 characters use for storage.
 */
typedef char byte_type;

/**
 * A type that can hold all the bytes of the largest encoded UTF-8 character.
 * Note that this is NOT a C string: it is NOT null-terminated (since the first
 * byte of a UTF-8 byte sequence encodes the number of bytes in the sequence).
 */
typedef byte_type char_type[6];

/**
 * The size type.
 */
typedef size_t size_type;

/**
 * UTF-8 character length table.  The index is the first byte of a UTF-8
 * character; the value is the number of bytes comprising said character [0-6].
 * A zero value indicates an invalid start byte.
 */
static char const len_table[] = {
  /*      0 1 2 3 4 5 6 7 8 9 A B C D E F */
  /* 0 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 1 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 2 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 3 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 4 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 5 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 6 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 7 */ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  /* 8 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // continuation bytes
  /* 9 */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* A */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* B */ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  //        |
  /* C */ 0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  // C0 & C1 are overlong ASCII
  /* D */ 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  /* E */ 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  /* F */ 4,4,4,4,4,4,4,4,5,5,5,5,6,6,0,0
};

/**
 * Byte Order Mark (BOM).
 */
byte_type const BOM[] = "\xEF\xBB\xBF";

/**
 * An %invalid_byte is-an invalid_argument for reporting invalid UTF-8 bytes.
 */
class invalid_byte : public std::invalid_argument {
public:
  invalid_byte( byte_type byte );
  ~invalid_byte() throw();

  byte_type byte() const throw() {
    return byte_;
  }

private:
  static std::string make_what( byte_type byte );
  byte_type byte_;
};

invalid_byte::invalid_byte( char byte ) :
  invalid_argument( make_what( byte ) ),
  byte_( byte )
{
}

invalid_byte::~invalid_byte() throw() {
  // out-of-line since it's virtual
}

string invalid_byte::make_what( utf8::byte_type byte ) {
  ostringstream oss;
  oss << '\'' << printable_char( byte ) << "': invalid UTF-8 byte";
  return oss.str();
}

/**
 * Gets the number of bytes needed to encode the given code-point.
 *
 * @param cp The Unicode code-point to encode.
 * @return Returns the number of byted needed to encode \a cp [1-6] or 0 if
 * invalid.
 */
inline int bytes_for( unsigned long cp ) {
  if ( cp <       0x80 ) return 1;
  if ( cp <      0x800 ) return 2;
  if ( cp <    0x10000 ) return 3;
  if ( cp <   0x200000 ) return 4;
  if ( cp <  0x4000000 ) return 5;
  if ( cp < 0x80000000 ) return 6;
  return 0;
}

/**
 * Gets the number of bytes used by a UTF-8 character.
 *
 * @param start The start byte of a UTF-8 byte sequence comprising a Unicode
 * character.
 * @return Returns the numer of bytes comprising the UTF-8 character or 0 if
 * the \a start is invalid.
 */
inline int char_len( byte_type start ) {
  return len_table[ static_cast<unsigned char>( start ) ];
}

/**
 * Checks whether the given byte is a continuation byte of a UTF-8 byte
 * sequence comprising an encoded character.  Note that this is not equivalent
 * to \c !is_start_byte(b).
 *
 * @param b The byte to check.
 * @return Returns \c true only if the byte is not the first byte of a UTF-8
 * byte sequence comprising an encoded character.
 */
inline bool is_continuation_byte( byte_type b ) {
  unsigned char const u = b;
  return u >= 128 && u < 192;
}

/**
 * Checks whether the given byte is the first byte of a UTF-8 byte sequence
 * comprising an encoded character.  Note that this is not equivalent to
 * \c !is_continuation_byte(b).
 *
 * @param b The byte to check.
 * @return Returns \c true only if the byte is the first byte of a UTF-8 byte
 * sequence comprising an encoded character.
 */
inline bool is_start_byte( byte_type b ) {
  unsigned char const u = b;
  return u < 128 || (u >= 194 && u < 254);
}

/**
 * Checks whether the given byte is a valid byte in a UTF-8 byte sequence
 * comprising an encoded character.
 *
 * @param b The byte to check.
 * @param check_start_byte If \c true, checks for a valid start byte; if \a
 * false, checks for a valid continuation byte.
 * @return Returns \c true only if the byte is valid.
 */
inline bool is_valid_byte( byte_type b, bool check_start_byte ) {
  return check_start_byte ? is_start_byte( b ) : is_continuation_byte( b );
}

/**
 * Checks whether the given byte is a valid byte in a UTF-8 byte sequence
 * comprising an encoded character.
 *
 * @param b The byte to check.
 * @param check_start_byte If \c true, checks for a valid start byte; if \a
 * false, checks for a valid continuation byte.
 * @throws invalid_byte if \a b is invalid.
 */
inline void check_valid_byte( byte_type b, bool check_start_byte ) {
  if ( !is_valid_byte( b, check_start_byte ) )
    throw invalid_byte( b );
}

} // namespace utf8

////////// UTF-16 /////////////////////////////////////////////////////////////

namespace utf16 {

typedef char16_t char_type;

/**
 * Byte Order Mark (BOM), big-endian.
 */
char_type const BOM_BE = 0xFEFF;

/**
 * Byte Order Mark (BOM), little-endian.
 */
char_type const BOM_LE = 0xFFFE;

} // namespace utf16

////////// UTF-32 ////////////////////////////////////////////////////////////

namespace utf32 {

typedef char32_t char_type;

/**
 * Byte Order Mark (BOM), big-endian.
 */
char_type const BOM_BE = 0x0000FEFF;

/**
 * Byte Order Mark (BOM), little-endian.
 */
char_type const BOM_LE = 0xFFFE0000;

} // namespace utf32

////////// Types //////////////////////////////////////////////////////////////

namespace unicode {

/**
 * A Unicode code-point.
 */
typedef utf32::char_type code_point;

/**
 * Converts the given high and low surrogate values into the code-point they
 * represent.  Note that no checking is done on the parameters.
 *
 * @param high The high surrogate value.
 * @param low The low surrogate value.
 * @return Returns the represented code-point.
 * @see is_high_surrogate()
 * @see is_low_surrogate()
 */
inline code_point convert_surrogate( unsigned high, unsigned low ) {
  return 0x10000 + ((high - 0xD800) << 10) + (low - 0xDC00);
}

/**
 * Converts the given code-point into the high and low surrogate values that
 * represent it.  Note that no checking is done on the parameters.
 *
 * @tparam ResultType The integer type for the results.
 * @param cp The code-point to convert.
 * @param high A pointer to where to put the high surrogate.
 * @param low A pointer to where to put the low surrogate.
 */
template<typename ResultType>
inline void convert_surrogate( code_point cp, ResultType *high,
                               ResultType *low ) {
  code_point const n = cp - 0x10000;
  *high = 0xD800 + (static_cast<unsigned>(n) >> 10);
  *low  = 0xDC00 + (n & 0x3FF);
}

/**
 * Checks whether the given value is a "high surrogate."
 *
 * @param n The value to check.
 * @return Returns \c true only if \a n is a high surrogate.
 */
inline bool is_high_surrogate( unsigned long n ) {
  return n >= 0xD800 && n <= 0xDBFF;
}

/**
 * Checks whether the given value is a "low surrogate."
 *
 * @param n The value to check.
 * @return Returns \c true only if \a n is a low surrogate.
 */
inline bool is_low_surrogate( unsigned long n ) {
  return n >= 0xDC00 && n <= 0xDFFF;
}

/**
 * Checks whether the given code-point is in the "supplementary plane" and
 * therefore would need a surrogate pair to be encoded in UTF-16.
 *
 * @param cp The code-point to check.
 * @return Returns \c true only if \a cp is within the supplementary plane.
 */
inline bool is_supplementary_plane( code_point cp ) {
  return cp >= 0x10000 && cp <= 0x10FFFF;
}

/**
 * Checks whether the given Unicode code-point is valid.
 *
 * @param cp The code-point to check.
 * @return Returns \c true only if the code-point is valid.
 */
inline bool is_valid( code_point cp ) {
  return                     cp <= 0x00D7FF
      ||  (cp >= 0x00E000 && cp <= 0x00FFFD)
      ||  is_supplementary_plane( cp );
}

} // namespace unicode

////////// Global variables ///////////////////////////////////////////////////

char const* me;

///////////////////////////////////////////////////////////////////////////////

/**
 * Prints the given character in a printable way: if \c isprint(c) is \c true,
 * prints \a c as-is; otherwise prints \c #x followed by the hexadecimal value
 * of the character.
 *
 * @param o The ostream to print to.
 * @param c The \c char to print.
 * @return Returns \a o.
 */
ostream& printable_char( ostream &o, char c ) {
  if ( isprint( c ) )
    o << c;
  else
    switch ( c ) {
      case '\n': o << "\\n"; break;
      case '\r': o << "\\r"; break;
      case '\t': o << "\\t"; break;
      default: {
        ios::fmtflags const old_flags = o.flags();
        o << "#x" << uppercase << hex << (static_cast<unsigned>( c ) & 0xFF);
        o.flags( old_flags );
      }
    } // switch
  return o;
}

// An ostream manipulator version of the above.
DEF_OMANIP1( printable_char char )

///////////////////////////////////////////////////////////////////////////////

namespace utf8 {

/**
 * Decodes a UTF-8 character to a Unicode codepoint character.
 *
 * @param pp A pointer to a pointer to a UTF-8 character sequence.
 * Upon return, this is advanced by the number of bytes comprising the UTF-8
 * character.
 * @return A Unicode code-point.
 * @see Francois Yergeau.  "UTF-8, a transformation format of ISO 10646,"
 * Request for Comments 2279, Network Working Group of the Internet Engineering
 * Taskforce, January 1998.
 */
unicode::code_point decode( byte_type const **pp ) {
  byte_type const *&p = *pp;
  //
  // Trivial if the byte value is in the ASCII range.
  //
  if ( *p <= 127u )
    return *p++;

  ////////// Convert to code-point ////////////////////////////////////////////

  byte_type c;
  int const len = char_len( *p );
  bool is_start = true;

#define DECODE_1 \
  c = *p; check_valid_byte( c, is_start ); cp += c

#define DECODE_N \
  DECODE_1; cp <<= 6; is_start = false; ++p

  try {
    unicode::code_point cp = 0;
    switch ( len ) {
      case 6: DECODE_N;
      case 5: DECODE_N;
      case 4: DECODE_N;
      case 3: DECODE_N;
      case 2: DECODE_N;
      case 1: DECODE_1; break;
      case 0: throw invalid_byte( c );
    } // switch

    static unicode::code_point const offset_table[] = {
      0, // unused
      0x0, 0x3080, 0xE2080, 0x3C82080, 0xFA082080, 0x82082080
    };
    return cp - offset_table[ len ];
  }
  catch ( invalid_byte const &e ) {
    // TODO
  }
}

/**
 * Encodes a Unicode code-point to a UTF-8 byte sequence.
 *
 * @param cp The Unicode code-point to encode.
 * @param pp A pointer to a pointer to what will be the first byte of a UTF-8
 * byte sequence.  The pointer is advanced to one byte past the newly encoded
 * character.
 * @return The number of bytes required to encode \a cp or 0 if \a cp is
 * invalid.
 * @see Francois Yergeau.  "UTF-8, a transformation format of ISO 10646,"
 * Request for Comments 2279, Network Working Group of the Internet Engineering
 * Taskforce, January 1998.
 */
size_type encode( unicode::code_point cp, byte_type **pp ) {
  if ( !unicode::is_valid( cp ) )
    return 0;
  size_type const size = bytes_for( cp );

  //
  // Stuff the encoded bytes into the output buffer in reverse order.
  //
  static unsigned char const start_byte_table[] = {
    0, // unused
    0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
  };
  byte_type *&p = *pp;
  p += size;
  switch ( size ) {                     // yes, no breaks
    case 6: *--p = byte_type( (cp | 0x80u) & 0xBFu ); cp >>= 6;
    case 5: *--p = byte_type( (cp | 0x80u) & 0xBFu ); cp >>= 6;
    case 4: *--p = byte_type( (cp | 0x80u) & 0xBFu ); cp >>= 6;
    case 3: *--p = byte_type( (cp | 0x80u) & 0xBFu ); cp >>= 6;
    case 2: *--p = byte_type( (cp | 0x80u) & 0xBFu ); cp >>= 6;
    case 1: *--p = byte_type(  cp | start_byte_table[ size ] );
  }
  p += size;
  return size;
}

} // namespace utf8

/**
 * Print the usage message and exit.
 */
static void usage() {
  cerr <<
"usage: " << me << " {-de} {-16 | -32} [-bEW] [bytes]\n"
"\n"
"-b : Include BOM in output\n"
"-d : Decode from UTF-8\n"
"-e : Encode to UTF-8\n"
"-16: Decode/encode UTF-16\n"
"-32: Decode/encode UTF-32\n"
"-E : Error on an invalid byte\n"
"-W : Warn about invalid bytes\n"
  ;
  ::exit( 1 );
}

///////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] ) {
  int  opt_utf    = 0;
  bool opt_bom    = false;
  bool opt_decode = false;
  bool opt_encode = false;
  bool opt_error  = false;
  bool opt_warn   = false;

  me = ::strrchr( argv[0], '/' );       // determine base name...
  me = me ? me + 1 : argv[0];           // ...of executable

  int opt;
  opterr = 1;
  while ( ( opt = ::getopt( argc, argv, "12368eEW" ) ) != EOF ) {
    switch ( opt ) {
      case '1':
      case '6': opt_utf    = 16;    break;
      case '3':
      case '2': opt_utf    = 32;    break;
      case 'b': opt_bom    = true;  break;
      case 'd': opt_decode = true;  break;
      case 'e': opt_encode = true;  break;
      case 'E': opt_error  = true;  break;
      case 'W': opt_warn   = true;  break;
      case '?': usage();
    } // switch
  } // while

  if ( !opt_utf )
    ERROR << "one of -1 or -3 is required" << endl;
  if ( !opt_decode && !opt_encode )
    ERROR << "one of -d or -e is required" << endl;
  if ( opt_decode && opt_encode )
    ERROR << "-d and -e are mutually exclusive" << endl;
  if ( opt_error && opt_warn )
    ERROR << "-E and -W are mutually exclusive" << endl;

  argc -= optind, argv += optind - 1;
  if ( !argc )
    usage();

  char const *bytes;
  if ( argv[0] ) {
    int const chunk_size = opt_utf / 8;
    size_t len = strlen( argv[0] );
    if ( len % chunk_size )
      ERROR << "command-line argument is not a multiple of " << chunk_size << endl;
    char buf[ 16 ];
    ::memset( buf, 0, sizeof( buf ) );
    char const *src = argv[0];
    for ( ; *src; src += chunk_size ) {
      ::memcpy( buf, src, chunk_size );
      char *end;
      (utf8::byte_type)::strtol( buf, &end, 16 );
    }

    bytes = new char[ len ];
  } else {
  }

  char const *bytes= argv[0];

  if ( opt_decode ) {

    ////////// decode

    utf8::byte_type xbuf[ 80 ];
    utf8::byte_type *p = xbuf;
    for ( ; *bytes; bytes += 2, ++p ) {
      static char ubuf[ 3 ];
      ::strncpy( ubuf, bytes, 2 );
      *p = (utf8::byte_type)::strtoul( ubuf, &end, 16 );
      if ( *end )
        ERROR << "-b arg has bad character" << endl;
    }
    *p = '\0';
    for ( byte_type const *p = xbuf; *p; )
      cout << hex << setfill('0') << setw(4) << utf8::decode( &p ) << endl;

  } else {

    ////////// encode

    for ( ; *bytes; bytes += 4 ) {
      static char ubuf[ 5 ];
      ::strncpy( ubuf, bytes, 4 );
      unicode::code_point const cp = ::strtol( ubuf, &end, 16 );
      if ( *end )
        ERROR << "bytes argument has bad character" << endl;
      cout << hex << setfill('0') << setw(2);
      utf8::char_type utf8[6];
      for ( char const *e = utf8::encode( cp, utf8 ); *e; ++e )
        cout << static_cast<unsigned short>( *e );
    }
    cout << endl;

  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

/* vim:set et sw=2 ts=2: */
