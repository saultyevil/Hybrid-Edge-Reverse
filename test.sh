make clean
rm reversed-image.pgm
make
mpirun -n 4 bin/edge-reverse images/edge768x768.pgm 1500
open reversed-image.pgm
