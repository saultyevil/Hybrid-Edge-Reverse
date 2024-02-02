/**
 * @file parallel.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

/**
 * @brief Distribute work to all ranks using scatter communication
 *
 * @param image       The image buffer to gather into
 * @param rank_image  The image for the rank
 * @param num_rows    The number of rows in `image`, but is updated to be the number of rows in `rank_image`
 * @param num_cols    The number of cols in `rank_image`
 * @param num_ranks   The number of ranks in the communicator
 */
void distribute_work(double *image, double **rank_image, int *num_rows, int num_cols, int num_ranks) {
  if ((*num_rows) % num_ranks != 0) {
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == ROOT_RANK) {
      printf("Unable to evenly split work across ranks\n");
    }

    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  const int num_rows_per_rank = (*num_rows) / num_ranks;
  const int elements_per_rank = num_rows_per_rank * num_cols;
  *rank_image = malloc(elements_per_rank * sizeof(double));
  if (*rank_image == NULL) {
    printf("Failed to allocate memory for `rank_image`\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  *num_rows = num_rows_per_rank;

  MPI_Scatter(image, elements_per_rank, MPI_DOUBLE, *rank_image, elements_per_rank, MPI_DOUBLE, ROOT_RANK,
              MPI_COMM_WORLD);
}

/**
 * @brief Gather results back to the root rank
 *
 * @param image       The image buffer to gather into
 * @param rank_image  The image for the rank
 * @param num_rows    The number of rows in `rank_image`
 * @param num_cols    The number of cols in `rank_image`
 * @param num_ranks   The number of ranks in the communicator
 */
void gather_work(double *image, double *rank_image, int num_rows_per_rank, int num_cols_per_rank) {
  MPI_Gather(rank_image, num_rows_per_rank * num_cols_per_rank, MPI_DOUBLE, image,
             num_rows_per_rank * num_cols_per_rank, MPI_DOUBLE, ROOT_RANK, MPI_COMM_WORLD);
}
