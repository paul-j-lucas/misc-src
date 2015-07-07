#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char hostname[ 256 ];
  int result = gethostname( hostname, sizeof( hostname ) );
  if ( result == -1 ) {
    perror( NULL );
    exit( 1 );
  }
  printf( "%s\n", hostname );
  exit( 0 );
}
/* vim:set et sw=2 ts=2: */
