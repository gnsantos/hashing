/*********************************************************************************************************
 * MAC323 - ESTRUTURAS DE DADOS                                                                          *
 * PRIMEIRO SEMESTRE DE 2013                                                                             *
 * PROFESSOR YOSHIHARU KOHAYAKAWA                                                                        *
 * EXERCICIO PROGRAMA 4 - LOCALIZACAO DE PALAVRAS II                                                     *
 * DATA DE ENTREGA: 17/06/2013                                                                           *
 * ALUNO: GERVASIO PROTASIO DOS SANTOS NETO                                                              *
 * NUSP: 7990996                                                                                         *
 * ep4.c                                                                                                 *
 ********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getline.h"
#include "word.h"
#include "Item.h"
#include "ST1.h"
#include "ST2.h"

#define INITIAL_SIZE 160000

int palavrasDistintas = 0, palavrasTotais = 0, tokensDistintos = 0, lemas = 0;

void printseg(char*,int,int);

int main(int argc, char* argv[]){
  FILE* entrada;
  char nome[64], text[200], lema[200]; /*strings auxiliares*/
  char sentenceIdentifier[50], sentenceAnalysis[500];/*strings que aramazenam o identificador de cada sentenca e sua analise, respectivamente*/
  int numSentence = 0, numTokens = 0, totalTokens = 0;
  int i = 0, j = 0, n = 0;
  int comecoFrase = 0, fimFrase = 0, comecoAnalise = 0, fimAnalise = 0, arraysize = 3000, buffersize;
  char *buffer;/*armazena a entrada*/
  analysis *analysisArray;/*aramazena as posicoes de comeco e fim das analises de cada frase*/
  char comando[5]; /*comando (i.e e, a, av etc)*/
  char argumento[100]; /*argumento do comando acima*/
 
  /*abre arquivo de entrada*/
  if(argc < 2){
    puts("Passe o arquivo como argumento.");
    exit(-1);
  }
  sscanf(argv[1], "-f%s", nome);
  entrada = fopen(nome, "r");
  if(!entrada){
    puts("Erro ao ler o arquivo de entrada.");
    exit(-2);
  }
  
  buffer = getinput(entrada);
  buffersize = strlen(buffer);
  
  fclose(entrada); /*Fecha arquivo de entrada. Ele esta presente no buffer.*/
  
  analysisArray = mallocSafe(arraysize*sizeof(*analysisArray));/*inicializa array*/

  /*inicializa tabelas de hash*/
  ST1init(INITIAL_SIZE);
  ST2init(INITIAL_SIZE);

  /*Percorre ate o fim do buffer*/
  while( buffer[n] != '\0'){
    i = 0;
    
    for(j = 0; j < 50; j++)
      sentenceIdentifier[j] = '\0';

    while( buffer[n] != '\n'){
      sentenceIdentifier[i++] = buffer[n++];
      if( i >= 50 ) break;
    }
    i = 0;
   
    /*consegue o numero da sentenca e o numero de tokens nesta sentanca*/
    sscanf(sentenceIdentifier, "Sentence #%d (%d tokens):", &numSentence, &numTokens);
    totalTokens += numTokens;
    comecoFrase = n++;
    /*le a sentenca enquanto a analise nao se inicia*/
    while(1){
      if(buffer[n] != '['){
	n++;
	continue;
      } 
      else if( buffer[n+1] != 'T'){
	n++;
	continue;
      }
      else if( buffer[n+2] != 'e'){
	n++;
	continue;
      }
      else if( buffer[n+3] != 'x'){
	n++;
	continue;
      }
      else if( buffer[n+4] != 't'){
	n++;
	continue;
      }
      else break;
    }
    fimFrase = n-1;

    comecoAnalise = n;
    
    /*le ate o fim da analise*/
    while( buffer[n] != '\n' ){
      Item w, l;
      sentence* ocorrenciaL = mallocSafe(sizeof(*ocorrenciaL));
      sentence* ocorrenciaW = mallocSafe(sizeof(*ocorrenciaW));

      ocorrenciaL->next = NULL;
      ocorrenciaL->ini = comecoFrase;
      ocorrenciaL->fim = fimFrase;
      ocorrenciaL->numSentence = numSentence;
      ocorrenciaL->numTokens = numTokens;

      ocorrenciaW->next = NULL;
      ocorrenciaW->ini = comecoFrase;
      ocorrenciaW->fim = fimFrase;
      ocorrenciaW->numSentence = numSentence;
      ocorrenciaW->numTokens = numTokens;
            
      /*limpa buffer*/
      for(j = 0; j < 500; j++)
	sentenceAnalysis[j] = '\0';
      i = 0;

      while( buffer[n] != ']'){
	if( buffer[n] == '\0' ){
	  break;
	}
	sentenceAnalysis[i] = buffer[n];
	n++; i++;
      }/*fecha o while que percorre entre os cochetes*/
      n++;
      i = 0;

      if(buffer[n] == '\0' || buffer[n] == '\n'){
	break;
      }

      /*limpa buffers*/
      for(j = 0; j < 200; j++){
	text[j] = '\0';
	lema[j] = '\0';
      }

      /*adquire a palavra exata e o lema desta palavra*/
      sscanf(sentenceAnalysis, "[Text=%s Lemma=%s %*s", text, lema);

      w = newitem(text, lema, NULL, ocorrenciaW, 1, WORD);
      l = newitem(text, lema, NULL, ocorrenciaL, 1, LEMA);

      ST1insert(w);
      ST2insert(l);

      n++;
    }/*fecha o while que percorre a analise da frase toda*/
    
    fimAnalise = n-1;
    analysisArray[numSentence].ini = comecoAnalise;
    analysisArray[numSentence].fim = fimAnalise;
    
    if(numSentence == arraysize - 10){
      analysis *help = mallocSafe(2*arraysize*sizeof(*help));
      for(j = 1; j <= numSentence; j++)
	help[j] = analysisArray[j];
      arraysize *= 2;
      free(analysisArray);
      analysisArray = help;
    }
    
    if( buffer[n++] == '\0' ) break;  

  }/*fecha o while que percorre o buffer*/
  puts("Analise encerrada");
  puts("Insira seus comandos:");
  
  /*recebe comandos enquanto nao e recebido o comando -F*/
  while( fgets(argumento, 100, stdin) != NULL){
    Item w, p, l;
    sentence* s;
    
    /*adquire o comando e seu argumento*/
    sscanf(argumento, "-%s %s", comando, argumento);
   
    /*interrompe o loop e finaliza o programa*/
    if( strcmp(comando, "F" ) == 0 )
      break;
    
    else if(strcmp(comando,"e") == 0){
      w = ST1search(argumento);
      /*se a palavra nao aparecer no texto, procura-se o proximo comando*/
      if( w == NULLitem ){
	puts("A palavra procurada nao foi encontrada.");
	continue;
      }
      for(p = w; p != NULLitem; p = ST1next(p)){
	if( eq(p->literal, argumento) ){
	  for(s = p->occurencies; s != NULL; s = s->next)
	    printseg(buffer, s->ini, s->fim);
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -e*/

    
    else if(strcmp(comando,"ev") == 0){
      w = ST1search(argumento);
      /*se a palavra nao aparecer no texto, procura-se o proximo comando*/
      if( w == NULLitem ){
	puts("A palavra procurada nao foi encontrada.");
	continue;
      }
      for(p = w; p != NULLitem; p = ST1next(p)){
	if( eq(p->literal, argumento) ){
	  for(s = p->occurencies; s!= NULL; s = s->next){
	    printf("Sentence #%d (%d tokens):\n", s->numSentence, s->numTokens);
	    printseg(buffer, s->ini, s->fim);
	  }
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -ev*/

    else if(strcmp(comando,"eV") == 0){
      w = ST1search(argumento);
      /*se a palavra nao aparecer no texto, procura-se o proximo comando*/
      if( w == NULLitem ){
	puts("A palavra procurada nao foi encontrada.");
	continue;
      }
      for(p = w; p != NULLitem; p = ST1next(p)){
	if( eq(p->literal, argumento) ){
	  for(s = p->occurencies; s != NULL; s = s->next){
	    printf("Sentence #%d (%d tokens):\n", s->numSentence, s->numTokens);
	    printseg(buffer, s->ini, s->fim);
	    printseg(buffer,analysisArray[s->numSentence].ini,analysisArray[s->numSentence].fim);
	  }
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -eV*/

    else if(eq(comando, "a")){
      w = ST1search(argumento);
      if( w == NULLitem){
	puts("A palavra nao foi encontrada");
	continue;
      }
      l = ST2search(w->lema);
      for(p = l; p != NULLitem; p = ST2next(p)){
	if(eq(p->lema, l->lema)){
	  for(s = p->occurencies; s != NULL; s = s->next)
	    printseg(buffer, s->ini, s->fim);
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -a*/

    else if(eq(comando, "av")){
      w = ST1search(argumento);
      if( w == NULLitem){
	puts("A palavra nao foi encontrada");
	continue;
      }
      l = ST2search(w->lema);
      for(p = l; p != NULLitem; p = ST2next(p)){
	if(eq(p->lema, l->lema)){
	  for(s = p->occurencies; s != NULL; s = s->next){
	    printf("Sentence #%d (%d tokens):\n", s->numSentence, s->numTokens);
	    printseg(buffer, s->ini, s->fim);
	  }
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -av*/

    else if(eq(comando, "aV")){
      w = ST1search(argumento);
      if( w == NULLitem){
	puts("A palavra nao foi encontrada");
	continue;
      }
      l = ST2search(w->lema);
      for(p = l; p != NULLitem; p = ST2next(p)){
	if(eq(p->lema, l->lema)){
	  for(s = p->occurencies; s != NULL; s = s->next){
	    printf("Sentence #%d (%d tokens):\n", s->numSentence, s->numTokens);
	    printseg(buffer, s->ini, s->fim);
	    printseg(buffer,analysisArray[s->numSentence].ini,analysisArray[s->numSentence].fim);
	  }
	}/*fecha if*/
      }/*fecha for*/
    }/*fecha comando -aV*/

    else if(eq(comando, "t"))
      ST1sort(ALL);

    else if(eq(comando, "d"))
      ST1sort(WORDS);

    else if(eq(comando, "l"))
      ST2sort(WORDS);

    else if((eq(comando, "L")))
      ST2show();

    else if(eq(comando, "s")){
      ST1countdiffer(&palavrasTotais, &palavrasDistintas, &tokensDistintos);
      ST2countdiffer(&lemas);
      printf("Estatisitcas sobre %s:\n", nome);
      printf("- %d sentencas no total.\n"
	     "- %d tokens no total.\n"
	     "- %d palavras no total.\n"
	     "- %d tokens distintos.\n"
	     "- %d palavras distintas.\n"
	     "- %d lemas distintos.\n"
	     ,numSentence,totalTokens, palavrasTotais, tokensDistintos, palavrasDistintas,lemas);
      palavrasTotais = 0;
      palavrasDistintas = 0;
      tokensDistintos = 0;
      lemas = 0;
    }/*fecha a opcao -s*/

  }/*fecha o while que roda enquanto houver comandos.*/
  
  free(buffer);
  free(analysisArray);
  return 0;
}/*fecha o programa*/


void printseg(char* b, int ini, int fim){
  int i;
  for(i = ini; i != fim; i++)
    printf("%c", b[i]);
  puts("\n");
}
