/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Student: 
 * Instructor: Bryan Mills, University of Pittsburgh
 * MPI particle-interaction code. 
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 1
#define TAG 7
#define CONSTANT 777

// Particle-interaction constants
#define A 10250000.0
#define B 726515000.5
#define MASS 0.1
#define DELTA 1

// Random initialization constants
#define POSITION 0
#define VELOCITY 1

// Structure for shared properties of a particle (to be included in messages)
struct Particle{
  float x;
  float y;
  float mass;
  float fx;
  float fy;
};
double compt_num = 0;
// Headers for auxiliar functions
float random_value(int type);
void print_particles(struct Particle *particles, struct Particle *result,int n);
void interact(struct Particle *source, struct Particle *destination);
void compute_interaction(struct Particle *source, struct Particle *destination, int limit);
void compute_self_interaction(struct Particle *set, int size);
void merge(struct Particle *first, struct Particle *second, int limit);

// Main function
main(int argc, char** argv){
  int myRank;// Rank of process
  int p;// Number of processes
  int n;// Number of total particles
  int previous;// Previous rank in the ring
  int next;// Next rank in the ring
  int tag = TAG;// Tag for message
  int number;// Number of local particles
  struct Particle *globals, *results;// Array of all particles in the system
  struct Particle *locals;// Array of local particles
  struct Particle *remotes;// Array of foreign particles
  char *file_name;// File name
MPI_Request request;  
MPI_Status status;// Return status for receive
  int i, j, rounds, initiator, sender;
  double start_time, end_time;
int recv_num=0, send_num=0;
int ind;  
// checking the number of parameters
  if(argc < 2){
    printf("ERROR: Not enough parameters\n");
    printf("Usage: %s <number of particles> [<file>]\n", argv[0]);
    exit(1);
  }
  
  // getting number of particles
  n = atoi(argv[1]);

  // initializing MPI structures and checking p is odd
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if(p % 2 == 0){
    p = p - 1;
    if(myRank == p){
      MPI_Finalize();
      return 0;
    }
  }
  srand(myRank+myRank*CONSTANT);

  // acquiring memory for particle arrays
  number = floor(n / p) + 1;
// number of data per process
int *data_per_proc = (int*)malloc(p);
int *sendcount = (int*)malloc(p);
int *displs = (int*)malloc(p);
for(i=0;i<p;i++){
	if(i==p-1){
		data_per_proc[i] = n%p;
	}else{
		data_per_proc[i]=number;
	}
	sendcount[i] = data_per_proc[i]* (sizeof(struct Particle))/sizeof(float);
	if(i==0)displs[i] = 0;
	else displs[i] = displs[i-1]+sendcount[i-1];
}

locals = (struct Particle *) malloc(number * sizeof(struct Particle));
  remotes = (struct Particle *) malloc(number * sizeof(struct Particle));

  // checking for file information
  if(argc == 3){
if(myRank == 0){
      globals = (struct Particle *) malloc(n * sizeof(struct Particle));
      results = (struct Particle *) malloc(n * sizeof(struct Particle));

//******************** YOUR CODE GOES HERE (reading particles from file)
	  FILE *ifp, *ofp;
	  char *mode = "r";
	  ifp = fopen(argv[2], mode);

	  if (ifp == NULL) {
	    fprintf(stderr, "Can't open input file!\n");
	    return 1;
	  }

	  // reading particle values
	  for(i=0; i<n; i++){
	    fscanf(ifp, "%f\t%f\t%f", &globals[i].x, &globals[i].y, &globals[i].mass);
	    globals[i].fx = 0.0;
	    globals[i].fy = 0.0;
	  }
	fclose(ifp);
	
	ifp=fopen("test_results_1.txt",mode);
	  if (ifp == NULL) {
	    fprintf(stderr, "Can't open input file!\n");
	    return 1;
	  }

	  // reading particle values
	  for(i=0; i<n; i++){
	    fscanf(ifp, "%d\t%f\t%f\t%f\t%f\t%f", &ind,&results[i].x, &results[i].y, &results[i].mass, &results[i].fx, &results[i].fy);
	}
	fclose(ifp);


    }

// ************* GET NEXT_RANK AND PREVIOUS RANK
previous=(myRank-1)%p;
next=(myRank+1)%p;

printf("rank=%d\tdatanum=%d\n",myRank,data_per_proc[myRank]);
 
    // To send/recv (or scatter/gather) you will need to learn how to
    // transfer structs of floats, treat it as a contiguous block of
    // floats. Here is an example:
    // MPI_Send(locals,
    //          number * (sizeof (struct Particle)) / sizeof(float),
    //          MPI_FLOAT,
    //          next_rank,
    //          tag,
    //          MPI_COMM_WORLD)
    // MPI_Recv(remotes,
    //          number * (sizeof (struct Particle)) / sizeof(float),
    //          MPI_FLOAT,
    //          previous_rank,
    //          tag,
    //          MPI_COMM_WORLD,
    //          &status);
    // hint: because your nodes need to both send and receive you
    // might consider asyncronous send/recv.

//*********************** YOUR CODE GOES HERE (distributing particles among processors) 
//printf("Broadcast initial data to processors...\n");

MPI_Scatterv(globals,//SEND_BUF
sendcount,//data_per_proc*(sizeof (struct Particle)) / sizeof(float),
displs,
MPI_FLOAT,
locals,//recv_buf
sendcount[myRank],//number*(sizeof (struct Particle)) / sizeof(float),//recv_count
MPI_FLOAT,
0,//SOURCE_PROCESSOR
MPI_COMM_WORLD);

//recv_num++;
//printf("myRank=%d\trecv_num=%d\n...",myRank,recv_num);

  } else {
    // random initialization of local particle array
    for(j = 0; j < number; j++){
      locals[j].x = random_value(POSITION);
      locals[j].y = random_value(POSITION);
      locals[j].fx = 0.0;
      locals[j].fy = 0.0;
      locals[j].mass = MASS;
    }
  }
  
  // starting timer
  if(myRank == 0){
    start_time = MPI_Wtime();
  }
//*********************** YOUR CODE GOES HERE (ring algorithm)
next = (myRank+1)%p;
	MPI_Isend(locals,
              number * (sizeof (struct Particle)) / sizeof(float),
              MPI_FLOAT,
              next, //dest
              myRank,//myRank,as tag
              MPI_COMM_WORLD, &request);
send_num++;
//if(DEBUG)printf("%d send to %d...send_num=%d\n",myRank,next,send_num);

// ****************** RING
previous = (myRank-1+p)%p;
for(i=0;i<(p-1)/2;i++){

// ************ receive 	
     MPI_Irecv(remotes,
              number * (sizeof (struct Particle)) / sizeof(float),
              MPI_FLOAT,
              previous,//source
              MPI_ANY_TAG,//(myRank-i-1+p)%p, // home of remotes
              MPI_COMM_WORLD,
              &request);

MPI_Wait(&request, &status);
recv_num++;
initiator = status.MPI_TAG;
if(DEBUG)printf("------ %d receive from %d...homeOfRemote=%d\n",myRank,previous,initiator);

//calculate force
compute_interaction(locals, remotes, number);

//pass remote
	//send to original processor if the last step
	if(i==(p-1)/2-1){
	next=initiator;//(myRank-i-1+p)%p;
	printf("%d --> %d...\n",myRank,next);
	}else next=(myRank+1+p)%p;
	
	MPI_Isend(remotes,
              number * (sizeof (struct Particle)) / sizeof(float),
              MPI_FLOAT,
              next, //dest
              initiator,//(myRank-i-1+p)%p,//tag
              MPI_COMM_WORLD, &request);
}
//printf("myRank=%d\trecv_num=%d\n...",myRank,recv_num);

//original processor receive nodes
     MPI_Irecv(remotes,
              number * (sizeof (struct Particle)) / sizeof(float),
              MPI_FLOAT,
              MPI_ANY_SOURCE,//source
              myRank, // home of remotes
              MPI_COMM_WORLD,
              &request);

//prinft("%d Collect \n");
MPI_Wait(&request, &status);
if(DEBUG)printf("%d collected from...%d, tag=%d\n",myRank,status.MPI_SOURCE,status.MPI_TAG);
if(DEBUG && status.MPI_TAG!=myRank)printf("**** Wrong!!!%d collected from...%d, tag=%d\n",myRank,status.MPI_SOURCE,status.MPI_TAG);
compute_self_interaction(locals, number);
// merge
merge(locals, remotes, number);

  // stopping timer
  if(myRank == 0){
    end_time = MPI_Wtime();
    printf("Duration: %f seconds\n", (end_time-start_time));
  }
  
  // printing information on particles
  if(argc == 3){
    
//********************** YOUR CODE GOES HERE (collect particles at rank 0)


MPI_Gatherv(locals,//SEND_BUF
sendcount[myRank],//number*(sizeof (struct Particle)) / sizeof(float),
MPI_FLOAT,
globals,//recv_buf
sendcount,//number*(sizeof (struct Particle)) / sizeof(float),//recv_count
displs,
MPI_FLOAT,
0,//dest_PROCESSOR
MPI_COMM_WORLD);


    if(myRank == 0) {
      print_particles(globals,results,n);
    }
  }


  // finalizing MPI structures
  MPI_Finalize();
//printf("Total compt_num=%f\n",compt_num);
free(data_per_proc);
free(displs);
free(sendcount);
}

// Function for random value generation
float random_value(int type){
  float value;
  switch(type){
  case POSITION:
    value = (float)rand() / (float)RAND_MAX * 100.0;
    break;
  case VELOCITY:
    value = (float)rand() / (float)RAND_MAX * 10.0;
    break;
  default:
    value = 1.1;
  }
  return value;
}

// Function for printing out the particle array
void print_particles(struct Particle *particles, struct Particle *result, int n){
  int j;
  printf("Index\tx\ty\tmass\tfx\tfy\n");
  for(j = 0; j < n; j++){
    if(DEBUG){
	printf("ERR\t%d\t%f\t%f\t%f\t%f\t%f(--err)\n",j,particles[j].x,particles[j].y,particles[j].mass,particles[j].fx-result[j].fx,particles[j].fy-result[j].fy);
	printf("Orig\t%d\t%f\t%f\t%f\t%f\t%f\n",j,particles[j].x,particles[j].y,particles[j].mass,particles[j].fx,particles[j].fy);
	printf("Ref\t%d\t%f\t%f\t%f\t%f\t%f\n",j,result[j].x,result[j].y,result[j].mass,result[j].fx,result[j].fy);
    }else
	printf("%d\t%f\t%f\t%f\t%f\t%f\n",j,particles[j].x,particles[j].y,particles[j].mass,particles[j].fx,particles[j].fy);
  }
}

// Function for computing interaction among two particles
// There is an extra test for interaction of identical particles, in which case there is no effect over the destination
void interact(struct Particle *first, struct Particle *second){
  float rx,ry,r,fx,fy,f;

  // computing base values
  rx = first->x - second->x;
  ry = first->y - second->y;
  r = sqrt(rx*rx + ry*ry);

  if(r == 0.0)
    return;

  f = A / pow(r,6) - B / pow(r,12);
  fx = f * rx / r;
  fy = f * ry / r;

  // updating sources's structure
  first->fx = first->fx + fx;
  first->fy = first->fy + fy;
  
  // updating destination's structure
  second->fx = second->fx - fx;
  second->fy = second->fy - fy;

}

// Function for computing interaction between two sets of particles
void compute_interaction(struct Particle *first, struct Particle *second, int limit){
  int j,k;
 compt_num++;
 
  for(j = 0; j < limit; j++){
    for(k = 0; k < limit; k++){
      interact(&first[j],&second[k]);
    }
  }
}

// Function for computing interaction between two sets of particles
void compute_self_interaction(struct Particle *set, int size){
  int j,k;
  
  for(j = 0; j < size; j++){
    for(k = j+1; k < size; k++){
      interact(&set[j],&set[k]);
    }
  }
}

// Function to merge two particle arrays
// Permanent changes reside only in first array
void merge(struct Particle *first, struct Particle *second, int limit){
  int j;
  
  for(j = 0; j < limit; j++){
    first[j].fx += second[j].fx;
    first[j].fy += second[j].fy;
  }
}

// Reads particle information from a text file
int read_file(struct Particle *set, int size, char *file_name){
  FILE *ifp, *ofp;
  char *mode = "r";
int i;  
ifp = fopen(file_name, mode);

  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file!\n");
    return 1;
  }

  // reading particle values
  for(i=0; i<size; i++){
    fscanf(ifp, "%f\t%f\t%f", &set[i].x, &set[i].y, &set[i].mass);
    set[i].fx = 0.0;
    set[i].fy = 0.0;
  }
  
  // closing file
  fclose(ifp);

  return 0;
}

