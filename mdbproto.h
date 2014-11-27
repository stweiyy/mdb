#ifndef _MDBPROTO_H_
#define _MDBPROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mdbtype.h"
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

void mdbBeginParse(Parse *,int);
void mdbStartTable(Parse *,Token *);
void mdbAddColumn(Parse *,Token *);
void mdbAddColumnType(Parse *,Token *);
void mdbEndTable(Parse *);


#ifdef __cplusplus
}
#endif


#endif
