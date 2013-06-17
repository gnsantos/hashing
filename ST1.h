#ifndef ST1_H_INCLUDED
#define SR1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void ST1init();
int ST1count();
void ST1insert(Item);
Item ST1search(Key);
void ST1sort(int);
void ST1delete(Item);
Item ST1next(Item);
void ST1countdiffer(int*,int*,int*);

#endif
