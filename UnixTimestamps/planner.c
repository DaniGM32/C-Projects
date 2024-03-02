#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

char *print_month_name(int index) {
    char *months[] = {
        "ianuarie",
        "februarie",
        "martie",
        "aprilie",
        "mai",
        "iunie",
        "iulie",
        "august",
        "septembrie",
        "octombrie",
        "noiembrie",
        "decembrie"
    };

    return months[index - 1];
}

int can_attend(Person *people, int index, unsigned event_ts, int oreEveniment) {
    unsigned HOUR = secHour;
    unsigned int event_ts_end = event_ts + oreEveniment * HOUR;
    // printf("Incerc persoana %d | intervalul incepe de la ts = %u pana la ts = %u\n", index, event_ts, event_ts_end);
    for (int i = 0; i < people[index].nrInt; i++) {
        unsigned ts_interval_start = convertDateTimeTZToUnixTimestamp(people[index].Intervale[i]);
        unsigned ts_interval_end = ts_interval_start + people[index].Durate[i] * HOUR;
        // printf("Intervalul %d - %u pana la %u\n", i, ts_interval_start, ts_interval_end);
        if (ts_interval_start <= event_ts && event_ts_end <= ts_interval_end) {
            // printf("Bravo tata\n");
            return 1;
        }
    }
    return 0;
}

void sort_persoane(Person *people, int P) {
    for (int i = 0; i < P; ++i) {
        for (int j = 0; j < P; ++j) {
            if (strcmp(people[i].namePers, people[j].namePers) < 0) {
                Person tmp = people[i];
                people[i] = people[j];
                people[j] = tmp;
            }
        }
    }
}

void findEventInterval(Person *people, int P, int F, int oreEveniment, TTimezone *timezones) {
    // printf("[d] Caut interval pentru %d (%d pers)\n", oreEveniment, F);

    // for (int i = 0; i < P; ++i) {
    //     for (int j = 0; j < people[i].nrInt; ++j) {
    //         printf("%s - %d : %d %d %d %d for %dh (ts=%u), tz = %s(UTC+%d)\n",
    //             people[i].namePers, j,
    //             people[i].Intervale[j].date.year, people[i].Intervale[j].date.month, people[i].Intervale[j].date.day,
    //             people[i].Intervale[j].time.hour, people[i].Durate[j],
    //             convertDateTimeTZToUnixTimestamp(people[i].Intervale[j]),
    //             people[i].Intervale[j].tz->name,
    //             people[i].Intervale[j].tz->utc_hour_difference
    //         );
    //     }
    // }
    unsigned timestampAns = 0;
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < people[i].nrInt; j++) {
            if (people[i].Durate[j] < oreEveniment) {
                continue;
            }
            unsigned timestamp = convertDateTimeTZToUnixTimestamp(people[i].Intervale[j]);
            // printf("incerc intervalul #%d de la pers %d (ts = %u) \n", j, i, timestamp);
            int num_people = 0;
            for (int k = 0; k < P; k++) {
                if (can_attend(people, k, timestamp, oreEveniment)) {
                    num_people++;
                    // printf("%d poate veni\n", k);
                }
                // else printf("%d nu poate veni\n", k);
            }
            if (num_people >= F) {
                if (timestampAns == 0 || timestampAns > timestamp) {
                    timestampAns = timestamp;
                }
            }
        }
    }
    // printf("AM GASIT RASP: %d\n", timestamp_ans);
    if (timestampAns == 0) {
        // daca ajunge aici, nu s a gasit niciun interval adecvat
        printf("imposibil\n");
    } else {
        sort_persoane(people, P);
        for (int i = 0; i < P; i++) {
            if (!can_attend(people, i, timestampAns, oreEveniment)) {
                printf("%s: invalid\n", people[i].namePers);
                continue;
            }
            TDateTimeTZ interval_time = convertUnixTimestampToDateTimeTZ(timestampAns, timezones, people[i].tz_index);
            printf("%s: %02d %s %d, %02d:%02d:%02d %s (UTC%+d)\n",
                people[i].namePers,
                interval_time.date.day, print_month_name(interval_time.date.month), interval_time.date.year,
                interval_time.time.hour, interval_time.time.min, interval_time.time.sec,
                people[i].nameTz,
                timezones[people[i].tz_index].utc_hour_difference);
        }
    }
}

int main() {
    // Task 9
    int T = 0;
    scanf("%d", &T);
    TTimezone *timezones = (TTimezone *)malloc(T * sizeof(TTimezone));
    for (int i = 0; i < T; i++) {
        scanf("%s %hhd", timezones[i].name, &timezones[i].utc_hour_difference);
    }
    int P = 0;
    scanf("%d", &P);
    Person *people = (Person *)malloc(P * sizeof(Person));
    for (int i = 0; i < P; i++) {
        scanf("%s %s %d", people[i].namePers, people[i].nameTz, &people[i].nrInt);
        int nrInterv = people[i].nrInt;
        people[i].Intervale = (TDateTimeTZ *)malloc(nrInterv * sizeof(TDateTimeTZ));
        people[i].Durate = (int *)malloc(nrInterv * sizeof(int));
        for (int j = 0; j < nrInterv; j++) {
            int year = 0, durata = 0;
            unsigned char month = 0, day = 0, hour = 0;
            scanf("%d %hhd %hhd %hhd %d", &year, &month, &day, &hour, &durata);
            people[i].Intervale[j].date.year = year;
            people[i].Intervale[j].date.month = month;
            people[i].Intervale[j].date.day = day;
            people[i].Intervale[j].time.hour = hour;
            people[i].Durate[j] = durata;
            for (int k = 0; k < T; k++) {
                if (strcmp(people[i].nameTz, timezones[k].name) == 0) {
                    people[i].Intervale[j].tz = &timezones[k];
                    people[i].tz_index = k;
                }
            }
        }
    }
    int F = 0;
    scanf("%d", &F);
    int oreEveniment = 0;
    scanf("%d", &oreEveniment);
    TTimezone firstTimeZone = timezones[0];
    // aduc toate intervalele la acelasi timezone
    for (int i = 0; i < P; i++) {
        if (strcmp(firstTimeZone.name, people[i].nameTz) != 0) {
            int nrInterv = people[i].nrInt;
            // printf("%d \n", people[0].Intervale[0].date.year);
            for (int j = 0; j < nrInterv; j++) {
                // printf("Am schimbat %d %d %d ora %d, tz %s (UTC+%d)",
                //     people[i].Intervale[j].date.day,
                //     people[i].Intervale[j].date.month,
                //     people[i].Intervale[j].date.year,
                //     people[i].Intervale[j].time.hour,
                //     people[i].Intervale[j].tz->name,
                //     people[i].Intervale[j].tz->utc_hour_difference
                // );

                unsigned int timestamp = convertDateTimeTZToUnixTimestamp(people[i].Intervale[j]);
                TDateTimeTZ newDateTime = convertUnixTimestampToDateTimeTZ(timestamp, timezones, 0);
                // newDateTime.tz = people[i].Intervale[j].tz;
                people[i].Intervale[j] = newDateTime;
                // printf(" in %d %d %d ora %d, tz %s (UTC+%d)\n",
                //     people[i].Intervale[j].date.day,
                //     people[i].Intervale[j].date.month,
                //     people[i].Intervale[j].date.year,
                //     people[i].Intervale[j].time.hour,
                //     people[i].Intervale[j].tz->name,
                //     people[i].Intervale[j].tz->utc_hour_difference
                // );
            }
        }
    }
    // printf("merge citirea \n");
    // concatenez intervalele consecutive
    for (int i = 0; i < P; i++) {
        int nrInterv = people[i].nrInt;
        for (int j = 0; j - 1 < nrInterv; j++) {
            if (people[i].Intervale[j].date.day == people[i].Intervale[j + 1].date.day) {
                if (people[i].Intervale[j].time.hour + people[i].Durate[j] == people[i].Intervale[j + 1].time.hour) {
                    people[i].Durate[j] += people[i].Durate[j + 1];
                    // printf("%d %d:%d \n", i, people[i].Intervale[j].time.hour, people[i].Durate[j]);
                    for (int k = j + 1; k < nrInterv - 1; k++) {
                        people[i].Intervale[k] = people[i].Intervale[k + 1];
                    }
                    people[i].nrInt--;
                    nrInterv--;
                    // free(people[i].Intervale);
                    people[i].Intervale = (TDateTimeTZ *)realloc(people[i].Intervale, nrInterv * sizeof(TDateTimeTZ));
                    j--;
                }
            }
        }
    }
    findEventInterval(people, P, F, oreEveniment, timezones);
    for (int i = 0; i < P; i++) {
        free(people[i].Intervale);
        free(people[i].Durate);
    }
    free(timezones);
    free(people);
    return 0;
}
