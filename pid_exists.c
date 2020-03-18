// standard
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

///////////////////////////////////////////////////////////////////////////////

static char const *me;                  // executable name

static void usage( void ) {
  fprintf( stderr, "usage: %s <pid>\n", me );
  exit( EX_USAGE );
}

static int parse_int( char const *s ) {
  char *end = NULL;
  errno = 0;
  long const n = strtol( s, &end, 10 );
  if ( errno != 0 || end == s )
    usage();
  return (int)n;
}

static bool pid_exists( pid_t pid ) {
  errno = 0;
  int const rv = kill( pid, 0 );
  return rv == 0 || errno == EPERM;
}

///////////////////////////////////////////////////////////////////////////////

int main( int argc, char const *argv[] ) {
  me = strrchr( argv[0], '/' );         // determine base name...
  me = me ? me + 1 : argv[0];           // ...of executable

  if ( --argc != 1 )
    usage();

  pid_t const pid = parse_int( argv[1] );
  return pid_exists( pid ) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
/* vim:set et sw=2 ts=2: */
