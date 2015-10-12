/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD
 * Student: 
 * Implement Pthreads version of Strassen algorithm for matrix multiplication.
 */

#include "timer.h"
#include "io.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define NUM_THREADS 7
bool SEQUENTIAL = true;
#define BREAK 1
// Make these globals so threads can operate on them. You will need to
// add additional matrixes for all the M and C values in the Strassen
// algorithms.
int **A;
int **B;
int **C;
// Reference matrix, call simpleMM to populate.
int **R;
int **M1,**M2,**M3,**M4,**M5,**M6,**M7;

int **allocMatrix(int size); 
// Stupid simple Matrix Multiplication, meant as example.
void simpleMM(int N) {
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      for (int k=0; k<N; k++) {
	R[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

// WRITE YOUR CODE HERE, you will need to also add functions for each
// of the sub-matrixes you will need to calculate but you can create your
// threads in this fucntion.
void *ComputeM(void *n){
	printf("Hello Thread %d\n",*(int *)n);
	return (void *) NULL;
}

void *ComputeC(void *n){
	return (void *) NULL;
}
void add(int **X, int **Y, int **Z, int n){
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
	Z[i][j] = X[i][j] + Y[i][j];
    }
  }
}
void sub(int **X, int **Y, int **Z, int n){
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
	Z[i][j] = X[i][j] - Y[i][j];
    }
  }
}
void multiply(int **X, int **Y, int **Z, int n){
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      	Z[i][j]=0;
	for (int k=0; k<n; k++) {
		Z[i][j] += X[i][k] * Y[k][j];
      	}
    }
  }
}

void matrixMul(int **X,int **Y, int **Z,int n);
void matmul(int **X,int **Y, int **Z,int n);
void matcopy(int **target, int **source, int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			target[i][j] = source[i][j];
		}	
	}
}
void strassenMM(int N) {
	/*pthread_t ids[NUM_THREADS];
  	int *MyID = (int *)malloc(NUM_THREADS * sizeof(int));
	for(int i=0;i<NUM_THREADS;i++){
		MyID[i] = i;
		pthread_create(&ids[i],NULL,ComputeM, &(MyID[i]));
	}
	for(int i=0;i<NUM_THREADS;i++){
		pthread_join(ids[i],NULL);
	}*/
	// sequential 
	// pad matrix
	int n = ceil((double)N/2);
	printf("N=%d, n=%d\n",N,n);
	int N_pad = 2*n;
	
	if(N!=N_pad){
	int **new_A, **new_B;
	new_A = allocMatrix(N_pad);
	new_B = allocMatrix(N_pad);
	matcopy(new_A,A,N);
	matcopy(new_B,B,N);
	free(A);free(B);free(C);
	A = new_A;B=new_B;
	C = allocMatrix(N_pad);
	}
	//matmul(A,B,C,N_pad);
	matrixMul(A,B,C,N_pad);
}

void ComputeM1(int **a11,int **a22,int **b11,int **b22, int **M1,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M1...\n");	
		
		add(a11,a22,sum_a,new_n);
		add(b11,b22,sum_b,new_n);
		multiply(sum_a,sum_b,M1,new_n);
		free(sum_a);free(sum_b);
}
void ComputeM2(int **a21,int **a22,int **b11,int **M2,int new_n){
		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M2...\n");	

		add(a21,a22,sum_a,new_n);
		//sub(b21,b22,sum_b,new_n);
		multiply(sum_a,b11,M2,new_n);
		free(sum_a);free(sum_b);
}

void ComputeM3(int **a11,int **b12,int **b22, int **M3,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M3...\n");	
		


		//add(a11,a22,sum_a,new_n);
		sub(b12,b22,sum_b,new_n);
		multiply(a11,sum_b,M3,new_n);
		free(sum_a);free(sum_b);
}
void ComputeM4(int **a22,int **b21,int **b11, int **M4,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M4...\n");	
		

		//add(a11,a22,sum_a,new_n);
		sub(b21,b11,sum_b,new_n);
		multiply(a22,sum_b,M4,new_n);
		free(sum_a);free(sum_b);
}
void ComputeM5(int **a11,int **a12,int **b22, int **M5,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M5...\n");	
		
	
		add(a11,a12,sum_a,new_n);
		//sub(b21,b11,sum_b,new_n);
		multiply(sum_a,b22,M5,new_n);
		free(sum_a);free(sum_b);
}

void ComputeM6(int **a21,int **a11,int **b11,int **b12, int **M6,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M6...\n");	
		
	
		sub(a21,a11,sum_a,new_n);
		add(b11,b12,sum_b,new_n);
		multiply(sum_a,sum_b,M6,new_n);
		free(sum_a);free(sum_b);
}
	
void ComputeM7(int **a12,int **a22,int **b21,int **b22, int **M7,int new_n){

		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M7...\n");	
		
		sub(a12,a22,sum_a,new_n);
		add(b21,b22,sum_b,new_n);
		multiply(sum_a,sum_b,M7,new_n);
		free(sum_a);free(sum_b);
}

void addmuladd(int *a1,int*a2,int *b1,int *b2,int **c,int n,int rw){
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int ind = i*rw+j;
			sum_a[i][j] = a1[ind]+a2[ind];			
			sum_b[i][j] = b1[ind]+b2[ind];			
		}	
	}
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i][j]=0;
	for(int k=0;k<n;k++){
		c[i][j] += sum_a[i][k]*sum_b[k][j]; 
	}}}
	free(sum_a);free(sum_b);
}
void addmul(int *a1,int*a2,int *b1,int **c,int n,int rw){
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int ind = i*rw+j;
			sum_a[i][j] = a1[ind]+a2[ind];			
			sum_b[i][j] = b1[ind];			
		}	
	}
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i][j]=0;
	for(int k=0;k<n;k++){
		c[i][j] += sum_a[i][k]*sum_b[k][j]; 
	}}}
	free(sum_a);
	free(sum_b);
}
void mulsub(int *a1,int *b1,int *b2,int **c,int n,int rw){
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int ind = i*rw+j;
			sum_a[i][j] = a1[ind];			
			sum_b[i][j] = b1[ind]-b2[ind];			
		}	
	}
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i][j]=0;
	for(int k=0;k<n;k++){
		c[i][j] +=sum_a[i][k]*sum_b[k][j]; 
	}}}
	free(sum_a);
	free(sum_b);
}
void submuladd(int *a1,int*a2,int *b1,int *b2,int **c,int n,int rw){
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			int ind = i*rw+j;
			sum_a[i][j] = a1[ind]-a2[ind];			
			sum_b[i][j] = b1[ind]+b2[ind];			
		}	
	}
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i][j]=0;
	for(int k=0;k<n;k++){
		c[i][j] += sum_a[i][k]*sum_b[k][j]; 
	}}}
	free(sum_a);free(sum_b);
}
void AddCFour(int **m1,int**m2,int**m3,int **m4,int **c,int n,int row, int col){
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i+(row-1)*n][j+(col-1)*n]=m1[i][j]+m2[i][j]+m3[i][j]-m4[i][j];
	}}
}
void AddCTwo(int **m1,int**m2,int **c,int n,int row, int col){
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
	c[i+(row-1)*n][j+(col-1)*n]=m1[i][j]+m2[i][j];
	}}
}
void compareMat(int *a, int *b, int n,int rw){
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		if(a[i*rw+j]!=b[i*rw+j]){
			printf("Matrix unequal...\n");
			return;
		}
	}}
}
void printMat(int *a, int n, int rw){
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		printf("%d\t",a[i*rw+j]);
}
	printf("\n");
}
}
void *GetC11(void *m){

	int n = *((int*)m);
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		C[i][j] = M1[i][j]+M4[i][j]-M5[i][j]+M7[i][j];
}}
return NULL;
}

void *GetC12(void *m){

	int n = *((int*)m);
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		C[i][j+n] = M3[i][j]+M5[i][j];//-M5[i][j]+M7[i][j];
}}
return NULL;
}

void *GetC21(void *m){

	int n = *((int*)m);
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		C[i+n][j] = M2[i][j]+M4[i][j];//-M5[i][j]+M7[i][j];
}}
return NULL;
}

void *GetC22(void *m){

	int n = *((int*)m);
	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		C[i+n][j+n] = M1[i][j]-M2[i][j]+M3[i][j]+M6[i][j];
}}
return NULL;
}


void *GetM1(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i][j]+A[i+n][j+n];
		sum_b[i][j] = B[i][j]+B[i+n][j+n];
		M1[i][j]=0;
}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M1[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}

void *GetM2(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i+n][j]+A[i+n][j+n];
		sum_b[i][j] = B[i][j];//+B[i+n][j+n];
		M2[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M2[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}


void *GetM3(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i][j];//+A[i+n][j+n];
		sum_b[i][j] = B[i][j+n]-B[i+n][j+n];
		M3[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M3[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}

void *GetM4(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i+n][j+n];
		sum_b[i][j] = B[i+n][j]-B[i][j];
		M4[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M4[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}

void *GetM5(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i][j]+A[i][j+n];
		sum_b[i][j] = B[i+n][j+n];//-B[i][j];
		M5[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M5[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}

void *GetM6(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i+n][j]-A[i][j];
		sum_b[i][j] = B[i][j]+B[i][j+n];
		M6[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M6[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}

void *GetM7(void *m){
	int n = *((int*)m);
	int **sum_a = allocMatrix(n);
	int **sum_b = allocMatrix(n);

	for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		sum_a[i][j] = A[i][j+n]-A[i+n][j+n];
		sum_b[i][j] = B[i+n][j]+B[i+n][j+n];
		M7[i][j]=0;

}}	
for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		for(int k = 0;k<n;k++){
		M7[i][j] += sum_a[i][k]*sum_b[k][j];
}}}
	free(sum_a);free(sum_b);
	return NULL;
}


void matrixMul(int **X,int **Y, int **Z,int n){
		//printf("Enter matrix mul...\n");
		int new_n = n/2;
		int *a11,*a12,*a21,*a22,*b11,*b12,*b21,*b22;
		a11 = X[0];a12=X[0]+new_n;
		a21 = X[new_n];a22=X[new_n]+new_n;	
		b11 = Y[0];b12=Y[0]+new_n;
		b21 = Y[new_n];b22=Y[new_n]+new_n;
		int rw = &X[1][0]-&X[0][0];		
		//compareMat(a11,X[0],new_n,rw);		
		//printf("Y...\n");printMat(Y[0],n,rw);
		//printf("b12...\n");printMat(b12,new_n,rw);	
		//printf("b21...\n");printMat(b21,new_n,rw);	
		
		M1 = allocMatrix(new_n);
		M2 = allocMatrix(new_n);
		M3 = allocMatrix(new_n);
		M4 = allocMatrix(new_n);
		M5 = allocMatrix(new_n);
		M6 = allocMatrix(new_n);
		M7 = allocMatrix(new_n);
		
		if(SEQUENTIAL){
		printf("Sequential Strassen...\n");
		/*addmuladd(a11,a22,b11,b22,M1,new_n,rw);	
		addmul(a21,a22,b11,M2,new_n,rw);
		mulsub(a11,b12,b22,M3,new_n,rw);
		mulsub(a22,b21,b11,M4,new_n,rw);
		addmul(a11,a12,b22,M5,new_n,rw);
		submuladd(a21,a11,b11,b12,M6,new_n,rw);
		submuladd(a12,a22,b21,b22,M7,new_n,rw);	
	
		int *C11,*C12,*C21,*C22;
		rw = &Z[1][0]-&Z[0][0];
		C11 = Z[0];C12=Z[0]+new_n;
		C21 = Z[new_n];C22=Z[new_n]+new_n;
		AddCFour(M1,M4,M7,M5,Z,new_n,1,1);
		AddCTwo(M3,M5,Z,new_n,1,2);
		
		rw = &Z[1+new_n][0]-&Z[new_n][0];
		AddCTwo(M2,M4,Z,new_n,2,1);
		AddCFour(M1,M3,M6,M2,Z,new_n,2,2);
		*/	
		GetM1(&new_n);
		GetM2(&new_n);
		GetM3(&new_n);
		GetM4(&new_n);
		GetM5(&new_n);
		GetM6(&new_n);
		GetM7(&new_n);
		
		GetC11(&new_n);
		GetC12(&new_n);
		GetC21(&new_n);
		GetC22(&new_n);
		}else{
		printf("Parallel Strassen...\n");
		pthread_t ids[NUM_THREADS];
		pthread_create(&ids[0],NULL,GetM1,&new_n);
		pthread_create(&ids[1],NULL,GetM2,&new_n);
		pthread_create(&ids[2],NULL,GetM3,&new_n);
		pthread_create(&ids[3],NULL,GetM4,&new_n);
		pthread_create(&ids[4],NULL,GetM5,&new_n);
		pthread_create(&ids[5],NULL,GetM6,&new_n);
		pthread_create(&ids[6],NULL,GetM7,&new_n);
		for(int i = 0;i<NUM_THREADS;i++){
			pthread_join(ids[i],NULL);
		}
		/*
		pthread_create(&ids[0],NULL,GetC11,&new_n);
		pthread_create(&ids[1],NULL,GetC12,&new_n);
		pthread_create(&ids[2],NULL,GetC21,&new_n);
		pthread_create(&ids[3],NULL,GetC22,&new_n);
		for(int i = 0;i<4;i++){
			pthread_join(ids[i],NULL);
		}
		*/
		GetC11(&new_n);
		GetC12(&new_n);
		GetC21(&new_n);
		GetC22(&new_n);

		}
		free(M1);free(M2);free(M3);free(M4);
		free(M5);free(M6);free(M7);
}
void matmul(int **X,int **Y, int **Z,int n){
		//printf("Enter matmul...\n");
		int new_n = n/2;
		int **a11,**a12,**a21,**a22,**b11,**b12,**b21,**b22;	
		
		a11 = allocMatrix(new_n);
		a12 = allocMatrix(new_n);
		//a12 = (int **)(X[0]+new_n);
		a21 = allocMatrix(new_n);
		a22 = allocMatrix(new_n);
		b11 = allocMatrix(new_n);
		b12 = allocMatrix(new_n);
		b21 = allocMatrix(new_n);
		b22 = allocMatrix(new_n);
		
		printf("Split Mat...\n");
		for(int i =0;i<new_n;i++){
			for(int j=0;j<new_n;j++){
				
				a11[i][j]=X[i][j];
				a12[i][j]=X[i][j+new_n];
				a21[i][j]=X[i+new_n][j];
				a22[i][j]=X[i+new_n][j+new_n];
				
				b11[i][j]=Y[i][j];
				b12[i][j]=Y[i][j+new_n];
				b21[i][j]=Y[i+new_n][j];
				b22[i][j]=Y[i+new_n][j+new_n];
			}
		}
		/*
		a21 = &(X[new_n]);
		a22= &(X[new_n])+new_n;
		b11 = Y;
		b12= &(Y[0])+new_n;
		b21 = &(Y[new_n]);
		b22= &(Y[new_n])+new_n;
		*/
		printf("A...\n");
		//printMatrix(A,n);
		printf("B...\n");
		//printMatrix(B,n);
		
		int **M1,**M2,**M3,**M4,**M5,**M6,**M7;
		M1 = allocMatrix(new_n);
		M2 = allocMatrix(new_n);
		M3 = allocMatrix(new_n);
		M4 = allocMatrix(new_n);
		M5 = allocMatrix(new_n);
		M6 = allocMatrix(new_n);
		M7 = allocMatrix(new_n);
		
		/*
		int **sum_a = allocMatrix(new_n);
		int **sum_b = allocMatrix(new_n);
		
		printf("Compute M...\n");	
		
		add(a11,a22,sum_a,new_n);
		add(b11,b22,sum_b,new_n);
		multiply(sum_a,sum_b,M1,new_n);
		
		add(a21,a22,sum_a,new_n);
		//sub(b21,b22,sum_b,new_n);
		multiply(sum_a,b11,M2,new_n);
		
		//add(a11,a22,sum_a,new_n);
		sub(b12,b22,sum_b,new_n);
		multiply(a11,sum_b,M3,new_n);

		//add(a11,a22,sum_a,new_n);
		sub(b21,b11,sum_b,new_n);
		multiply(a22,sum_b,M4,new_n);
	
		add(a11,a12,sum_a,new_n);
		//sub(b21,b11,sum_b,new_n);
		multiply(sum_a,b22,M5,new_n);
	
		sub(a21,a11,sum_a,new_n);
		add(b11,b12,sum_b,new_n);
		multiply(sum_a,sum_b,M6,new_n);
	
		sub(a12,a22,sum_a,new_n);
		add(b21,b22,sum_b,new_n);
		multiply(sum_a,sum_b,M7,new_n);
		free(sum_a);free(sum_b);
		*/
	 	ComputeM1(a11,a22,b11,b22,M1,new_n);	
	 	ComputeM2(a21,a22,b11,M2,new_n);	
	 	ComputeM3(a11,b12,b22,M3,new_n);	
	 	ComputeM4(a22,b21,b11,M4,new_n);	
	 	ComputeM5(a11,a12,b22,M5,new_n);	
	 	ComputeM6(a21,a11,b11,b12,M6,new_n);	
	 	ComputeM7(a12,a22,b21,b22,M7,new_n);	

		printf("Compute R...\n");
		// compute C			
		int **C11 = allocMatrix(new_n);
		int **C12 = allocMatrix(new_n);
		int **C21 = allocMatrix(new_n);
		int **C22 = allocMatrix(new_n);
		add(M1,M4,C11,new_n);
		sub(C11,M5,C11,new_n);
		add(C11,M7,C11,new_n);
		
		add(M3,M5,C12,new_n);
		
		add(M2,M4,C21,new_n);
		
		sub(M1,M2,C22,new_n);
		add(C22,M3,C22,new_n);
		add(C22,M6,C22,new_n);
		
		for(int i =0;i<new_n;i++)
			for(int j =0;j<new_n;j++){
				Z[i][j]=C11[i][j];
				Z[i][j+new_n]=C12[i][j];
				Z[i+new_n][j]=C21[i][j];
				Z[i+new_n][j+new_n]=C22[i][j];
		}
		
		free(a11);
		free(a12);
		free(a21);free(a22);	
		free(b11);free(b12);free(b21);free(b22);	
		free(C11);free(C12);free(C21);free(C22);	
		free(M1);free(M2);free(M3);free(M4);
		free(M5);free(M6);free(M7);
		
}


// Allocate square matrix.
int **allocMatrix(int size) {
  int **matrix;
  matrix = (int **)malloc(size * sizeof(int *));
  for (int row = 0; row < size; row++) {
    matrix[row] = (int *)malloc(size * sizeof(int));
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

// Allocate memory for all the matrixes, you will need to add code
// here to initialize any matrixes that you need.
void initMatrixes(int N) {
  A = allocMatrix(N); B = allocMatrix(N); C = allocMatrix(N); R = allocMatrix(N);
}

// Free up matrixes.
void cleanup() {
  free(A);
  free(B);
  free(C);
  free(R);
}

// Main method
int main(int argc, char* argv[]) {
  int N;
  double elapsedTime;

  // checking parameters
  if (argc != 2 && argc != 4) {
    printf("Parameters: <N> [<fileA> <fileB>]\n");
    return 1;
  }
  N = atoi(argv[1]);
  initMatrixes(N);

  // reading files (optional)
  if(argc == 4){
    readMatrixFile(A,N,argv[2]);
    readMatrixFile(B,N,argv[3]);
  } else {
    // Otherwise, generate two random matrix.
    for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
	A[i][j] = rand() % 5;
	B[i][j] = rand() % 5;
      }
    }
  }

  // Do simple multiplication and time it.
  timerStart();
  simpleMM(N);
  printf("Simple MM took %ld ms\n\n", timerStop());

  // Do strassen multiplication and time it.
  timerStart();
SEQUENTIAL = true;
  strassenMM(N);
  printf("Strassen MM took %ld ms\n\n", timerStop());
  // Do strassen multiplication and time it.
  timerStart();
  SEQUENTIAL = false;strassenMM(N);
  printf("Strassen MM took %ld ms\n\n", timerStop());

  if (compareMatrix(C, R, N) != 0) {
    if (N < 20) {
      printf("\n\n------- MATRIX C\n");
      printMatrix(C,N);
      printf("\n------- MATRIX R\n");
      printMatrix(R,N);
    }
    printf("Matrix C doesn't match Matrix R, if N < 20 they will be printed above.\n");
  }

  // stopping timer
  
  cleanup();
  return 0;
}
