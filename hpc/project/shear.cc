/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD (bmills@cs.pitt.edu)
 * STUDENTS: Implement OpenMP parallel shear sort.
 */

#include <omp.h>
#include <math.h>
#include "timer.h"
#include "io.h"

#define MAX_VALUE 10000

void SortColumn(int **A, int M){
	int k, len, temp, i;
	bool Up;
	#pragma omp parallel for \
	default(none) private(k,len,temp,i,Up) shared(A,M)
	for(int k= 0;k<M;k++){
	for(int len = M-1; len >= 2; len--){
		for(int i =0 ; i < len ; i++){	
			if(A[i][k]>A[i+1][k]){
			int temp = A[i][k];
			A[i][k] = A[i+1][k];
			A[i+1][k] = temp;
			}
		}
	}
	}
}


void SortRow(int **A, int M){
	int k, len, temp, i;
	bool Up;
	#pragma omp parallel for \
	default(none) private(k,len,temp,i,Up) shared(A,M)
	for(k= 0;k<M;k++){
	Up = (k%2==0);
	for(len = M-1; len >= 2; len--){
		for(i =0 ; i < len ; i++){	
			if((A[k][i]>A[k][i+1] && Up) ||(A[k][i]<A[k][i+1]) && !Up){
			temp = A[k][i];
			A[k][i] = A[k][i+1];
			A[k][i+1] = temp;
			}
		}
	}
	}
}

void shear_sort(int **A, int M) {
  // Students: Implement parallel shear sort here.
	int MaxLoop = ceil(log2(M*M));
	//printf("MaxLoop=%d\n",MaxLoop);
	for(int i=0;i<MaxLoop;i++){
		SortRow(A, M);
		SortColumn(A,M);
	}
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
bool CheckResult(int **A, int M){
for(int i=0;i<M;i++){
bool right = (i%2==0);
for(int j=0;j<M;j++){
	if(i==M-1){
	 if(j<M-1){
		if((A[i][j]<A[i][j+1] && !right)|| (A[i][j]>A[i][j+1] && right))return false;
	}
	}else{
	if(j==M-1 && right){
		if(A[i][j]>A[i+1][j])return false;
	}else if(j == 0 && !right){
		if(A[i][j]>A[i+1][j])return false;
	}else{
		if((A[i][j]<A[i][j+1] && !right)|| (A[i][j]>A[i][j+1] && right))return false;
		}
	}
}}
return true;
}
// Main method      
int main(int argc, char* argv[]) {
  int N, M;
  int **A;
  double elapsedTime;

  // checking parameters
  if (argc != 2 && argc != 3) {
    printf("Parameters: <N> [<file>]\n");
    return 1;
  }
  N = atoi(argv[1]);
  M = (int) sqrt(N); 
  if(N != M*M){
    printf("N has to be a perfect square!\n");
    exit(1);
  }

  // allocating matrix A
  A = allocMatrix(M);

  // reading files (optional)
  if(argc == 3){
    readMatrixFile(A,M,argv[2]);
  } else {
    srand (time(NULL));
    // Otherwise, generate random matrix.
    for (int i=0; i<M; i++) {
      for (int j=0; j<M; j++) {
	A[i][j] = rand() % MAX_VALUE;
      }
    }
  }
  
  // starting timer
  timerStart();

  // calling shear sort function
  shear_sort(A,M);
  // stopping timer
  elapsedTime = timerStop();

  // print if reasonably small
  if (M <= 10) {
    printMatrix(A,M);
  }
//printf("ShearSort...N=%d\n",N);
  printf("Took %ld ms\n", timerStop());
bool Correct = CheckResult(A,M);
/*if(Correct)printf("---Congrats! The result is correct!\n");
else printf("---Error!\n");*/
  // releasing memory
  for (int i=0; i<M; i++) {
    delete [] A[i];
  }
  delete [] A;

  return 0;
}
