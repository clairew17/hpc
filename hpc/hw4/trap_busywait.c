/**
 * University of Pittsburgh
 * Department of Computer Science
 * CS1645: Introduction to HPC Systems
 * Instructor Bryan Mills, PhD
 * Student: 
 * Implement Pthreads version of trapezoidal approximation.
 */

#include <stdio.h>
#include "timer.h"

//#ifdef _OPENMP
#include <omp.h>
//#endif
#include <pthread.h>
// Global variables to make coverting to pthreads easier :)
double a;
double b;
long long int n = 1800000000;
double approx = 0.0;
pthread_mutex_t M1;
int flag = 0;
static bool local_sum = false;
// Actual areas under the f(x) = x^2 curves, for you to check your
// values against.
double static NEG_1_TO_POS_1 = 0.66666666666667;
double static ZERO_TO_POS_10 = 333.333;

// f function is defined a x^2
double f(double a) {
  return a * a;
}

// Serial implementation of trapezoidal approximation. You should
// refactor the loop in this function to be parallized using pthread.
void trap() {
  double h = (b-a) / n;
  approx = ( f(a) - f(b) ) / 2.0;
  for(int i = 1; i < n-1; i++) {
    double x_i = a + i*h;
    approx += f(x_i);
  }
  approx = h*approx;
}
void trap_busywait(){
printf("Busy waiting");
if(local_sum)printf("\t with local sum...");
printf("\n");	
	double my_result;
	#ifdef _OPENMP
		int my_rank=omp_get_thread_num();
		int thread_count = omp_get_num_threads();
	#else
		int my_rank = 0;
		int thread_count = 1;
	#endif
	double h = (b-a)/n;
	int local_n = n/thread_count;
	double local_a = a + my_rank*local_n*h;
	double local_b = local_a + local_n*h;
	my_result = (f(local_a)+f(local_b))/2.0;
	for(int i = 1; i < local_n-1; i++) {
    		double x_i = local_a + i*h;
    		if(local_sum){
		my_result += f(x_i);
		}else{
		while(flag != my_rank);
		approx += f(x_i)*h;
		flag = (flag+1) % thread_count;
		}
}
if(local_sum){
while(flag != my_rank);
approx += my_result*h;
flag = (flag+1) % thread_count;
}
/*#pragma omp critical
	{
		approx += my_result*h;
	}
*/
}
void trap_mutex(){
printf("Mutex");
if(local_sum)printf("\t with local sum...");
printf("\n");	
double my_result;
	#ifdef _OPENMP
		int my_rank=omp_get_thread_num();
		int thread_count = omp_get_num_threads();
	#else
		int my_rank = 0;
		int thread_count = 1;
	#endif
	double h = (b-a)/n;
	int local_n = n/thread_count;
	double local_a = a + my_rank*local_n*h;
	double local_b = local_a + local_n*h;
	my_result = (f(local_a)+f(local_b))/2.0;
	for(int i = 1; i < local_n-1; i++) {
    		double x_i = local_a + i*h;
		if(local_sum){
		my_result += f(x_i);
		}else{
		pthread_mutex_lock(&M1);
		approx += f(x_i)*h;
		pthread_mutex_unlock(&M1);
    		}
  	}
if(local_sum){
pthread_mutex_lock(&M1);
approx += my_result*h;
pthread_mutex_unlock(&M1);
}
}


int main(int argc, char * argv[]) {
  // Example 1 [-1,1]
int thread_count = atoi(argv[1]);
local_sum = bool(atoi(argv[2]));
long timeS,timeP;
approx = 0.0;
printf("\n**************************************\n\nExample 1\n");
  a = -1.0;
  b = 1.0;
  //n = 2000000000;
timerStart();
trap();
timeS = timerStop();
  printf("Took %ld ms\n", timeS);
  printf("a:%f\t b:%f\t n:%ld\t actual:%f\t approximation:%f\n", a, b, n, NEG_1_TO_POS_1, approx);

approx =0.0;
timerStart();
//printf("\nThreadCount=%d",thread_count);
#pragma omp parallel num_threads(thread_count)
{
//trap_mutex();
trap_busywait();
}
timeP = timerStop();
printf("Took %ld ms\n", timeP);
printf("a:%f\t b:%f\t n:%ld\t actual:%f\t approximation:%f\terror:%f\n", a, b, n,NEG_1_TO_POS_1, approx, NEG_1_TO_POS_1-approx);
printf(" --- ThreadCount:%d\tSpeedUp:%f\terror:%f\n",thread_count,timeS*1.0/timeP, NEG_1_TO_POS_1-approx);

approx = 0.0;
printf("\n**************************************\n\nExample 2\n");
  // Example 2 [0,10]
  a = 0.0;
  b = 10.0;
  //n = 1000000000;
timerStart();
trap();
timeS = timerStop();
  printf("Took %ld ms\n", timeS);
  printf("a:%f\t b:%f\t n:%ld\t actual:%f\t approximation:%f\n", a, b, n, ZERO_TO_POS_10, approx);

approx = 0.0;
timerStart();
#pragma omp parallel num_threads(thread_count)
{
//trap_mutex();
trap_busywait();
}
timeP = timerStop();
printf("Took %ld ms\n", timeP);
printf("a:%f\t b:%f\t n:%ld\t actual:%f\t approximation:%f\terror:%f\n", a, b, n, ZERO_TO_POS_10, approx, ZERO_TO_POS_10-approx);
printf("ThreadCount:%d\tSpeedUp:%f\n",thread_count,timeS*1.0/timeP);
printf(" --- ThreadCount:%d\tSpeedUp:%f\terror:%f\n",thread_count,timeS*1.0/timeP, ZERO_TO_POS_10-approx);

return 0;
}
