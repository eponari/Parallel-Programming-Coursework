public class Main {
    //global variables to keep track of the maximum value of the function and for which x and y is it archived.
    static double maxValue = Double.MIN_VALUE;
    static double maxValueX = 0;
    static double maxValueY = 0;


    static int tries;
    static int threadNo;
    static double minX;
    static double maxX;
    static double minY;
    static double maxY;
    static double stepSize;

    public static void main(String[] args) throws InterruptedException {
        //check for valid number of arguments.
        if(args.length!=7){
            System.out.println("You need to provide 7 parameters! <number of trials> <number of threads> <minimum X> <maximum X> <minimum Y> <maximum Y> <step size>");
            return;
        }

        //map arguments to their variables.
        tries = Integer.parseInt(args[0]);
        threadNo = Integer.parseInt(args[1]);
        minX = Double.parseDouble(args[2]);
        maxX = Double.parseDouble(args[3]);
        minY = Double.parseDouble(args[4]);
        maxY = Double.parseDouble(args[5]);
        stepSize = Double.parseDouble(args[6]);

        //data parallelism by splitting on the x-axis the problem for each thread to solve.
        double xDifference = (maxX-minX)/threadNo;

        //create array of threads from our class.
        HillClimbingThread[] threads = new HillClimbingThread[threadNo];

        for(int i=0;i<threadNo;i++){
            //get the boundaries for the thread to work.
            double currentMinX = minY + xDifference*i;
            double currentMaxX = currentMinX + xDifference;

            //create and start the current thread.
            threads[i] = new HillClimbingThread(i);
            threads[i].start();
        }

        //join all the threads.
        for(int i=0;i<threadNo;i++){
            threads[i].join();
        }

        //print the result.
        System.out.println("Number of threads: "+threadNo);
        System.out.println("Number of times the algorithm is applied: "+tries);
        System.out.println("Estimation of maximum value: "+maxValue);
        System.out.println("Value of X for the maximum value: "+maxValueX);
        System.out.println("Value of Y for the maximum value: "+maxValueY);
    }

    static class HillClimbingThread extends Thread{
        //variables to be used by the thread.
        int rank;

        //local maximum for this region, the value is initialized to the lowest possible value.
        double localMaxValue = Double.MIN_VALUE;
        double localMaxValueX = 0;
        double localMaxValueY = 0;

        //8 possible directions our x and y can take to find higher value(hill)
        final int[][] directions = {
                {-1,0},
                {1,0},
                {0,-1},
                {0,1},
                {1,1},
                {-1,-1},
                {-1,1},
                {1,-1},
        };

        //constructor.
        public HillClimbingThread(int rank) {
            this.rank=rank;

            localMaxValueX = rank + 0.5;
            localMaxValueY = 0.5;
            localMaxValue = objectiveFunction(localMaxValueX,localMaxValueY);
        }

        //function chosen by us.
        double objectiveFunction(double x, double y){
            return 50 + 20*x*y - 3*x*x*y - 2*x*y*y;
        }

        //get results in all 8 directions our algorithm can go.
        double[] getResults(double x,double y){
            double[] results = new double[directions.length];

            for(int i=0;i<8;i++){
                results[i] = objectiveFunction(x + directions[i][0]*stepSize,y + directions[i][1]*stepSize);
            }

            return results;
        }

        boolean isInside(double x,double y,int direction){
            return (x+directions[direction][0]>=minX&&x+directions[direction][0]<=maxX) && (y+directions[direction][1]>=minY&&y+directions[direction][1]<=maxY);
        }

        //function which represents the algorithm
        void hillClimb(double x,double y){
            double initialX = x;
            double initialY = y;

            //initial result from x and y. Will be updated each step.
            double currentResult = objectiveFunction(x,y);

            //repeat the algorithm for the number in the inputs.
            for(int i=0;i<tries;i++){
                //get all the results from 8 directions the point can move.
                double[] results = getResults(x,y);
                //keep track if the values change during execution.
                boolean changed = false;

                for(int j=0;j<8;j++){
                    if(!isInside(x,y,j)){
                        continue;
                    }
                    //if any result is higher than current result.
                    if(results[j]>currentResult){
                        //reassign x,y and their result. Indicate that the variables changed.
                        currentResult = results[j];
                        x += directions[j][0]*stepSize;
                        y += directions[j][1]*stepSize;
                        changed=true;
                    }
                    //if any result is higher than current local thread maximum result.
                    if(results[j]>localMaxValue){
                        //reassign local maximum variables and inidicate that the variables changed.
                        localMaxValue = results[j];
                        localMaxValueX = x + directions[j][0]*stepSize;
                        localMaxValueY = y + directions[j][1]*stepSize;
                        changed=true;
                    }
                }

                //if there aren't any changes, don't repeat the algorithm since we found the local maximum.
                if(!(changed)){
                    break;
                }
            }
        }

        @Override
        public void run(){
            //for each square unit where height*width%threadNo == rank.
            for(int i=rank;i<(maxX-minX)*(maxY-minY);i+=threadNo){
                //get the middle point on the square unit.
                double x = minX + (int)(i%(maxX-minX)) + 0.5;
                double y = minY + (int)(i/(maxY-minY)) + 0.5;

                //apply hill climbing algorithm in this point.
                hillClimb(x,y);
            }

            //synchronize this region.
            synchronized (this){
                //if the local maximum value of this thread is higher that the global max value.
                if(maxValue<localMaxValue){
                    //update the global max value variables.
                    maxValue = localMaxValue;
                    maxValueX = localMaxValueX;
                    maxValueY = localMaxValueY;
                }
            }

        }
    }
}