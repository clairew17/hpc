all:
	icpc shear.cc -o shear -fopenmp -g
	icpc shear.cc -o shear_serial -w -g
	icpc life.cc -o life -fopenmp -g
	icpc life.cc -o life_serial -w -g

mac:
	g++ shear.cc -o shear -fopenmp
	g++ life.cc -o life -fopenmp

clean:
	rm -f life life_serial shear shear_serial
