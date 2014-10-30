/*
**      omanip -- output stream manipulator definition helper
**      omanip.h
**
**      Copyright (C) 2001  Paul J. Lucas
**
**      This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2 of the License, or
**      (at your option) any later version.
** 
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
** 
**      You should have received a copy of the GNU General Public License
**      along with this program; if not, write to the Free Software
**      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef OMANIP_H
#define OMANIP_H

// standard
#include <iostream>

namespace PJL {

///////////////////////////////////////////////////////////////////////////////

/**
 * An %omanip1 is a class for assisting in the creation of ostream manipulators
 * by storing a pointer to a function and its argument to be called later via
 * operator<<().
 *
 * See also: "Standard C++ IOStreams and Locales," Angelika Langer and Klaus
 * Kreft, Addison-Wesley, pp. 179-191.
 */
template<typename Arg1Type>
class omanip1 {
public:

  /**
   * The signature of functions this %omanip1 can handle.
   */
  typedef std::ostream& (*func_type)( std::ostream&, Arg1Type );

  /**
   * Constructs an %omanip1.
   *
   * @param f   The function to call when this %omanip1 is inserted into an
   *            ostream.
   * @param a1  The argument to be passed to the function.
   */
  omanip1( func_type f, Arg1Type a1 ) :
    f_( f ), a1_( a1 )
  {
  }

  /**
   * Inserts the given %omanip1 into the given ostream.  This has the effect of
   * calling the function and argument bound to the %omanip1 at the time of its
   * construction.
   *
   * @param o The ostream to insert into.
   * @param m The %omanip1 to insert.
   */
  friend std::ostream& operator<<( std::ostream &o, omanip1 const &m ) {
    if ( o.good() )
      (*m.f_)( o, m.a1_ );
    return o;
  }

private:
  func_type const f_;
  Arg1Type const a1_;
};

/**
 * Defines an ostream manipulator "thunk" function that calls an existing
 * non-manipulator function having the same name.
 *
 * @param FN_NAME The name of the existing function.
 * @param ARG1_T  The type of the non-ostream argument.
 */
#define DEF_OMANIP1(FN_NAME,ARG1_T)             \
  inline PJL::omanip1<ARG1_T>                   \
  FN_NAME( ARG1_T a1 ) {                        \
    return PJL::omanip1<ARG1_T>( FN_NAME, a1 ); \
  }

///////////////////////////////////////////////////////////////////////////////

/**
 * An %omanip2 is a class for assisting in the creation of ostream manipulators
 * by storing a pointer to a function and its arguments to be called later via
 * operator<<().
 *
 * See also: "Standard C++ IOStreams and Locales," Angelika Langer and Klaus
 * Kreft, Addison-Wesley, pp. 179-191.
 */
template<typename Arg1Type,typename Arg2Type>
class omanip2 {
public:

  /**
   * The signature of functions this %omanip2 can handle.
   */
  typedef std::ostream& (*func_type)( std::ostream&, Arg1Type, Arg2Type );

  /**
   * Constructs an %omanip2.
   *
   * @param f   The function to call when this %omanip2 is inserted into an
   *            ostream.
   * @param a1  The first argument to be passed to the function.
   * @param a2  The second argument to be passed to the function.
   */
  omanip2( func_type f, Arg1Type a1, Arg2Type a2 ) :
    f_( f ), a1_( a1 ), a2_( a2 )
  {
  }

  /**
   * Inserts the given %omanip2 into the given ostream.  This has the effect of
   * calling the function and argument bound to the %omanip2 at the time of its
   * construction.
   *
   * @param o The ostream to insert into.
   * @param m The %omanip2 to insert.
   */
  friend std::ostream& operator<<( std::ostream &o, omanip2 const &m ) {
    if ( o.good() )
      (*m.f_)( o, m.a1_, m.a2_ );
    return o;
  }

private:
  func_type const f_;
  Arg1Type const a1_;
  Arg2Type const a2_;
};

/**
 * Defines an ostream manipulator "thunk" function that calls an existing
 * non-manipulator function having the same name.
 *
 * @param FN_NAME The name of the existing function.
 * @param ARG1_T  The type of the first non-ostream argument.
 * @param ARG2_T  The type of the second non-ostream argument.
 */
#define DEF_OMANIP2(FN_NAME,ARG1_T,ARG2_T)                  \
  inline PJL::omanip2<ARG1_T,ARG2_T>                        \
  FN_NAME( ARG1_T a1, ARG2_T a2 ) {                         \
    return PJL::omanip2<ARG1_T,ARG2_T>( FN_NAME, a1, a2 );  \
  }

///////////////////////////////////////////////////////////////////////////////

/**
 * An %omanip3 is a class for assisting in the creation of ostream manipulators
 * by storing a pointer to a function and its arguments to be called later via
 * operator<<().
 *
 * See also: "Standard C++ IOStreams and Locales," Angelika Langer and Klaus
 * Kreft, Addison-Wesley, pp. 179-191.
 */
template<typename Arg1Type,typename Arg2Type,typename Arg3Type>
class omanip3 {
public:

  /**
   * The signature of functions this %omanip3 can handle.
   */
  typedef std::ostream& (*func_type)( std::ostream&, Arg1Type, Arg2Type,
                                      Arg3Type );

  /**
   * Constructs an %omanip3.
   *
   * @param f   The function to call when this %omanip3 is inserted into an
   *            ostream.
   * @param a1  The first argument to be passed to the function.
   * @param a2  The second argument to be passed to the function.
   * @param a3  The third argument to be passed to the function.
   */
  omanip3( func_type f, Arg1Type a1, Arg2Type a2, Arg3Type a3 ) :
    f_( f ), a1_( a1 ), a2_( a2 ), a3_( a3 )
  {
  }

  /**
   * Inserts the given %omanip3 into the given ostream.  This has the effect of
   * calling the function and argument bound to the %omanip3 at the time of its
   * construction.
   *
   * @param o The ostream to insert into.
   * @param m The %omanip3 to insert.
   */
  friend std::ostream& operator<<( std::ostream &o, omanip3 const &m ) {
    if ( o.good() )
      (*m.f_)( o, m.a1_, m.a2_, m.a3_ );
    return o;
  }

private:
  func_type const f_;
  Arg1Type const a1_;
  Arg2Type const a2_;
  Arg3Type const a3_;
};

/**
 * Defines an ostream manipulator "thunk" function that calls an existing
 * non-manipulator function having the same name.
 *
 * @param FN_NAME The name of the existing function.
 * @param ARG1_T  The type of the first non-ostream argument.
 * @param ARG2_T  The type of the second non-ostream argument.
 * @param ARG3_T  The type of the third non-ostream argument.
 */
#define DEF_OMANIP3(FN_NAME,ARG1_T,ARG2_T,ARG3_T)                     \
  inline PJL::omanip3<ARG1_T,ARG2_T,ARG3_T>                           \
  FN_NAME( ARG1_T a1, ARG2_T a2, ARG3_T a3 ) {                        \
    return PJL::omanip3<ARG1_T,ARG2_T,ARG3_T>( FN_NAME, a1, a2, a3 ); \
  }

///////////////////////////////////////////////////////////////////////////////

/**
 * An %omanip4 is a class for assisting in the creation of ostream manipulators
 * by storing a pointer to a function and its arguments to be called later via
 * operator<<().
 *
 * See also: "Standard C++ IOStreams and Locales," Angelika Langer and Klaus
 * Kreft, Addison-Wesley, pp. 179-191.
 */
template<
  typename Arg1Type,typename Arg2Type,typename Arg3Type,typename Arg4Type
>
class omanip4 {
public:

  /**
   * The signature of functions this %omanip4 can handle.
   */
  typedef std::ostream& (*func_type)( std::ostream&, Arg1Type, Arg2Type,
                                      Arg3Type, Arg4Type );

  /**
   * Constructs an %omanip4.
   *
   * @param f   The function to call when this %omanip4 is inserted into an
   *            ostream.
   * @param a1  The first argument to be passed to the function.
   * @param a2  The second argument to be passed to the function.
   * @param a3  The third argument to be passed to the function.
   * @param a4  The fourth argument to be passed to the function.
   */
  omanip4( func_type f, Arg1Type a1, Arg2Type a2, Arg3Type a3, Arg4Type a4 ) :
    f_( f ), a1_( a1 ), a2_( a2 ), a3_( a3 ), a4_( a4 )
  {
  }

  /**
   * Inserts the given %omanip4 into the given ostream.  This has the effect of
   * calling the function and argument bound to the %omanip4 at the time of its
   * construction.
   *
   * @param o The ostream to insert into.
   * @param m The %omanip4 to insert.
   */
  friend std::ostream& operator<<( std::ostream &o, omanip4 const &m ) {
    if ( o.good() )
      (*m.f_)( o, m.a1_, m.a2_, m.a3_, m.a4_ );
    return o;
  }

private:
  func_type const f_;
  Arg1Type const a1_;
  Arg2Type const a2_;
  Arg3Type const a3_;
  Arg4Type const a4_;
};

/**
 * Defines an ostream manipulator "thunk" function that calls an existing
 * non-manipulator function having the same name.
 *
 * @param FN_NAME The name of the existing function.
 * @param ARG1_T  The type of the first non-ostream argument.
 * @param ARG2_T  The type of the second non-ostream argument.
 * @param ARG3_T  The type of the third non-ostream argument.
 * @param ARG4_T  The type of the fourth non-ostream argument.
 */
#define DEF_OMANIP4(FN_NAME,ARG1_T,ARG2_T,ARG3_T,ARG4_T)  \
  inline PJL::omanip4<ARG1_T,ARG2_T,ARG3_T,ARG4_T>        \
  FN_NAME( ARG1_T a1, ARG2_T a2, ARG3_T a3, ARG4_T a4 ) { \
    return PJL::omanip4<ARG1_T,ARG2_T,ARG3_T,ARG4_T>( FN_NAME, a1, a2, a3, a4 ); \
  }

///////////////////////////////////////////////////////////////////////////////

} // namespace PJL

#endif  /* OMANIP_H */
/* vim:set et sw=2 ts=2: */
