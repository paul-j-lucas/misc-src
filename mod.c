/*
**      mod -- modulus calculator
**      mod.c
**
**      Copyright (C) 2016  Paul J. Lucas
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

/* standard */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/

static char const *me;                  /* executable name */

static void usage( void ) {
  fprintf( stderr, "usage: %s <n> <m>\n", me );
  exit( 1 );
}

static unsigned long long parse_ull( char const *s ) {
  char *end;
  errno = 0;
  unsigned long long const n = strtoull( s, &end, 10 );
  if ( errno )
    usage();
  return n;
}

/*****************************************************************************/

int main( int argc, char const *argv[] ) {
  me = strrchr( argv[0], '/' );         /* determine base name... */
  me = me ? me + 1 : argv[0];           /* ...of executable */

  if ( --argc != 2 )
    usage();
  printf( "%llu\n", parse_ull( argv[1] ) % parse_ull( argv[2] ) );
  exit( EXIT_SUCCESS );
}

/*****************************************************************************/
/* vim:set et sw=2 ts=2: */
