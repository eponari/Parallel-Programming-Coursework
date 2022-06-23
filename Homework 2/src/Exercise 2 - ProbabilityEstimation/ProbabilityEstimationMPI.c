#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "stringMethods.h"

void getArgs();

int main(int argc,char* argv[]){
	char* pattern1 = malloc(256);
	char* pattern2 = malloc(256);
	int maxLengthPattern=255;
	
	int tries = 100000;
	
	int pattern1Occurences = 0;
	int pattern2Occurences = 0;
	
	int rank;
	int size;
	
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank==0){
		printf("First pattern: ");
		scanf("%255s",pattern1);
		printf("Second pattern: ");
		scanf("%255s",pattern2);
		printf("Number of tries: ");
		scanf("%d",&tries);
		
		if(strlen(pattern1)>strlen(pattern2)){
			maxLengthPattern=strlen(pattern1);
		}else{
			maxLengthPattern=strlen(pattern2);
		}
	}
	
	MPI_Bcast(pattern1,255,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(pattern2,255,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(&tries,1,MPI_INT,0,MPI_COMM_WORLD);
	
	
	int localPattern1Occurences = 0;
	int localPattern2Occurences = 0;
	
	for (int i = 0; i < tries/size + (tries%size > rank); i++ ){
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
      				localPattern1Occurences++;
      				cleanStr(generatedPattern,maxLengthPattern);
      				break;
    			}
    			else if(endsWith(generatedPattern,pattern2)){
    				cleanStr(generatedPattern,maxLengthPattern);
      				localPattern2Occurences++;
      				break;
    			}
			
			//shiftLeft is my own defined function. shifts the pattern by 1 when the pattern generated gets longer than the actual maximum length.
    			if(index==maxLengthPattern-1){
      				shiftLeft(generatedPattern,maxLengthPattern);
    			}
        	}
    	}
    	
    	
    	MPI_Reduce(&localPattern1Occurences, &pattern1Occurences, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    	MPI_Reduce(&localPattern2Occurences, &pattern2Occurences, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    	
    	if(rank==0){
    		printf("The probability of %s to appear first is: %f\n",pattern1,(float)pattern1Occurences/tries);
    		printf("The probability of %s to appear first is: %f\n",pattern2,(float)pattern2Occurences/tries);
    	}
	MPI_Finalize();
    	return 0;
}
