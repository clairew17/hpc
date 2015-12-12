#include <stdio.h>
#include <stdlib.h>
#include <thrust/sort.h>
#include <thrust/merge.h>

#define NUM_SETS 100000
#define DSIZE 100
typedef int mytype;

// for ascending sorted data
#define cmp(A,B) ((A)<(B))
#define nTPB 512
#define nBLK 128

#include <time.h>
#include <sys/time.h>
#define USECPSEC 1000000ULL

long long dtime_usec(unsigned long long start){

  timeval tv;
  gettimeofday(&tv, 0);
  return ((tv.tv_sec*USECPSEC)+tv.tv_usec)-start;
}

template <typename T>
__host__ __device__ void smerge(const T * __restrict__  a, const T * __restrict__ b, T * __restrict__ c, const unsigned len_a, const unsigned len_b, const unsigned stride_a = 1, const unsigned stride_b = 1, const unsigned stride_c = 1){

  unsigned len_c = len_a+len_b;
  unsigned nc = 0;
  unsigned na = 0;
  unsigned nb = 0;
  unsigned fa = (len_b == 0);
  unsigned fb = (len_a == 0);
  T nxta = a[0];
  T nxtb = b[0];
  while (nc < len_c){
    if (fa)      {c[stride_c*nc++] = nxta; na++; nxta = a[stride_a*na];}
    else if (fb) {c[stride_c*nc++] = nxtb; nb++; nxtb = b[stride_b*nb];}
    else if (cmp(nxta,nxtb)){
      c[stride_c*nc++] = nxta;
      na++;
      if (na == len_a) fb++;
      else nxta = a[stride_a*na];}
    else {
      c[stride_c*nc++] = nxtb;
      nb++;
      if (nb == len_b) fa++;
      else nxtb = b[stride_b*nb];}}
}



template <typename T>
__global__ void rmtest(const T * __restrict__  a, const T * __restrict__ b, T * __restrict__  c, int num_arr, int len){

  int idx=threadIdx.x+blockDim.x*blockIdx.x;

  while (idx < num_arr){
    int sel=idx*len;
    smerge(a+sel, b+sel, c+(2*sel), len, len);
    idx += blockDim.x*gridDim.x;}
}

template <typename T>
__global__ void cmtest(const T * __restrict__ a, const T * __restrict__ b, T * __restrict__ c, int num_arr, int len, int stride_a, int stride_b, int stride_c){
  int idx=threadIdx.x+blockDim.x*blockIdx.x;
  while (idx < num_arr){
    smerge(a+idx, b+idx, c+idx, len, len, stride_a, stride_b, stride_c);
    idx += blockDim.x*gridDim.x;}
}




template <typename T>
int rmvalidate(T *a, T *b, T *c, int num_arr, int len){

  T *vc = (T *)malloc(2*len*sizeof(T));
  for (int i = 0; i < num_arr; i++){
    thrust::merge(a+(i*len), a+((i+1)*len), b+(i*len), b+((i+1)*len), vc);
#ifndef TIMING
    for (int j = 0; j < len*2; j++)
      if (vc[j] != c[(i*2*len)+j]) {printf("rm mismatch i: %d, j: %d, was: %d, should be: %d\n", i, j, c[(i*2*len)+j], vc[j]); return 0;}
#endif
    }
  return 1;
}

template <typename T>
int cmvalidate(const T *c1, const T *c2, int num_arr, int len){
  for (int i = 0; i < num_arr; i++)
    for (int j = 0; j < 2*len; j++)
      if (c1[i*(2*len)+j] != c2[j*(num_arr)+i]) {printf("cm mismatch i: %d, j: %d, was: %d, should be: %d\n", i, j, c2[j*(num_arr)+i], c1[i*(2*len)+j]); return 0;}
  return 1;
}

int main(){


  mytype *h_a, *h_b, *h_c, *d_a, *d_b, *d_c;
  h_a = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype));
  h_b = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype));
  h_c = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype)*2);
  cudaMalloc(&d_a, (DSIZE*NUM_SETS+1)*sizeof(mytype));
  cudaMalloc(&d_b, (DSIZE*NUM_SETS+1)*sizeof(mytype));
  cudaMalloc(&d_c, DSIZE*NUM_SETS*sizeof(mytype)*2);
// test "row-major" storage
  for (int i =0; i<DSIZE*NUM_SETS; i++){
    h_a[i] = rand();
    h_b[i] = rand();}
  thrust::sort(h_a, h_a+DSIZE*NUM_SETS);
  thrust::sort(h_b, h_b+DSIZE*NUM_SETS);
  cudaMemcpy(d_a, h_a, DSIZE*NUM_SETS*sizeof(mytype), cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, h_b, DSIZE*NUM_SETS*sizeof(mytype), cudaMemcpyHostToDevice);
  unsigned long gtime = dtime_usec(0);
  rmtest<<<nBLK, nTPB>>>(d_a, d_b, d_c, NUM_SETS, DSIZE);
  cudaDeviceSynchronize();
  gtime = dtime_usec(gtime);
  cudaMemcpy(h_c, d_c, DSIZE*NUM_SETS*2*sizeof(mytype), cudaMemcpyDeviceToHost);
  unsigned long ctime = dtime_usec(0);
  if (!rmvalidate(h_a, h_b, h_c, NUM_SETS, DSIZE)) {printf("fail!\n"); return 1;}
  ctime = dtime_usec(ctime);
  printf("CPU time: %f, GPU RM time: %f\n", ctime/(float)USECPSEC, gtime/(float)USECPSEC);
// test "col-major" storage
  mytype *ch_a, *ch_b, *ch_c;
  ch_a = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype));
  ch_b = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype));
  ch_c = (mytype *)malloc(DSIZE*NUM_SETS*sizeof(mytype));
  for (int i = 0; i < NUM_SETS; i++)
    for (int j = 0; j < DSIZE; j++){
      ch_a[j*NUM_SETS+i] = h_a[i*DSIZE+j];
      ch_b[j*NUM_SETS+i] = h_b[i*DSIZE+j];}
  cudaMemcpy(d_a, ch_a, DSIZE*NUM_SETS*sizeof(mytype), cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, ch_b, DSIZE*NUM_SETS*sizeof(mytype), cudaMemcpyHostToDevice);
  gtime = dtime_usec(0);
  cmtest<<<nBLK, nTPB>>>(d_a, d_b, d_c, NUM_SETS, DSIZE, NUM_SETS, NUM_SETS, NUM_SETS );
  cudaDeviceSynchronize();
  gtime = dtime_usec(gtime);
  cudaMemcpy(ch_c, d_c, DSIZE*NUM_SETS*2*sizeof(mytype), cudaMemcpyDeviceToHost);
  if (!cmvalidate(h_c, ch_c, NUM_SETS, DSIZE)) {printf("fail!\n"); return 1;}

  printf("GPU CM time: %f\n", gtime/(float)USECPSEC);
  return 0;
}