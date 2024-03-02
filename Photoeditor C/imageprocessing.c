#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define CONST0 0
#define CONST1 255

int ***imageGenerator(int N, int M) {
    int ***newImage = (int ***)calloc(N, sizeof(int **));
    for (int i = 0; i < N; i++) {
        newImage[i] = (int **)calloc(M, sizeof(int *));
        for (int j = 0; j < M; j++) {
            newImage[i][j] = (int *)calloc(3, sizeof(int));
        }
    }
    return newImage;
}

// Task 1
int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M/2; j++) {
            for (int k = 0; k < 3; k++) {
                int aux = image[i][j][k];
                image[i][j][k] = image[i][M - j - 1][k];
                image[i][M - j - 1][k] = aux;
            }
        }
    }
    return image;
}

// Task 2
int ***rotate_left(int ***image, int N, int M) {
    int ***A = imageGenerator(M, N);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 3; k++) {
                A[i][j][k] = image[j][M - i - 1][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return A;
}

// Task 3
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***A = imageGenerator(h, w);
    int height = 0, width = 0;
    for (int i = y; i < N && height < h; i++, height++) {
        width = 0;
        for (int j = x; j < M && width < w; j++, width++) {
            for (int k = 0; k < 3; k++) {
                A[height][width][k] = image[i][j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return A;
}

// Task 4
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int n = 0, m = 0;
    n = N + 2 * rows;
    m = M + 2 * cols;
    int ***A = (int ***)calloc(n, sizeof(int **));  // Aloc o noua imagine cu n linii si m coloane
    for (int i = 0; i < n; i++) {
        A[i] = (int **)calloc(m, sizeof(int *));
        for (int j = 0; j < m; j++) {
            A[i][j] = (int *)calloc(3, sizeof(int));
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < 3; k++) {
                if (k == 0) {
                    A[i][j][k] = new_R;
                } else if (k == 1) {
                    A[i][j][k] = new_G;
                } else if (k == 2) {
                    A[i][j][k] = new_B;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                A[i + rows][j + cols][k] = image[i][j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return A;
}

// Task 5
int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src, int N_src, int M_src, int x, int y) {
    for (int i = 0; i + y < N_dst && i < N_src; i++) {
        for (int j = 0; j + x < M_dst && j < M_src; j++) {
            for (int k = 0; k < 3; k++) {
                image_dst[i + y][j + x][k] = image_src[i][j][k];
            }
        }
    }
    return image_dst;
}

// Task 6
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***A = imageGenerator(N, M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                A[i][j][k] = image[i][j][k];
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                float aux = 0;
                for (int l = 0; l < filter_size; l++) {
                    for (int t = 0; t < filter_size; t++) {
                        int i_new = i - filter_size / 2 + l;
                        int j_new = j - filter_size / 2 + t;
                        if (i_new >= 0 && i_new < N && j_new >= 0 && j_new < M) {
                            aux += (float)A[i_new][j_new][k] * filter[l][t];
                        }
                    }
                }
                image[i][j][k] = (int)aux;
                if  (image[i][j][k] < CONST0) {
                    image[i][j][k] = CONST0;
                } else if (image[i][j][k] > CONST1) {
                    image[i][j][k] = CONST1;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(A[i][j]);
        }
        free(A[i]);
    }
    free(A);
    return image;
}
