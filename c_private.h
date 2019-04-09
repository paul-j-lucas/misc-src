#ifndef C_PRIVATE_H
#define C_PRIVATE_H

///////////////////////////////////////////////////////////////////////////////

/**
 * @file
 * These macros can be used to have "private" (sort of) `struct` members in C.
 * For example:
 *
 *    struct my_struct {
 *      // "public" members go here
 *      int shout;
 *    C_PRIVATE_BEGIN
 *      // "private" members go here
 *      int shhh;
 *    C_PRIVATE_END
 *    };
 *
 * Then, to access a "private" member:
 *
 *    struct my_struct s;
 *    // ...
 *    C_PRIVATE(s).shhh = 0;
 *
 * @note There can be at most one "private" section per `struct`.
 */

/**
 * Begins a "private" section of a `struct`.
 * @note There can be at most one "private" section per `struct`.
 */
#define C_PRIVATE_BEGIN struct {

/**
 * Ends a "private" section of a `struct`.
 */
#define C_PRIVATE_END   } c_private;

/**
 * Accesses a "private" member of a `struct.
 *
 * @param OBJ The `struct` object to access a "private" member of.
 */
#define C_PRIVATE(OBJ)  ((OBJ).c_private)

///////////////////////////////////////////////////////////////////////////////
#endif /* C_PRIVATE_H */
/* vim:set et sw=2 ts=2: */
