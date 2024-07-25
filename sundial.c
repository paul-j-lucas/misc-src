/*
**      sundial -- print equation-of-time & longitude corrections for a sundial
**      sundial.c
**
**      Copyright (C) 2024  Paul J. Lucas
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
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////

/**
 * Calculate the Equation of Time discrepancy for a sundial.
 *
 * @param year The year to use.  It must be &ge; 2000.
 * @param day The zero-based day within the year.
 * @return Returns the EoT discrepancy in minutes where negative values are
 * "slow."
 *
 * @sa https://en.wikipedia.org/wiki/Equation_of_time
 */
double calc_eot_min( unsigned year, unsigned day ) {
  assert( year >= 2000 );
  assert( day <= 366 );

  double const D = 6.24004077 + 0.01720197 * (365.25 * (year - 2000) + day);
  double const orbit = -7.659 * sin( D );
  double const tilt  = 9.863 * sin( 2 * D + 3.5932 );

  return orbit + tilt;
}

/**
 * Calculate the longitude discrepancy for a sundial.
 *
 * @param sundial_long The sundial's longitude.
 * @return Returns the longitude discrepancy in minutes where negative values
 * are "slow."
 */
double calc_long_min( double sundial_long ) {
  double const central_long = round( sundial_long / 15 ) * 15;
  return (sundial_long - central_long) * 4;
}

/**
 * Prints the solar hour, equation-of-time correction, longitude correction,
 * and clock time.
 *
 * @param month The 1-based month to use.
 * @param day_of_month The 1-based day of \a month.
 * @param is_dst Is daylight savings time in effect?
 * @param solar_hour The solar hour to use.
 * @param eot_min The equation-of-time discrepancy in minutes.
 * @param long_min The sundial's longitude discrepancy in minutes.
 */
void print_sundial_time( unsigned month, unsigned day_of_month, bool is_dst,
                         unsigned solar_hour, double eot_min,
                         double long_min ) {
  assert( month >= 1 && month <=12 );
  assert( day_of_month <= 31 );
  assert( solar_hour >= 6 && solar_hour <= 20 );
  assert( eot_min > -15 && eot_min < 17 );
  assert( long_min >= -30 && long_min <= 30 );

  unsigned clock_hour = solar_hour + is_dst;
  int      clock_min  = round( -(long_min + eot_min) );

  if ( clock_min < 0 ) {
    --clock_hour;
    clock_min += 60;
  }

  printf( "%2u/%02u %02u:00 %+d %+6.2f %+6.2f %02u:%02d\n",
    month, day_of_month,
    solar_hour, is_dst,
    - eot_min, - long_min,
    clock_hour, clock_min
  );
}

/**
 * The main entry point.
 */
int main( void ) {
  double const sundial_long = -122.2006;

  time_t const now = time( /*tloc=*/NULL );
  struct tm const *const tm = localtime( &now );

  print_sundial_time(
    tm->tm_mon + 1,                     // month of year (0 - 11)
    tm->tm_mday,                        // day of month (1 - 31)
    tm->tm_isdst,
    12,
    calc_eot_min( tm->tm_year + 1900, tm->tm_yday ),
    calc_long_min( sundial_long )
  );
}

///////////////////////////////////////////////////////////////////////////////
/* vim:set et sw=2 ts=2: */
