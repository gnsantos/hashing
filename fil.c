#include <stdio.h>
#include <stdlib.h>

#define advanceif(A) \
      c = getchar(); putchar(c); \
      if (c == (A)) state++; \
      else state = 0; \
      break; \

#define advanceif2(A) \
      c = getchar(); \
      if (c == (A)) state++; \
      else state = 7; \
      break; \

#define MLEN 50

char buff[MLEN];
char *L = " Lemma=";

int main()
{ int i;
  int state = 0;
  int c;

  for (;;)
    switch (state) {
    case 0:
      if ((c = getchar()) == EOF) exit(0);
      putchar(c);
      if (c == '[') state++;
      break;
    case 1: advanceif('T'); 
    case 2: advanceif('e'); 
    case 3: advanceif('x'); 
    case 4: advanceif('t'); 
    case 5: advanceif('='); 
    case 6:
      while ((c = getchar()) != ' ')
	putchar(c);
      state = 7;
      break;
    case 7: 
      c = getchar();
      if (c == ' ') state = 8; 
      break; 
    case 8: advanceif2('L');
    case 9: advanceif2('e');
    case 10: advanceif2('m');
    case 11: advanceif2('m');
    case 12: advanceif2('a');
    case 13: advanceif2('=');
    case 14:
      i = 0;
      while ((c = getchar()) != ']')
	buff[i++] = (char)c;
      buff[i]='\0';
      printf("%s%s] ", L, buff);
      getchar(); /* space after ']' */
      state = 0;
      break;
    default: printf("Shouldn't happen.\n"); exit(1);
    }

 return 0;
}
