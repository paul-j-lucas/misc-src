#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char hostname[ 256 ];
  int result = gethostname( hostname, sizeof( hostname ) );
  if ( result == -1 ) {
    perror( NULL );
    exit( EXIT_FAILURE );
  }
  printf( "%s\n", hostname );
  exit( EXIT_SUCCESS );
}
/* vim:set et sw=2 ts=2: */
