/* 
 * trans.c - Natrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part I of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]){
int row, col, i, j, val1, val2, val3, val4, val5, val6, val7, val8;
 if (M == 32 && N == 32){
 	for (row = 0; row < M; row+=8) {
            for (col = 0; col < N; col+=8) {
                for (i = row; i < row+8; i++){
                    val1 = A[i][col];
                    val2 = A[i][col + 1];
                    val3 = A[i][col + 2];
                    val4 = A[i][col + 3];
                    val5 = A[i][col + 4];
                    val6 = A[i][col + 5];
                    val7 = A[i][col + 6];
                    val8 = A[i][col + 7];
                    B[col][i] = val1;
                    B[col + 1][i] = val2;
                    B[col + 2][i] = val3;
                    B[col + 3][i] = val4;
                    B[col + 4][i] = val5;
                    B[col + 5][i] = val6;
                    B[col + 6][i] = val7;
                    B[col + 7][i] = val8;
                }
            }
        }    
    } else if (M == 64 && N == 64){
        for (row = 0; row < M; row+=4) {
            for (col = 0; col < N; col+=4) {
                for (i = row; i < row+4; i++){
                    val1 = A[i][col];
                    val2 = A[i][col + 1];
                    val3 = A[i][col + 2];
                    val4 = A[i][col + 3];
                    B[col][i] = val1;
                    B[col + 1][i] = val2;
                    B[col + 2][i] = val3;
                    B[col + 3][i] = val4;
                }
            }
        }    
    } else if(M == 67 && N == 61) {
    for(row = 0; row < M; row += 16) {
      for(col = 0; col < N; col += 16) {
	for(i = row; (i < row + 16) && (i < M); i++) {
	  for(j = col; (j < col + 16) && (j < N); j++) {
	    if(i < 67 && j < 61) {
	      A[i][j] = B[j][i];
	    }
	  }
	}
      }
    }
  }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M])
{
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M])
{
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

