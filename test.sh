NUM_CPU_CORES=$(($(nproc) / 2))
NUM_THREADS=$(($NUM_CPU_CORES/ $1))
export OMP_NUM_THREADS=$NUM_THREADS

make
rm reversed-image.pgm

time mpirun -n $1 bin/edge-reverse images/edge768x768.pgm 2000

open reversed-image.pgm
