#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define cst0 5
#define cst1 10
#define cst2 100

struct imageType {
    int N, M;
    int ***image;
};

struct filterType {
    int N;
    float **filter;
};

int ***allocNewImage(int N, int M) {
    int ***newImage = (int ***)calloc(N, sizeof(int **));
    for (int i = 0; i < N; i++) {
        newImage[i] = (int **)calloc(M, sizeof(int *));
        for (int j = 0; j < M; j++) {
            newImage[i][j] = (int *)calloc(3, sizeof(int));
        }
    }
    return newImage;
}

float **allocNewFilter(int size) {
    float **filter = (float **)calloc(size, sizeof(float *));
    for (int i = 0; i < size; i++) {
        filter[i] = (float *)calloc(size, sizeof(float));
    }
    return filter;
}

// Task 7
int main() {
    char *command = (char *)malloc(cst0 * sizeof(char));    // 5 in loc de 2 ca sa fie Doamne ajuta
    int exit = 1;
    int imageIndex = 0;
    int filterIndex = 0;
    struct imageType *images = calloc(cst1, sizeof(struct imageType));
    struct filterType *filters = calloc(cst1, sizeof(struct filterType));
    while (exit) {
        scanf("%s", command);
        if (strcmp(command, "e") == 0) {
            exit = 0;
        } else if (strcmp(command, "l") == 0) {
            int n = 0, m = 0;
            char *path = (char *)malloc(cst2 * sizeof(char));
            scanf("%d %d %s", &n, &m, path);
            images[imageIndex].N = n;
            images[imageIndex].M = m;
            images[imageIndex].image = allocNewImage(n, m);
            read_from_bmp(images[imageIndex].image, images[imageIndex].N, images[imageIndex].M, path);
            imageIndex++;
            free(path);
        } else if (strcmp(command, "s") == 0) {
            char *path = (char *)malloc(cst2 * sizeof(char));
            int ind = 0;
            scanf("%d %s", &ind, path);
            write_to_bmp(images[ind].image, images[ind].N, images[ind].M, path);
            free(path);
        } else if (strcmp(command, "ah") == 0) {
            int ind = 0;
            scanf("%d", &ind);
            images[ind].image = flip_horizontal(images[ind].image, images[ind].N, images[ind].M);
        } else if (strcmp(command, "ar") == 0) {
            int ind = 0, aux = 0;
            scanf("%d", &ind);
            images[ind].image = rotate_left(images[ind].image, images[ind].N, images[ind].M);
            aux = images[ind].M;
            images[ind].M = images[ind].N;
            images[ind].N = aux;
        } else if (strcmp(command, "ac") == 0) {
            int ind = 0, x = 0, y = 0, w = 0, h = 0;
            scanf("%d %d %d %d %d", &ind, &x, &y, &w, &h);
            images[ind].image = crop(images[ind].image, images[ind].N, images[ind].M, x, y, h, w);
            images[ind].N = h;
            images[ind].M = w;
        } else if (strcmp(command, "ae") == 0) {
            int ind = 0, rows = 0, cols = 0, R = 0, G = 0, B = 0;
            scanf("%d %d %d %d %d %d", &ind, &rows, &cols, &R, &G, &B);
            images[ind].image = extend(images[ind].image, images[ind].N, images[ind].M, rows, cols, R, G, B);
            images[ind].N = images[ind].N + 2 * rows;
            images[ind].M = images[ind].M + 2 * cols;
        } else if (strcmp(command, "ap") == 0) {
            int i_dst = 0, i_src = 0, x = 0, y = 0;
            scanf("%d %d %d %d", &i_dst, &i_src, &x, &y);
            int n_dst = images[i_dst].N;
            int m_dst = images[i_dst].M;
            int n_src = images[i_src].N;
            int m_src = images[i_src].M;
            images[i_dst].image = paste(images[i_dst].image, n_dst, m_dst, images[i_src].image, n_src, m_src, x, y);
        } else if (strcmp(command, "cf") == 0) {
            int size = 0;
            scanf("%d", &size);
            filters[filterIndex].N = size;
            filters[filterIndex].filter = allocNewFilter(size);
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    scanf("%f", &filters[filterIndex].filter[i][j]);
                }
            }
            filterIndex++;
        } else if (strcmp(command, "af") == 0) {
            int ind_img = 0, ind_fil = 0;
            scanf("%d %d", &ind_img, &ind_fil);
            int n = images[ind_img].N;
            int m = images[ind_img].M;
            int size = filters[ind_fil].N;
            images[ind_img].image = apply_filter(images[ind_img].image, n, m, filters[ind_fil].filter, size);
        } else if (strcmp(command, "df") == 0) {
            int index_filter = 0;
            scanf("%d", &index_filter);
            for (int i = 0; i < filters[index_filter].N; i++) {      // Dezaloc filtrul cu indexul index_filter
                free(filters[index_filter].filter[i]);
            }
            free(filters[index_filter].filter);
            for (int i = index_filter; i < filterIndex - 1; i++) {   // Schimb indecsi dupa ce am sters filtrul
                filters[i] = filters[i + 1];
            }
            filterIndex--;
        } else if (strcmp(command, "di") == 0) {
            int index_img = 0;
            scanf("%d", &index_img);
            for (int i = 0; i < images[index_img].N; i++) {
                for (int j = 0; j < images[index_img].M; j++) {
                    free(images[index_img].image[i][j]);
                }
                free(images[index_img].image[i]);
            }
            free(images[index_img].image);
            for (int i = index_img; i < imageIndex - 1; i++) {
                images[i] = images[i + 1];
            }
            imageIndex--;
        }
    }
    for (int i = 0; i < imageIndex; i++) {
        for (int j = 0; j < images[i].N; j++) {
            for (int k = 0; k < images[i].M; k++) {
                free(images[i].image[j][k]);
            }
            free(images[i].image[j]);
        }
        free(images[i].image);
    }
    for (int i = 0; i < filterIndex; i++) {
        for (int j = 0; j < filters[i].N; j++) {
            free(filters[i].filter[j]);
        }
        free(filters[i].filter);
    }
    free(images);
    free(filters);
    free(command);
    return 0;
}
