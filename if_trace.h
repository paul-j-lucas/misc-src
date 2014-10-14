#ifndef TRACE_IF_H
#define TRACE_IF_H

#ifndef TRACE_IF_PRINT
#ifdef __cplusplus
# include <iostream>
# define TRACE_IF_BOOL bool
# define TRACE_IF_PRINT(RESULT) \
    std::cout << "\"" __FILE__ "\":" << __LINE__ \
    << " 'if' == " << (RESULT) << std::endl
#else
# include <stdio.h>
# define TRACE_IF_BOOL int
# define TRACE_IF_PRINT(RESULT) \
    printf( "\"" __FILE__ "\":%d 'if' == %s\n", __LINE__, (RESULT) )
#endif /* __cplusplus */
#endif /* TRACE_IF_PRINT */

/* Yes, this many levels of indirection are necessary. */
#define TRACE_IF_HELPER3(EXPR,TEMP) \
  TRACE_IF_BOOL const TEMP = (TRACE_IF_BOOL)(EXPR); \
  TRACE_IF_PRINT( TEMP ? "true" : "false" ); \
  if ( TEMP )
#define TRACE_IF_HELPER2(EXPR,ID) TRACE_IF_HELPER3( EXPR, if_##ID )
#define TRACE_IF_HELPER1(EXPR,ID) TRACE_IF_HELPER2( EXPR, ID )

#define TRACE_IF
#ifdef TRACE_IF
# define IF(EXPR) TRACE_IF_HELPER1( EXPR, __LINE__ )
#else
# define IF(EXPR) if ( EXPR )
#endif /* TRACE_IF */

#endif /* TRACE_IF_H */
/* vim:set et sw=2 ts=2: */
