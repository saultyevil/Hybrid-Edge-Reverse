/**
 * @file global.h
 * @author Edward John Parkinson
 * @brief  Global things
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

void pgmsize(char *filename, int *nx, int *ny);
void pgmread(char *filename, void *vx, int nx, int ny);
void pgmwrite(char *filename, void *vx, int nx, int ny);
void edge_reverse(double *edge_image, int nx, int ny, int num_iterations, int my_rank, int num_ranks);
void distribute_work(double *image, double **rank_image, int *num_rows, int num_cols, int num_ranks);
void gather_work(double *image, double *rank_image, int num_rows, int num_cols);

#define ROOT_RANK 0
