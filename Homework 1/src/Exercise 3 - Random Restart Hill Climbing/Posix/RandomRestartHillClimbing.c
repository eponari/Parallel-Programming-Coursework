//include needed libraries.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

//default values are as low as possible.
double globalMaxValue = -10000000;
double globalMaxValueX = 0;
double globalMaxValueY = 0;

//function declaration.
void getArgs();
void* calculateHillClimbing();

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

//global variables.
int tries;
int threadNo;
double stepSize;
double minX,minY,maxX,maxY;

//mutex to syncronize a region and keep data integrity.
pthread_mutex_t mutex;

int main(int argc,char* argv[]) {
	//get arguments and if the number isn't right, print this message.
  if(argc!=8){
    printf("You need to provide 7 parameters! <number of trials> <number of threads> <minimum X> <maximum X> <minimum Y> <maximum Y> <step size>");
    return 0;
  }

  getArgs(argv);

	//create array of threads and initialize mutex.
  pthread_t threads[threadNo];
  pthread_mutex_init(&mutex, NULL);

	//create each thread.
  for(long i=0;i<threadNo;i++){
    pthread_create(&threads[i],NULL,calculateHillClimbing,(void*)i);
  }

	//join all threads.
  for(long i=0;i<threadNo;i++){
    pthread_join(threads[i], NULL);
  }

	//print the results.
  printf("Number of threads: %d\n",threadNo);
  printf("Number of times the algorithm is applied: %d\n",tries);
  printf("Estimation of maximum value: %lf\n",globalMaxValue);
  printf("Value of X for the maximum value: %lf\n",globalMaxValueX);
  printf("Value of Y for the maximum value: %lf\n",globalMaxValueY);

  return 0;
}

//function chosen by us.
double objectiveFunction(double x, double y){
  return 50 + 20*x*y - 3*x*x*y - 2*x*y*y;
}

//get results in all 8 directions our algorithm can go.
double* getResults(double x,double y){
  static double results[8];
     

  for(int i=0;i<8;i++){
    results[i] = objectiveFunction(x + directions[i][0]*stepSize,y + directions[i][1]*stepSize);
  }

  return results;
}

//function which represents the algorithm
void hillClimb(double x,double y,double* maxValue,double* maxValueX,double* maxValueY){
  double initialX = x;
  double initialY = y;

  //initial result from x and y. Will be updated each step.
  double currentResult = objectiveFunction(x,y);

  //repeat the algorithm for the number in the inputs.
  for(int i=0;i<tries;i++){
    //get all the results from 8 directions the point can move.
    double* results = getResults(x,y);
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
      //if any result is higher than current local thread maximum result.
      if(results[j]>*maxValue){
        //reassign local maximum variables and inidicate that the variables changed.
        *maxValue = results[j];
        *maxValueX = x + directions[j][0]*stepSize;
        *maxValueY = y + directions[j][1]*stepSize;
        changed=true;
      }
    }

    //if there aren't any changes, don't repeat the algorithm since we found the local maximum.
    if(!(changed)){
      break;
    }
  }
}

void* calculateHillClimbing(void* param){
  long rank = (long) param;

  double localMaxValue = -1000000000;
  double localMaxValueX = 0;
  double localMaxValueY = 0;

  for(int i=rank;i<(maxX-minX)*(maxY-minY);i+=threadNo){
    //get the middle point on the square unit.
    double x = minX + i%threadNo + 0.5;
    double y = minY + rank  + 0.5;

    //apply hill climbing algorithm in this point.
    hillClimb(x,y,&localMaxValue,&localMaxValueX,&localMaxValueY);
  }

  pthread_mutex_lock(&mutex);
  //if the local maximum value of this thread is higher that the global max value.
  if(globalMaxValue<localMaxValue){
    //update the global max value variables.
    globalMaxValue = localMaxValue;
    globalMaxValueX = localMaxValueX;
    globalMaxValueY = localMaxValueY;
  }
  pthread_mutex_unlock(&mutex);

  return NULL;
}

void getArgs(char* argv[]){
  tries = atoi(argv[1]);
  threadNo = atoi(argv[2]);
  minX = atof(argv[3]);
  minY = atof(argv[5]);
  maxX = atof(argv[4]);
  maxY = atof(argv[6]);
  stepSize = atof(argv[7]);
}