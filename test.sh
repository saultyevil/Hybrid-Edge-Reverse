make clean
rm reversed-image.pgm
make
bin/edge-reverse images/edge768x768.pgm 1500
display reversed-image.pgm
