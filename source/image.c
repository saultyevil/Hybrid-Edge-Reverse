/**
 * @file image.c
 * @author Edward John Parkinson
 * @brief Image processing functions
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
 * @brief  Calculate the 1D index for a 2D array
 *
 * @param row       The row index
 * @param col       The column index
 * @param num_cols  The number of columns in the 2d space
 * @return int      The index the for the 1D array
 */
int index_into_2d(int row, int col, int num_cols) { return row * num_cols + col; }

/**
 * @brief
 *
 * @param edge_image      The image to convert in-place
 * @param num_rows        The number of rows of pixels
 * @param num_cols        The number of columns of pixels
 * @param num_iterations  The max number of iterations to use
 */
void edge_reverse(double *edge_image, int num_rows, int num_cols, int num_iterations) {
  const int buffer_rows = num_rows + 2;
  const int buffer_cols = num_cols + 2;

  double *old_image = malloc(buffer_rows * buffer_cols * sizeof(*old_image));
  if (!old_image) {
    printf("Failed to allocate memory for `old_image`");
    exit(EXIT_FAILURE);
  }
  double *new_image = malloc(buffer_rows * buffer_cols * sizeof(*new_image));
  if (!new_image) {
    printf("Failed to allocate memory for `new_image`");
    exit(EXIT_FAILURE);
  }

  /* Initialise first guest as being completely white */
  for (int i = 0; i < buffer_rows * buffer_cols; ++i) {
    old_image[i] = 255.0;
  }

  /* Iterative calculation */
  for (int n = 0; n < num_iterations; ++n) {
    /* Convert a pixel's edge image back to the image */
    for (int i = 1; i < num_rows + 1; ++i) {
      for (int j = 1; j < num_cols + 1; ++j) {
        new_image[index_into_2d(i, j, buffer_cols)] =
            0.25 * (old_image[index_into_2d(i - 1, j, buffer_cols)] + old_image[index_into_2d(i + 1, j, buffer_cols)] +
                    old_image[index_into_2d(i, j - 1, buffer_cols)] + old_image[index_into_2d(i, j + 1, buffer_cols)] -
                    edge_image[index_into_2d(i - 1, j - 1, num_cols)]);
      }
    }
    /* Copy new into old for next iteration - if we did in the previous step,
       then this will interfere with iterations for other pixels */
    for (int i = 1; i < num_rows + 1; ++i) {
      for (int j = 1; j < num_cols + 1; ++j) {
        old_image[index_into_2d(i, j, buffer_cols)] = new_image[index_into_2d(i, j, buffer_cols)];
      }
    }
  }

  /* Copy final processed image into edge_image */
  for (int i = 1; i < num_rows + 1; ++i) {
    for (int j = 1; j < num_cols + 1; ++j) {
      edge_image[index_into_2d(i - 1, j - 1, num_cols)] = new_image[(index_into_2d(i, j, buffer_cols))];
    }
  }

  free(old_image);
  free(new_image);
}

void create_halo_type(MPI_Datatype *type) {}

void communicate_halo(MPI_Datatype *halo_type, double *image) {}
