#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double randomNumber(){
	//get nr from 0 to 10000 and divide it be 100000 to get values from 0 to 1.
	double nr = (rand()%10001)/10000.0;
	
	return rand()%2==1?nr:-nr;
}



int main(int argc, char** argv) {
	int rank;
	int size;
	
	int m,n;
        
        int p;
        
        double* matrix1;
        double* matrix2;
        double* result;
        double* finalResult;
        
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(rank==0){
		printf("Program to calculate Matrix Multiplication\n");
		
		printf("First matrix mxn\n");
		
		printf("Enter m: ");
		scanf("%d",&m);
		
		printf("Enter n: ");
		scanf("%d",&n);
		
		matrix1 = malloc(n*m * sizeof(double));
		
		printf("Enter %dx%d numbers:\n",m,n);
		
		for(int i=0;i<m;i++){
			for(int j=0;j<n;j++){
				scanf("%lf",&matrix1[i*m+j]);
			}
		}
		
		printf("Second matrix nxp\n");
		
		printf("Enter p:");
		scanf("%d",&p);
		
		
        	matrix2 = malloc(n*p * sizeof(double));
		
		printf("Enter %dx%d numbers:\n",n,p);
		
		if(m%size!=0 || p%size!=0){
			printf("Can't distribute elements to the processes equally! m and p should be divisable by the number of processes.");
			MPI_Abort(MPI_COMM_WORLD,1);
			return 0;
		}
		
		for(int i=0;i<n;i++){
			for(int j=0;j<p;j++){
				scanf("%lf",&matrix2[i*n+j]);
			}
		}
		
		result = malloc(p*m/size * sizeof(double));
		finalResult = malloc(p*m*sizeof(double));
		
		MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(&p,1,MPI_INT,0,MPI_COMM_WORLD);
		
		MPI_Scatter(&matrix1,m*n/size,MPI_DOUBLE,&matrix1,m*n/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
		MPI_Bcast(&matrix2,p*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
	}else{
		MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
		MPI_Bcast(&p,1,MPI_INT,0,MPI_COMM_WORLD);
		
		matrix1 = malloc(n*m/size * sizeof(double));
		matrix2 = malloc(n*p * sizeof(double));
		result = malloc(p*m/size * sizeof(double));
		
		MPI_Scatter(&matrix1,m*n/size,MPI_DOUBLE,&matrix1,m*n/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
		MPI_Bcast(&matrix2,p*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
	}
	
	
	
	for (int i = 0; i < m*p/size; i++){
       	result[i] = 0;
    	}
    	
    	for (int i = 0; i < m*n/size; i++){
       	for(int j=0;j<p;j++){
       		for(int k=0;k<n;k++){
       			result[i*n+j] += matrix1[i*n+k]*matrix2[j*p+k];
       		}
       	}
    	}
    		
        MPI_Gather(&result, m*p/size, MPI_DOUBLE,&finalResult, m*p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
        if(rank == 0) {        	
        	for(int i=0;i<m;i++){
        		for(int j=0;j<p;j++){
        			printf("%lf ",finalResult[i*m+j]);
        		}
        		printf("\n");
        	}
        }
	
        MPI_Finalize();
        return 0;
}
