#ifndef WORD_H_INCLUDED
#define WORD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*definicoes de tipo*/
#define LEMA 2
#define WORD 1
#define WORDS 3
#define ALL 4
#define isLetter(x) ((x>='a' && x <= 'z') || (x>='A' && x<'Z')) 

typedef struct sentence sentence;
typedef struct analysis analysis;
typedef char* string;
typedef struct word* Item;
typedef char* Key;

struct sentence{
  int numSentence; /*numero da frase segundo o CoreNLP*/
  int numTokens; /*numero de tokens nessa frase*/
  int ini;
  int fim;
  sentence* next; /*proximo elemento em uma possivel lista ligada*/
};

struct analysis{
  int ini;
  int fim;
};

struct word{
  string literal; /*texto em si*/
  string lema; /*lema da palavra*/
  Item next; /*proximo elemento em uma lista ligada*/
  sentence* occurencies; /*lista ligada de ocorrencias da palavra*/
  int N; /*numero de vezes que a palavra aparece*/
  Key chave; /*chave do item*/
};

Item newitem(string, string, Item, sentence*, int, int);
void* mallocSafe(size_t);
string fixstring(string ,int);

#endif
