#ifndef ST2_H_INCLUDED
#define ST2_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

void ST2init();
int ST2count();
void ST2insert(Item);
Item ST2search(Key);
void ST2sort(int);
void ST2show();
void ST2delete(Item);
Item ST2next(Item);
void ST2countdiffer(int*);

#endif
