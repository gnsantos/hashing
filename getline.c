#include "getline.h"
#include "word.h"

char* getinput(FILE* entrada){
  char *line, *nline;
  int n, ch, size;
  n = 0;
  size = INITIAL_BUFFER_SIZE;
  line = (char *) mallocSafe(size+1);
  
  while( (ch = fgetc(entrada)) != EOF){
    if(n == size){
      size *= 2;
      nline = (char *) mallocSafe(size +1);
      strcpy(nline, line);
      free(line);
      line = nline;
    }
    line[n++] = ch;
  }
  if(n == 0 && ch == EOF){
    free(line);
    return NULL;
  }
  line[n] = '\0';
  nline = (char *) mallocSafe(n+1);
  strcpy(nline, line);
  free(line);
  return nline;
}
