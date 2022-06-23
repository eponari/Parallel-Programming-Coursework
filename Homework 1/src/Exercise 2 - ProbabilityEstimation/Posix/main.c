//include all needed libraries.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "stringMethods.h"

//function declarations.
void* probabilityCalculation();
void getArgs();

//global variables.
char* pattern1;
char* pattern2;
long maxLengthPattern;

//default values.
long pattern1Occurences = 0;
long pattern2Occurences = 0;
long threadCount = 10;
long tries = 100000;

//mutex to syncronize region and keep data integrity.
pthread_mutex_t mutex;

int main(int argc,char* argv[]) {
	//get argument from argv.
  getArgs(argc,argv,pattern1,pattern1,&threadCount,&tries,&maxLengthPattern);

	//create array of threads and initialized mutex.
  pthread_t* thread_handles;
  thread_handles = (pthread_t*) malloc(sizeof(pthread_t)*threadCount);
  pthread_mutex_init(&mutex, NULL);

	//create each thread.
  for(long i=0;i<threadCount;i++){
    pthread_create(&thread_handles[i],NULL,probabilityCalculation,(void*)i);
  }
  
  //join each thread.
  for(long i=0;i<threadCount;i++){
  	pthread_join(thread_handles[i],NULL);
  }

	//print results.
  printf("Estimated values with %ld random experiements with %ld thread\n",tries,threadCount);
  printf("The probability that %s appears first is %lf\n",pattern1,(double)pattern1Occurences/tries);
  printf("The probability that %s appears first is %lf\n",pattern2,(double)pattern2Occurences/tries);

  return 0;
}

void* probabilityCalculation(void* rank){
  long myRank = (long) rank;

	//local variables.
  long localpattern1Occurences = 0;
  long localpattern2Occurences = 0;

	//generated text with H and T.
  char generatePattern[maxLengthPattern];
  int index=0;

  for(int i = myRank ; i<tries;){
	  //generate random character.
    char randomCharacter;

    if(rand()%2==1){
      randomCharacter = 'H';
    }else{
      randomCharacter = 'T';
    }

	//add character to index.
    generatePattern[index] = randomCharacter;

	//keep track that index doesn't go over the maximum length.
    if(index<maxLengthPattern-1){
      index++;
    }
    
	//endsWith is my own defined function. check if pattern is generated in each case.
    if(endsWith(generatePattern,pattern1)){
      localpattern1Occurences++;
      cleanStr(generatePattern,maxLengthPattern);
	  //keep workload balance.
      i+=threadCount;
    }
    else if(endsWith(generatePattern,pattern2)){
      localpattern2Occurences++;
      cleanStr(generatePattern,maxLengthPattern);
      //keep workload balance.
	  i+=threadCount;
    }

	//shiftLeft is my own defined function. shifts the pattern by 1 when the pattern generated gets longer than the actual maximum length.
    if(index==maxLengthPattern-1){
      shiftLeft(generatePattern,maxLengthPattern);
    }
  }
  
  //acquire the mutex to block other threads.
  pthread_mutex_lock(&mutex);
  //add local results to global results.
  pattern1Occurences += localpattern1Occurences;
  pattern2Occurences += localpattern2Occurences;
  //release mutex to allow other threads.
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void getArgs(int argc,char* argv[]){
  if(argc>4){
    pattern1 = argv[1];
    pattern2 = argv[2];
    tries = strtol(argv[3],NULL,10);
    threadCount = strtol(argv[4],NULL,10);
  }
  else if(argc>3){
    pattern1 = argv[1];
    pattern2 = argv[2];
    tries = strtol(argv[3],NULL,10);
  }
  else if(argc>2){
    pattern1 = argv[1];
    pattern2 = argv[2];
  }
  else{
    printf("You should provide: <pattern1> <pattern2> <nr of threads>(optional) <number of trials>(optional)");
    exit(0);
  }

  long length1 = strlen(pattern1);
  long length2 = strlen(pattern2);

  if(length1>length2){
    maxLengthPattern = length1;
  }
  else{
    maxLengthPattern = length2;
  }
}