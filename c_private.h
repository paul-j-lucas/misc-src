#ifndef C_PRIVATE_H
#define C_PRIVATE_H

/**
 * @file
 * These macros can be used to have "private" (sort of) struct members in C.
 * For example:
 *
 *    struct my_struct {
 *      // "public" members go here
 *      // ...
 *      int shout;
 *    C_PRIVATE(my_struct)
 *      // "private" members go here
 *      // ...
 *      int shhh;
 *    C_PRIVATE_END
 *    };
 *
 * Then, to access a "private" member:
 *
 *    struct my_struct s;
 *    // ...
 *    C_PRIV(s).shhh = 0;
 */

#define C_PRIVATE(STRUCT) struct STRUCT##c_private {
#define C_PRIVATE_END     } c_private;
#define C_PRIV(OBJ)       ((OBJ).c_private)

#endif /* C_PRIVATE_H */
/* vim:set et sw=2 ts=2: */
