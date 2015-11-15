make
#NODE NUMBER
#PROCESS NUMBER
NP=$1
ibrun -np $NP ./particles 496 test_particles_1.txt
