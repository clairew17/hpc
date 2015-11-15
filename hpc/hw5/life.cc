/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD (bmills@cs.pitt.edu)
 * Students:
 * Implement openmp verions of conway's game of life.
 */

#include "timer.h"
#include "io.h"
#define dead 0
#define live 1
#define thread_count 3
#ifdef _OPENMP
#include<omp.h>
#endif

// Function implementing Conway's Game of Life
void conway(int **World, int N, int M){
  // STUDENT: IMPLEMENT THE GAME HERE, make it parallel!
  int i, curgen;
	for(curgen=0;curgen<M;curgen++){
		int a,b,c,d,e,f,g,h; // neibourghs 
		int countNeighbor;
		#pragma omp parallel for\
		default(none) shared(World, N) private(a,b,c,d,e,f,g,h,countNeighbor)	
		for(i=0;i<N;i++){
		for(int j=0;j<N;j++){
		a = (i>0&&j>0)?World[i-1][j-1]:0;
		b = (i>0)?World[i-1][j]:0;
		c = (i>0 && j<N-1)?World[i-1][j+1]:0;
		d = (j<N-1)?World[i][j+1]:0;
		e = (i<N-1 && j <N-1) ? World[i+1][j+1]:0;
		f = (i<N-1)?World[i+1][j]:0;
		g = (i<N-1 && j>0)?World[i+1][j-1]:0;	
		h = (j>0) ? World[i][j-1]:0;
		countNeighbor = a+b+c+d+e+f+g+h;
		if(countNeighbor<2 || countNeighbor>3) World[i][j] = dead;
		if(countNeighbor ==3 && World[i][j]==dead) World[i][j] = live;
		}}
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

// Main method      
int main(int argc, char* argv[]) {
  
int N,M;
  int **World;
  double elapsedTime;

  // checking parameters
  if (argc != 3 && argc != 5) {
    printf("Parameters: <N> <M> [<file>]\n");
    return 1;
  }
  N = atoi(argv[1]);
  M = atoi(argv[2]);

  // allocating matrices
  World = allocMatrix(N);
int **Result = allocMatrix(N);
  // reading files (optional)
  if(argc == 5){
    readMatrixFile(World,N,argv[3]);
    readMatrixFile(Result,N,argv[4]);
	} else {
    // Otherwise, generate two random matrix.
    srand (time(NULL));
    for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
	World[i][j] = rand() % 2;
      }
    }
  }

  // starting timer
  timerStart();
// calling conway's game of life 
  conway(World,N,M);

  // stopping timer

elapsedTime = timerStop();

  //printMatrix(World,N);
  printf("Took %ld ms\n", timerStop());
	if(argc==5){
		int counterr = 0;
		for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(World[i][j] != Result[i][j])counterr++;
		}
		}
		if(counterr != 0){
			printf("\n--- Error!\n");
			//printMatrix(Result,N);
		}else printf("\n---Congrats! Result is correct!\n");
	}
	
  // releasing memory
  for (int i=0; i<N; i++) {
    delete [] Result[i];
    delete [] World[i];
  }
  delete [] World;delete [] Result;
  return 0;
}
