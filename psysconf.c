/*
**      psysconf -- print most values of sysconf(3)
**      psysconf.c
**
**      Copyright (C) 2015  Paul J. Lucas
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

// standard
#include <stdio.h>
#include <unistd.h>

#define PRINT_SYSCONF(C) printf( #C " = %ld\n", sysconf( _SC_##C ) )

int main( void ) {
  PRINT_SYSCONF( ARG_MAX );
  PRINT_SYSCONF( CHILD_MAX );
  PRINT_SYSCONF( HOST_NAME_MAX );
  PRINT_SYSCONF( LOGIN_NAME_MAX );
  PRINT_SYSCONF( NGROUPS_MAX );
  PRINT_SYSCONF( NPROCESSORS_CONF );
  PRINT_SYSCONF( NPROCESSORS_ONLN );
  PRINT_SYSCONF( OPEN_MAX );
  PRINT_SYSCONF( PAGESIZE );
  PRINT_SYSCONF( STREAM_MAX );
  PRINT_SYSCONF( SYMLOOP_MAX );
  PRINT_SYSCONF( TTY_NAME_MAX );
  PRINT_SYSCONF( TZNAME_MAX );
}

/* vim:set et sw=2 ts=2: */
