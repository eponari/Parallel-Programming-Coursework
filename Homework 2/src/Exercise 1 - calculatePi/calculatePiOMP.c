#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double randomNumber(){
	//get nr from 0 to 10000 and divide it be 100000 to get values from 0 to 1.
	double nr = (rand()%10001)/10000.0;
	
	return rand()%2==1?nr:-nr;
}

int main(void){
	int nrOfThreads;
	int nrOfTries;
	
	printf("Program to calculate PI\n");
	
	printf("Number of tries: ");
	scanf("%d",&nrOfTries);
	
	printf("Number of threads: ");
	scanf("%d",&nrOfThreads);
	
	double insideCircle = 0;
	
	#pragma omp parallel for num_threads(nrOfThreads) reduction(+: insideCircle)
	for (int i = 0; i < nrOfTries; i++ )
	{
        	double x = randomNumber();
        	double y = randomNumber();
		
		if(sqrt(x*x+y*y)<=1){
			insideCircle++;
		}
    }
    	
    	printf("The estimation of PI is: %f\n",4 * (float)insideCircle/nrOfTries);
    	

    return 0;
}
