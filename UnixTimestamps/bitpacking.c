#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timelib.h"

int main() {
    // Task 7 & 8
    int task = 0;
    scanf("%d", &task);
    if (task == cst7) {
        int N = 0;
        scanf("%d", &N);
        TDate *Dates = (TDate *)malloc(N * sizeof(TDate));
        for (int i = 0; i < N; i++) {
            unsigned int binaryDate = 0;
            scanf("%u", &binaryDate);
            Dates[i].day = binaryDate & dayMask;                          // 5 biți pentru zi 0x1F = 00011111
            Dates[i].month = (binaryDate >> cst5) & monthMask;            // 4 biți pentru lună 0xF = 00001111
            Dates[i].year = year0 + ((binaryDate >> cst9) & yearMask);    // 6 biți pentru an 0x3F = 00111111
        }
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                unsigned int day1 = 0, day2 = 0, month1 = 0, month2 = 0, year1 = 0, year2 = 0;
                day1 = Dates[i].day;
                day2 = Dates[j].day;
                month1 = Dates[i].month;
                month2 = Dates[j].month;
                year1 = Dates[i].year;
                year2 = Dates[j].year;
                if (year1 > year2) {
                    TDate aux = Dates[i];
                    Dates[i] = Dates[j];
                    Dates[j] = aux;
                } else if (year1 == year2 && month1 > month2) {
                    TDate aux = Dates[i];
                    Dates[i] = Dates[j];
                    Dates[j] = aux;
                } else if (year1 == year2 && month1 == month2 && day1 > day2) {
                    TDate aux = Dates[i];
                    Dates[i] = Dates[j];
                    Dates[j] = aux;
                }
            }
        }
        for (int i = 0; i < N; i++) {
            printf("%d ", Dates[i].day);
            int month = Dates[i].month;
            if (month == jan) {
                printf("ianuarie ");
            } else if (month == feb) {
                printf("februarie ");
            } else if (month == mar) {
                printf("martie ");
            } else if (month == apr) {
                printf("aprilie ");
            } else if (month == may) {
                printf("mai ");
            } else if (month == jun) {
                printf("iunie ");
            } else if (month == jul) {
                printf("iulie ");
            } else if (month == aug) {
                printf("august ");
            } else if (month == sep) {
                printf("septembrie ");
            } else if (month == oct) {
                printf("octombrie ");
            } else if (month == nov) {
                printf("noiembrie ");
            } else if (month == dec) {
                printf("decembrie ");
            }
            printf("%d\n", Dates[i].year);
        }
        free(Dates);
    }
    return 0;
}
