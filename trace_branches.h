/*
**      Trace branch-points ("if" and "switch") in C or C++ code
**      trace_branches.h
**
**      Copyright (C) 2014  Paul J. Lucas
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

#ifndef TRACE_BRANCHES_H
#define TRACE_BRANCHES_H

/*****************************************************************************/

#if defined(TRACE_IF) || defined(TRACE_SWITCH)
/*
 * If you want to specify how traces are printed, define your own version of
 * TRACE_BRANCH_PRINT before #include'ing this file.
 */
#ifndef TRACE_BRANCH_PRINT
#ifdef __cplusplus
# include <iostream>
# define TRACE_BRANCH_PRINT(WHAT,RESULT) \
    std::cout << "\"" __FILE__ "\":" << __LINE__ \
    << " '" WHAT "' == " << (RESULT) << std::endl
#else
# include <stdio.h>
# define TRACE_BRANCH_PRINT(WHAT,RESULT) \
    printf( "\"" __FILE__ "\":%d '" WHAT "' == %s\n", __LINE__, (RESULT) )
#endif /* __cplusplus */
#endif /* TRACE_BRANCH_PRINT */

#endif /* defined(TRACE_IF) || defined(TRACE_SWITCH) */

/*****************************************************************************/

#ifdef TRACE_IF
#ifdef __cplusplus
# define TRACE_IF_BOOL bool
#else
# define TRACE_IF_BOOL int
#endif /* __cplusplus */
  /* Yes, this many levels of indirection are necessary. */
# define TRACE_IF_HELPER3(EXPR,TEMP) \
    TRACE_IF_BOOL const TEMP = (TRACE_IF_BOOL)(EXPR); \
    TRACE_BRANCH_PRINT( "if", TEMP ? "true" : "false" ); \
    if ( TEMP )
# define TRACE_IF_HELPER2(EXPR,ID) TRACE_IF_HELPER3( EXPR, if_##ID )
# define TRACE_IF_HELPER1(EXPR,ID) TRACE_IF_HELPER2( EXPR, ID )
# define IF(EXPR) TRACE_IF_HELPER1( EXPR, __LINE__ )
#else
# define IF(EXPR) if ( EXPR )
#endif /* TRACE_IF */

/*****************************************************************************/

/* Note that TRACE_SWITCH requires C++11.  */

#if defined(TRACE_SWITCH) && __cplusplus >= 201103L
  /* Yes, this many levels of indirection are necessary. */
# define TRACE_SWITCH_HELPER3(EXPR,TEMP) \
    auto const TEMP( EXPR ); \
    TRACE_BRANCH_PRINT( "switch", TEMP ); \
    switch ( TEMP )
# define TRACE_SWITCH_HELPER2(EXPR,ID) TRACE_SWITCH_HELPER3( EXPR, switch_##ID )
# define TRACE_SWITCH_HELPER1(EXPR,ID) TRACE_SWITCH_HELPER2( EXPR, ID )
# define SWITCH(EXPR) TRACE_SWITCH_HELPER1( EXPR, __LINE__ )
#else
# define SWITCH(EXPR) switch ( EXPR )
#endif /* TRACE_SWITCH */

/*****************************************************************************/

#endif /* TRACE_BRANCHES_H */
/* vim:set et sw=2 ts=2: */
