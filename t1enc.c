#include "Item.h"
#include "ST1.h"
#include "word.h"
#include <math.h>

typedef struct ST1node *link;
struct ST1node { Item item; link next; };

static link *heads, z;
static int N, M;

void mergeSort(link*, int, int);

int p4[] = {
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

int prime_m4(int m)
{
  int x;
  x = log(m)/log(2.0) + 1;
  if (x < 8)
    return p4[0];
  else 
    return p4[x - 8];
}


static link NEW(Item item, link next)
{
  link x = malloc(sizeof *x);
  x->item = item;
  x->next = next;
  return x;
}

int ST1count()
{
  return N;
}

/* prog14.1 */

int hash(char *v, int M)
{ int h = 0, a = 127; /* a = 128; */
  for (; *v != '\0'; v++) 
    h = (a*h + *v) % M;
  return h;
}

#define hash(v,M) (hash(v,M)) 

void ST1init(int max) 
{ int i;
  N = 0;
  M = 65521; 
  heads = malloc(M*sizeof(link));
  z = NEW(NULLitem, NULL);
  for (i = 0; i < M; i++) heads[i] = z; 
}

Item searchR(link t, Key v)
{ 
  if (t == z) return NULLitem;
  if (eq(key(t->item), v)) return t->item;
  return searchR(t->next, v);
}

Item ST1search(Key v)
{ return searchR(heads[hash(v, M)], v); }

void ST1insert(Item item)
{ int i = hash(key(item), M), j = 0;
  heads[i] = NEW(item, heads[i]); N++;
  
  if(N >= 3*M/4){
    link *aux, t;
    int oldsize = M;
    M = prime_m4(2*M);
    aux = mallocSafe(M*sizeof(*aux));
    for(i = 0; i < M; i++)
      aux[i] = z;
    for(i = 0; i < oldsize; i++){
      if(heads[i] != z){
	for(t = heads[i]; t != z; t = t->next){
	  j = hash(key(t->item) , M);
	  aux[j] = NEW(item, aux[j]);
	}
      }
    }
    free(heads);
    heads = aux;
  }
}

void ST1sort(int mod)
{
  int i, j = 0;
  link t;
  link *ordem = mallocSafe(N*sizeof(link));
  Key p;
  for (i=0; i<M; i++) {
    if (heads[i] != z)
      for (t = heads[i]; t != z; t = t->next){
	ordem[j] = mallocSafe(sizeof(*(ordem[j])));
	ordem[j++]->item = t->item;
      }
  }
  
  mergeSort(ordem, 0, N);
  
  if(mod == ALL){
    p = ordem[0]->item->literal;
    puts(p);
    for(j = 1; j < N; j++){
      if(!eq(p, ordem[j]->item->literal)){
	p = ordem[j]->item->literal;
	puts(p);
      }
    }
  }

  if(mod == WORDS){
    p = ordem[0]->item->literal;
    if( isLetter(p[0]) )
      puts(p);
    for(j = 1; j < N; j++){
      if(!eq(p, ordem[j]->item->literal)){
	p = ordem[j]->item->literal;
	if(isLetter(p[0]))
	  puts(p);
      }
    }
  }

  free(ordem);
}

void ST1delete(Item item){
  return ;
}

Item ST1next(Item item){
  Key v = key(item);
  int i = hash(v, M);
  link w;
  if(heads[i]->item == item) return heads[i]->next->item;
  for(w = heads[i]->next; w->item != item; w = w->next);
  return w->next->item;
}

void ST1countdiffer(int *palavrasTotais, int *palavrasDistintas, int *tokensDistintos){
  int i, j = 0, pT = 0, pD = 0, tT = 0;
  link t;
  link *ordem = mallocSafe(N*sizeof(link));
  Key p;
  for (i=0; i<M; i++) {
    if (heads[i] != z)
      for (t = heads[i]; t != z; t = t->next){
	ordem[j] = mallocSafe(sizeof(*(ordem[j])));
	ordem[j]->item = t->item;
	if(isLetter(ordem[j]->item->literal[0]))
	  pT++;
	j++;
      }
  }
  
  mergeSort(ordem, 0, N);

  p = ordem[0]->item->literal;
  tT++;
  if( isLetter(p[0]) )
    pD++;
  for(j = 1; j < N; j++){
    if(!eq(p, ordem[j]->item->literal)){
      tT++;
      p = ordem[j]->item->literal;
      if(isLetter(p[0])){
	pD++;
      }
    }
  }
  
  *palavrasDistintas = pD;
  *palavrasTotais = pT;
  *tokensDistintos = tT;

  free(ordem);
}

void
intercala( int p, int q, int r, link* v)
{
  int i, j, k;
  link *w;
   w = mallocSafe( (r-p) * sizeof (*w));
   for (i = 0, k = p; k < q; ++i, ++k) w[i] = v[k];
   for (j = r-p-1, k = q; k < r; --j, ++k) w[j] = v[k];
   i = 0; j = r-p-1;
   for (k = p; k < r; ++k)
     if ( eq(w[i]->item->lema, w[j]->item->lema) || less(w[i]->item->lema, w[j]->item->lema)) v[k] = w[i++];
      else v[k] = w[j--];
   free( w);
}

void
mergeSort(link *v, int p, int r)
{
   if (p < r-1) {
      int q = (p + r)/2;
      mergeSort(v, p, q);
      mergeSort(v, q, r);
      intercala( p, q, r, v);
   }
}
