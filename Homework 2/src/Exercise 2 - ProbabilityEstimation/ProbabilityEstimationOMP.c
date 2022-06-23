#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "stringMethods.h"

void getArgs();

int main(int argc,char* argv[]){
	char* pattern1 = NULL;
	char* pattern2 = NULL;
	int maxLengthPattern;
	
	int threadCount = 10;
	int tries = 100000;
	
	getArgs(argc,argv,&pattern1,&pattern2,&threadCount,&tries,&maxLengthPattern);

	int pattern1Occurences = 0;
	int pattern2Occurences = 0;
	
	//printf("%ld",maxLengthPattern);
	
	
    
	#pragma omp parallel for num_threads(threadCount) reduction(+: pattern1Occurences,pattern2Occurences)
	for (int i = 0; i < tries; i++ )
	{
		int index=0;
		char generatedPattern[maxLengthPattern];
        	while(true){
        	
        		char randomCharacter;

    			if(rand()%2==1){
      				randomCharacter = 'H';
    			}else{
      				randomCharacter = 'T';
    			}

			//add character to index.
    			generatedPattern[index] = randomCharacter;

			//keep track that index doesn't go over the maximum length.
    			if(index<maxLengthPattern-1){
      				index++;
    			}
    
			//endsWith is my own defined function. check if pattern is generated in each case.
    			if(endsWith(generatedPattern,pattern1)){
      				pattern1Occurences++;
      				cleanStr(generatedPattern,maxLengthPattern);
      				break;
    			}
    			else if(endsWith(generatedPattern,pattern2)){
    				cleanStr(generatedPattern,maxLengthPattern);
      				pattern2Occurences++;
      				break;
    			}
			
			
			
			//shiftLeft is my own defined function. shifts the pattern by 1 when the pattern generated gets longer than the actual maximum length.
    			if(index==maxLengthPattern-1){
      				shiftLeft(generatedPattern,maxLengthPattern);
    			}
        	}
    	}
    
    	printf("The probability of %s to appear first is: %f\n",pattern1,(float)pattern1Occurences/tries);
    	printf("The probability of %s to appear first is: %f\n",pattern2,(float)pattern2Occurences/tries);
    	

    	return 0;
}

void getArgs(int argc,char* argv[],char** pattern1,char** pattern2,int* threadCount,int* tries,int* maxLengthPattern){
  if(argc>4){
    *pattern1 = argv[1];
    *pattern2 = argv[2];
    *tries = strtol(argv[3],NULL,10);
    *threadCount = strtol(argv[4],NULL,10);
  }
  else if(argc>3){
    *pattern1 = argv[1];
    *pattern2 = argv[2];
    *tries = strtol(argv[3],NULL,10);
  }
  else if(argc>2){
    *pattern1 = argv[1];
    *pattern2 = argv[2];
  }
  else{
    printf("You should provide: <pattern1> <pattern2> <nr of threads>(optional) <number of trials>(optional)");
    exit(0);
  }

  long length1 = strlen(*pattern1);
  long length2 = strlen(*pattern2);
  
  if(length1>length2){
    *maxLengthPattern = length1;
  }
  else{
    *maxLengthPattern = length2;
  }
}
