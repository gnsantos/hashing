#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Item.h"
#define z NULLitem
#define o ++n_probes
#define null(A) (st[A] == NULLitem)
#define onull(A) (o, st[A] == NULLitem)
static int N, M;
static long n_probes=0, n_probes_hit=0, n_probes_miss=0;
static Item *st;
void mergeSort(Item *v, int p, int r);
int p[] = {
  251,
  509,
  1021,
  2039,
  4093,
  8191,
  16381,
  32749,
  65521,
  131071,
  262139,
  524287,
  1048573,
  2097143,
  4194301,
  8388593,
  16777213,
  33554393,
  67108859,
  134217689,
  268435399,
  536870909,
  1073741789,
  2147483647
};

int prime_m(int m)
{
  int x;
  x = log(m)/log(2.0) + 1;
  if (x < 8)
    return p[0];
  else 
    return p[x - 8];
}

int hash(char *v, int M)
{ int h = 0, a = 127; /* a = 128; */
  for (; *v != '\0'; v++) 
    h = (a*h + *v) % M;
  return h;
}

void ST1init(int max)
{ int i;
  N = 0; M = prime_m(2*max);
  st = malloc(M*sizeof(Item));
  for (i = 0; i < M; i++) st[i] = NULLitem;
}
int ST1count() { return N; }

void ST1insert(Item item)
{ Key v = key(item);
  int i = hash(v, M), j = 0;
  while (!null(i)){
    if( eq(st[i]->literal, item->literal) ){
      item->occurencies->next = st[i]->occurencies;
      st[i]->occurencies = item->occurencies;
      return;
    }
    i = (i+1) % M;
  }
  st[i] = item;
  N++;

  if(N >= M/2){ /*resize dinamico*/
    Item *aux;
    int oldsize = M;
    M = prime_m(2*M);
    aux = mallocSafe(M*sizeof(Item));
    for(i = 0; i < M; i++) aux[i] = NULLitem;
    for(i = 0; i < oldsize; i++){
      if(st[i] != NULLitem){
	Key v = key(st[i]);
	j = hash(v, M);
	while( aux[j] != NULLitem ) j = (j+1)%M;
	aux[j] = st[i];
      }
    }
    free(st);
    st = aux;
  }
}
Item ST1search(Key v)
{ int i = hash(v, M);
  int t = n_probes;
  while (!onull(i))
    if (eq(v, key(st[i]))) {
      n_probes_hit+=n_probes-t;
      return st[i];
    } else i = (i+1) % M;
  n_probes_miss+=n_probes-t;
  return NULLitem;
}

void ST1delete(Item item)
{ int j, i = hash(key(item), M); Item v;
  while (!null(i))
    if (eq(key(item), key(st[i]))) break; 
    else i = (i+1) % M;
  if (null(i)) return;
  st[i] = NULLitem; N--;
  for (j = i+1; !null(j); j = (j+1) % M, N--)
    { v = st[j]; st[j] = NULLitem; ST1insert(v); }
}

Item ST1next(Item item){
  Key v = key(item);
  int i = hash(v, M);
  for(; st[i] != item; i++);
  return st[i+1];
}

void
intercala( int p, int q, int r, Item* v)
{
  int i, j, k;
  Item *w;
  w = mallocSafe( (r-p) * sizeof (*w));
  for (i = 0, k = p; k < q; ++i, ++k)
    w[i] = v[k];
  for (j = r-p-1, k = q; k < r; --j, ++k)
    w[j] = v[k];
  i = 0; j = r-p-1;
  for (k = p; k < r; ++k)
    if (w[i] != NULLitem && w[j] != NULLitem){
      if ((eq(w[i]->lema, w[j]->lema) || less(w[i]->lema, w[j]->lema))) v[k] = w[i++];
      else v[k] = w[j--];
    }
  free( w);
}

void
mergeSort(Item *v, int p, int r)
{
  if (p < r-1) {
    int q = (p + r)/2;
    mergeSort(v, p, q);
    mergeSort(v, q, r);
    intercala( p, q, r, v);
  }
}


void ST1countdiffer(int *palavrasTotais, int* palavrasDistintas, int *tokensDistintos)
{
  int i = 0, j = 0, pD = 0, pT = 0 ;
  Item *ordem = mallocSafe(N*sizeof(*ordem));
  Key p;
  sentence *s;
  for(i = 0; i < M; i++)
    if(st[i] != NULLitem){
      ordem[j] = mallocSafe(sizeof(*(ordem[j])));
      ordem[j] = st[i];
      if( isLetter(ordem[j]->literal[0] ))
	for(s = ordem[j]->occurencies; s != NULL; s = s->next)pT++;
      j++;
    }

  mergeSort(ordem, 0, N);
  
  p = ordem[0]->literal;
  if( isLetter(p[0]) )
    pD++;
  for(j = 1; j < N; j++){
    if(!eq(p, ordem[j]->lema)){
      p = ordem[j]->lema;
      if(isLetter(p[0]))
	for(s = ordem[j]->occurencies; s != NULL; s = s->next) pD++;
    }
  }
  *palavrasDistintas = pD;
  *palavrasTotais = pT;
  *tokensDistintos = ST1count()+1;
  free(ordem);
}

void ST1sort(int mod){
  Item* ordem = mallocSafe(N*sizeof(*ordem));
  Key p;
  int i = 0, j = 0;
  for(i = 0; i < M; i++)
    if(st[i] != NULLitem){
      ordem[j] = mallocSafe(sizeof(*(ordem[j])));
      ordem[j++] = st[i];
    }

  mergeSort(ordem, 0, N);

 if(mod == ALL){
    p = ordem[0]->literal;
    puts(p);
    for(j = 1; j < N; j++){
      if(!eq(p, ordem[j]->literal)){
	p = ordem[j]->literal;
	puts(p);
      }
    }
  }

  if(mod == WORDS){
    p = ordem[0]->literal;
    if( isLetter(p[0]) )
      puts(p);
    for(j = 1; j < N; j++){
      if(!eq(p, ordem[j]->literal)){
	p = ordem[j]->literal;
	if(isLetter(p[0]))
	  puts(p);
      }
    }
  }

  free(ordem);
}
