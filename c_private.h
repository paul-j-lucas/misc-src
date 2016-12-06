#ifndef C_PRIVATE_H
#define C_PRIVATE_H

/**
 * @file
 * These macros can be used to have "private" (sort of) struct members in C.
 * For example:
 *
 *    struct my_struct {
 *      // "public" members go here
 *      int shout;
 *    C_PRIVATE_BEGIN(my_struct)
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
 */

#define C_PRIVATE_BEGIN(STRUCT) struct STRUCT##_c_private {
#define C_PRIVATE_END           } c_private;
#define C_PRIVATE(OBJ)          ((OBJ).c_private)

#endif /* C_PRIVATE_H */
/* vim:set et sw=2 ts=2: */
