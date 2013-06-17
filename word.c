#include <stdio.h>
#include <stdlib.h>
#include "word.h"

/*cria e devolve um novo item*/
Item newitem(string text, string l, Item n, sentence* s, int N, int mod){
  Item word = malloc(sizeof(*word));
  int tam = 0, i = 0;
  
  while( text[i++] != '\0' )
    tam++;

  word->literal = mallocSafe(tam+2);
  word->next = n;
  word->occurencies = s;
  word->N = N;
  word->lema = mallocSafe(tam+2);

  i = 0;

  while( text[i] != '\0' ){
    word->literal[i] = text[i];
    i++;
  }
  word->literal[tam] = '\0';
  
  i = 0;
  
  while( l[i] != '\0'){
    word->lema[i] = l[i];
    i++;
  }
  word->lema[i] = '\0';

  if(mod == WORD) word->chave = word->literal;
  if(mod == LEMA) word->chave = word->lema;
  
  return word;
}

/*funcao malloc que interrompe o programa ao acabar a memoria. Retirada da pagina do professor Feofiloff*/
void *mallocSafe (size_t nbytes){
  
  /*aloca memoria para um ponteiro e avisa se o ponteiro devolvido for NULL*/

  void *ptr;
  ptr = malloc (nbytes);
  if (ptr == NULL) {
    fprintf (stderr, "Socorro! malloc devolveu NULL!\n");
    exit (-1);
  }
  return ptr;
}

/*caso uma string buffer fique cheia, aloca-se mais epaco para ela*/
string fixstring(string a, int newsize){
  string temp = (char*)mallocSafe(newsize);
  strcpy(temp, a);
  /*free(a);*/
  return temp;
}
