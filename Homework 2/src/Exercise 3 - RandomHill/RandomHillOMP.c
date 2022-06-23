#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
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
	int threadCount;
	
	double minX;
	double maxX;
	double minY;
	double maxY;
	double stepSize;
	
	int tries;
	
	printf("Number of threads: ");
	scanf("%d",&threadCount);
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
	
	double maxValue=objectiveFunction(minX,minY);
	double maxValueX=minX;
	double maxValueY=minY;
    
	#pragma omp parallel for num_threads(threadCount)
	for (int i = 0; i < tries; i++ )
	{
		double x=rand()%((int)(maxX-minX+1)) + minX;
		double y=rand()%((int)(maxY-minY+1)) + minY;
		double currentResult=objectiveFunction(x,y);
		
		while(true){
			//get all the results from 8 directions the point can move.
			double* results = getResults(x,y,stepSize);
			//keep track if the values change during execution.
			bool changed = false;

			for(int j=0;j<8;j++){
				//if any result is higher than current result.
			   	if(results[j]>currentResult && 
			   	x+directions[j][0]*stepSize>=minX && x+directions[j][0]*stepSize<=maxX && 
			   	y+directions[j][1]*stepSize>=minY && y+directions[j][1]*stepSize<=maxY){
			   		//reassign x,y and their result. Indicate that the variables changed.
					currentResult = results[j];
					x += directions[j][0]*stepSize;
					y += directions[j][1]*stepSize;
					changed=true;	
			   		
			   		#pragma omp critical
			   		{
			   			if(currentResult>maxValue){
							maxValue = currentResult;
							maxValueX=x;
							maxValueY=y;	
						}
			   		}
			      	}
			}

			//if there aren't any changes, don't repeat the algorithm since we found the local maximum.
			if(!(changed)){
			      	break;
			}
		}
    	}
    
    	printf("The maximum value for the function in this region is %lf and it is archieved at x=%lf and y=%lf\n",maxValue,maxValueX,maxValueY);
    	

    	return 0;
}
