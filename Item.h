#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "word.h"

#define NULLitem NULL

Key key(Item);
int eq(Key, Key);
int less(Key, Key);
void ITEMshow(Item);
void freeItem(Item);

#endif
