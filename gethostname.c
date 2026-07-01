#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>                  /* for MAXHOSTNAMELEN */
#include <unistd.h>

int main( void ) {
  char hostname[ MAXHOSTNAMELEN ];
  if ( gethostname( hostname, sizeof hostname ) == -1 ) {
    perror( NULL );
    exit( 1 );
  }
  puts( hostname );
}

/* vim:set et sw=2 ts=2: */
