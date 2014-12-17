#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

#define UNIQUE_ID_HELPER2(ID,LINE) ID ## _ ## LINE
#define UNIQUE_ID_HELPER1(ID,LINE) UNIQUE_ID_HELPER2( ID, LINE )
#define UNIQUE_ID(ID) UNIQUE_ID_HELPER1( ID, __LINE__ )

#endif /* UNIQUE_ID_H */
/* vim:set et sw=2 ts=2: */
