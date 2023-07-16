/**
 * @file main.c
 * @author Edward John Parkinson
 * @brief Entry point of the program
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

/**
 * @brief Main function of the program.
 *
 * @param argc  Number of arguments provided
 * @param argv  The argument vector
 */
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Invalid arguments, expected two arguments: filename, num_iterations\n");
    return EXIT_FAILURE;
  }

  double *image;
  int num_rows, num_cols;
  pgmsize(argv[1], &num_rows, &num_cols);

  image = malloc(num_rows * num_cols * sizeof(*image));
  if (!image) {
    printf("Failed to allocate memory for `image`");
    return EXIT_FAILURE;
  }

  pgmread(argv[1], image, num_rows, num_cols);
  edge_reverse(image, num_rows, num_cols, atoi(argv[2]));

  pgmwrite("reversed-image.pgm", image, num_rows, num_cols);
  free(image);

  return EXIT_SUCCESS;
}
