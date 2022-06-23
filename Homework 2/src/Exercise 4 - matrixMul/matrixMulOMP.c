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
	
	printf("Program to calculate Matrix Multiplication\n");
	
	int m,n;
	
	printf("First matrix mxn\n");
	
	printf("Enter m: ");
	scanf("%d",&m);
	
	printf("Enter n: ");
	scanf("%d",&n);
	
	double matrix1[n][m];
	
	printf("Enter %dx%d numbers:\n",m,n);
	
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			scanf("%lf",&matrix1[j][i]);
		}
	}
	
	printf("Second matrix nxp\n");
	
	int p;
	
	printf("Enter p:");
	scanf("%d",&p);
	
	double matrix2[p][n];
	
	printf("Enter %dx%d numbers:\n",n,p);
	
	for(int i=0;i<n;i++){
		for(int j=0;j<p;j++){
			scanf("%lf",&matrix2[j][i]);
		}
	}
	
	
	printf("Number of threads: ");
	scanf("%d",&nrOfThreads);
	
	double result[m][p];
	
	for(int i=0;i<m;i++){
		for(int j=0;j<p;j++){
			result[j][i]=0;
		}
		printf("\n");
	}
	
	int i,j,k;
	
	#pragma omp parallel for num_threads(nrOfThreads/2) private(j,k)
	for (i = 0; i < m; i++ )
	{
        	#pragma omp parallel for num_threads(2) private(k)
        	for(j=0;j<p;j++){
        		
        		for(k=0;k<n;k++){
        			#pragma omp critical
        			result[j][i] += matrix1[k][i]*matrix2[j][k];
        		}
        		
        	}
    	}
    	
    	printf("Result of multiplication:\n");
    	
    	for(int i=0;i<m;i++){
		for(int j=0;j<p;j++){
			printf("%lf ",result[j][i]);
		}
		printf("\n");
	}

    return 0;
}
