#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////

int get_any_key( void ) {
  int rv;
  struct termios t;

  // get current terminal attributes ...
  if ( (rv = tcgetattr( STDIN_FILENO, &t )) == 0 ) {
    struct termios t_orig = t;          // ... and save them

    // set new terminal attributes
    t.c_lflag &= ~(ECHO | ICANON);      // disable echo & canonical mode
    t.c_cc[ VMIN ] = 1;                 // wait for at least one key
    t.c_cc[ VTIME ] = 0;                // wait indefinitely
    if ( (rv = tcsetattr( STDIN_FILENO, TCSANOW, &t )) == 0 ) {

      // read any key
      char c = (char)EOF;
      rv = (int)read( STDIN_FILENO, &c, 1 );

      // put original terminal attributes back
      int const rv2 = tcsetattr( STDIN_FILENO, TCSANOW, &t_orig );

      if ( rv != -1 )
        rv = rv2;
      if ( rv != -1 )
        rv = c;
    }
  }
  return rv;
}

///////////////////////////////////////////////////////////////////////////////

int main() {
  printf( "Press any key to continue" );
  fflush( stdout );
  int const c = get_any_key();
  printf( "\n" );
  if ( c == -1 ) {
    fprintf( stderr, "Error %d: %s\n", c, strerror( errno ) );
    exit( 1 );
  }
  printf( "Key pressed was '%c'\n", (char)c );
}

///////////////////////////////////////////////////////////////////////////////
/* vim:set et sw=2 ts=2: */
