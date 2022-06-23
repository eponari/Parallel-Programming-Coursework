#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double randomNumber(){
	//get nr from 0 to 10000 and divide it be 100000 to get values from 0 to 1.
	double nr = (rand()%10001)/10000.0;
	
	return rand()%2==1?nr:-nr;
}



int main(int argc, char** argv) {
        int result;
	int nrOfTries=10000000;
	int insideCircle = 0;
	
	int rank;
	int size;
	
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank==0){
		printf("Number of tries: ");
		scanf("%d",&nrOfTries);
	}
	
	for (int i = 0; i < nrOfTries/size + (nrOfTries%size > rank); i++){
       	double x = randomNumber();
        	double y = randomNumber();
		
		if(x*x+y*y<=1){
			insideCircle++;
		}
    	}
    		
        MPI_Reduce(&insideCircle, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
        if(rank == 0) {        	
        	printf("The estimation of PI is: %f\n",4 * (float)result/nrOfTries);
        }
	
        MPI_Finalize();
        return 0;
}
