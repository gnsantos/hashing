#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "word.h"
#include "Item.h"

/*devolve a key do item*/
string key(Item item){
  if(item == NULL) return NULL;
  return item->chave;
}

/*checa se duas keys sao iguais*/
int eq(Key a, Key b){
  return(strcmp(a,b) == 0);
}

/*verifica se a key a e menor que a key b*/
int less(Key a, Key b){
  return(strcmp(a,b) < 0);
}

/*imprime um item*/
void ITEMshow(Item i){
  printf("%s\n", key(i));
}
