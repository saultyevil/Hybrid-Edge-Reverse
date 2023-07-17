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
  MPI_Init(&argc, &argv);

  int my_rank, num_ranks;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if (argc != 3) {
    printf("Invalid arguments, expected two arguments: filename, num_iterations\n");
    return EXIT_FAILURE;
  }

  int num_rows, num_cols;
  pgmsize(argv[1], &num_rows, &num_cols);

  double *rank_image = NULL;
  double *image = malloc(num_rows * num_cols * sizeof(*image));
  if (!image) {
    printf("Failed to allocate memory for `image`\n");
    return EXIT_FAILURE;
  }

  pgmread(argv[1], image, num_rows, num_cols);
  distribute_work(image, &rank_image, &num_rows, num_cols, num_ranks);
  edge_reverse(rank_image, num_rows, num_cols, atoi(argv[2]));
  gather_work(image, rank_image, num_rows, num_cols, num_ranks);

  if (my_rank == ROOT_RANK) {
    pgmwrite("reversed-image.pgm", image, num_rows * num_ranks, num_cols);
  }

  free(image);
  free(rank_image);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
