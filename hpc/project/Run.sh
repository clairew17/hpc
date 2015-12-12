#!/bin/bash

N=4096

sbatch mergesort_64.batch $N
sbatch mergesort_128.batch $N
sbatch mergesort_256.batch $N
sbatch mergesort_512.batch $N
sbatch shear.batch $N

