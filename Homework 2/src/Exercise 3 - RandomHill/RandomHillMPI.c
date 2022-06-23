#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdbool.h>

//8 directions.
int directions[8][2] = {
  	{-1,0},
  	{1,0},
  	{0,-1},
  	{0,1},
  	{1,1},
  	{-1,-1},
  	{-1,1},
  	{1,-1},
};

//function chosen by us.
double objectiveFunction(double x, double y){
  	return 50 + 20*x*y - 3*x*x*y - 2*x*y*y;
}

//get results in all 8 directions our algorithm can go.
double* getResults(double x,double y,double stepSize){
  	static double results[8];
  	
  	for(int i=0;i<8;i++){
    		results[i] = objectiveFunction(x + directions[i][0]*stepSize,y + directions[i][1]*stepSize);
  	}

  	return results;
}

int main(int argc,char* argv[]){
	int rank;
	int size;
	
	double minX;
	double maxX;
	double minY;
	double maxY;
	double stepSize;
	
	int tries;
	
	double maxValue;
	
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank==0){
		printf("Number of tries: ");
		scanf("%d",&tries);
		printf("Minimum X: ");
		scanf("%lf",&minX);
		printf("Maximum X: ");
		scanf("%lf",&maxX);
		printf("Minimum Y: ");
		scanf("%lf",&minY);
		printf("Maximum Y: ");
		scanf("%lf",&maxY);
		printf("Step size: ");
		scanf("%lf",&stepSize);
	}
	
	MPI_Bcast(&tries,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&minX,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(&maxX,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(&minY,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(&maxY,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(&stepSize,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	
	double x=rand()%((int)(maxX-minX+1)) + minX;
	double y=rand()%((int)(maxY-minY+1)) + minY;
	double currentResult=objectiveFunction(x,y);
	
	
	for (int i = 0; i < tries/size + (tries%size > rank); i++ ){
		while(true){
			//get all the results from 8 directions the point can move.
			double* results = getResults(x,y,stepSize);
			//keep track if the values change during execution.
			bool changed = false;

			for(int j=0;j<8;j++){
				//if any result is higher than current result.
			   	if(results[j]>currentResult){
					//reassign x,y and their result. Indicate that the variables changed.
					currentResult = results[j];
					x += directions[j][0]*stepSize;
					y += directions[j][1]*stepSize;
					changed=true;
			      	}
			}

			//if there aren't any changes, don't repeat the algorithm since we found the local maximum.
			if(!(changed)){
			      	break;
			}
		}
    	}
    	
    	MPI_Allreduce(&currentResult, &maxValue, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    	
    	int correctRank = rank;
    	
    	MPI_Bcast(&correctRank,1,MPI_INT,rank,MPI_COMM_WORLD);
    	
    	if(rank==correctRank){
    		printf("The maximum value for the function in this region is %lf and it is archieved at x=%lf and y=%lf\n",currentResult,x,y);
    	}
    	
    	MPI_Finalize();
    	
	
    	return 0;
}
