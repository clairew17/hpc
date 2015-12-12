all:
	mergesort
	icpc shear.cc -o shear -fopenmp -g
	icpc shear.cc -o shear_serial -w -g

mac:
	g++ shear.cc -o shear -fopenmp

mergesort: mergesort.cc
	mpicc -o mergesort mergesort.cc -g

clean:
	rm -f shear shear_serial
	rm -f mergesort
