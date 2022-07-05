/*
**      args -- command-line individual argument printer
**      args.c
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
#include <libgen.h>                     /* for basename(3) */
#include <stdio.h>
#include <stdlib.h>                     /* for exit(3) */
#include <string.h>
#include <unistd.h>                     /* for getopt(3) */

/* exit(3) status codes */
#define EXIT_USAGE  64

#define GET_SUPPRESS(X) (suppress_flags &  SUPPRESS_##X)
#define SET_SUPPRESS(X) (suppress_flags |= SUPPRESS_##X)

char const *me;                         /* executable name */

static void usage( void ) {
  fprintf( stderr,
"usage: %s [-0hn] [-e] ...\n"
"\n"
"-0: Suppress printing terminating null argument.\n"
"-h: Suppress printing header.\n"
"-n: Suppress printing argc numbers.\n"
"-e: Suppress printing everything except actual arguments.\n"
    "", me
  );
  exit( EXIT_USAGE );
}

/*****************************************************************************/

int main( int argc, char *argv[] ) {
  enum {
    SUPPRESS_HEADER  = 1 << 0,
    SUPPRESS_NULL    = 1 << 1,
    SUPPRESS_NUMBERS = 1 << 2,
  };

  char const *const argv0_orig = argv[0];
  unsigned suppress_flags = 0;

  me = basename( argv[0] );

  optind = opterr = 1;
  for ( int opt; (opt = getopt( argc, argv, "0ehn" )) != EOF; ) {
    switch ( opt ) {
      case '0': SET_SUPPRESS( NULL );    break;
      case 'e': suppress_flags = ~0u;    break;
      case 'h': SET_SUPPRESS( HEADER );  break;
      case 'n': SET_SUPPRESS( NUMBERS ); break;
      default : usage();
    } // switch
  } // while
  argc -= optind, argv += optind;

  if ( !GET_SUPPRESS( HEADER ) )
    printf( "argc argv\n" );

  int i;
  if ( GET_SUPPRESS( NUMBERS ) ) {
    printf( "%s\n", argv0_orig );
    for ( i = 0; i < argc; ++i )
      printf( "%s\n", argv[i] );
    if ( !GET_SUPPRESS( NULL ) )
      printf( "\\0\n" );
  } else {
    printf( "%4d %s\n", 0, argv0_orig );
    for ( i = 0; i < argc; ++i )
      printf( "%4d %s\n", i+1, argv[i] );
    if ( !GET_SUPPRESS( NULL ) )
      printf( "%4d \\0\n", i+1 );
  }

  exit( EXIT_SUCCESS );
}

/*****************************************************************************/
/* vim:set et sw=2 ts=2: */
