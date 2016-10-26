/* standard */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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
  me = argv[0];
  if ( --argc != 2 )
    usage();
  printf( "%llu\n", parse_ull( argv[1] ) % parse_ull( argv[2] ) );
  exit( EXIT_SUCCESS );
}

/*****************************************************************************/
/* vim:set et sw=2 ts=2: */
