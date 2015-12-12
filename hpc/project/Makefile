all:
	icpc shear.cc -o shear -fopenmp -g
	icpc shear.cc -o shear_serial -w -g

mac:
	g++ shear.cc -o shear -fopenmp

clean:
	rm -f shear shear_serial
