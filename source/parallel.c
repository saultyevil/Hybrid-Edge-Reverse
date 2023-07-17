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
 * @param image
 * @param rank_image
 * @param num_rows
 * @param num_cols
 * @param num_ranks
 */
void distribute_work(double *image, double **rank_image, int *num_rows, int num_cols, int num_ranks) {
  if ((*num_rows) % num_ranks == 0) {
    const int num_rows_per_rank = (*num_rows) / num_ranks;
    const int elements_per_rank = num_rows_per_rank * num_cols;

    *rank_image = malloc(num_rows_per_rank * num_cols * sizeof(double));
    if (!*rank_image) {
      printf("Failed to allocate memory for `rank_image`\n");
      PMPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    *num_rows = num_rows_per_rank;

    MPI_Scatter(image, elements_per_rank, MPI_DOUBLE, *rank_image, elements_per_rank, MPI_DOUBLE, ROOT_RANK,
                MPI_COMM_WORLD);
  } else {
    printf("MPI_Scatterv\n");
  }
}

/**
 * @brief Gather results back to the root rank
 *
 * @param image
 * @param rank_image
 * @param num_rows
 * @param num_cols
 * @param num_ranks
 */
void gather_work(double *image, double *rank_image, int num_rows, int num_cols, int num_ranks) {
  if (num_rows % num_ranks == 0) {
    MPI_Gather(rank_image, num_rows * num_cols, MPI_DOUBLE, image, num_rows * num_cols, MPI_DOUBLE, ROOT_RANK,
               MPI_COMM_WORLD);
  } else {
    printf("MPI_Gatherv\n");
  }
}
