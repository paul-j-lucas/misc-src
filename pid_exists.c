// standard
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

////////// local variables ////////////////////////////////////////////////////

static char const *me;                  // executable name

////////// local functions ////////////////////////////////////////////////////

_Noreturn static void print_usage( void ) {
  fprintf( stderr, "usage: %s <pid>\n", me );
  exit( EX_USAGE );
}

static int parse_int( char const *s ) {
  char *end = NULL;
  errno = 0;
  long const n = strtol( s, &end, 10 );
  if ( errno == 0 && *end == '\0' )
    return (int)n;
  print_usage();
}

static bool pid_exists( pid_t pid ) {
  errno = 0;
  int const rv = kill( pid, 0 );
  return rv == 0 || errno == EPERM;
}

////////// extern functions ///////////////////////////////////////////////////

int main( int argc, char const *argv[] ) {
  me = strrchr( argv[0], '/' );         // determine base name...
  me = me ? me + 1 : argv[0];           // ...of executable

  if ( --argc != 1 )
    print_usage();

  pid_t const pid = parse_int( argv[1] );
  return pid_exists( pid ) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////////
/* vim:set et sw=2 ts=2: */
