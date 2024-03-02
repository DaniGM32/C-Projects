#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

// Task 1
TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    unsigned int time = timestamp;
    time = time % secDay;
    unsigned int Hour = 0, minutes = 0, seconds = 0;
    Hour = time / secHour;
    minutes = (time % secHour) / secMin;
    seconds = time - Hour * secHour - minutes * secMin;
    result.hour = Hour;
    result.min = minutes;
    result.sec = seconds;
    return result;
}

// Task2
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    int time = (int)timestamp;
    int d = 0, m = 0, y = 0;
    int yearsDif = time / secYear1;        //  diferenta de ani dintre 1970 si anul prezent
    int monthTime = time - yearsDif * secYear1;  //  diferenta de luni, practic cate luni ne raman din an
    int monthDif = 0;
    int monthTimeAux = 0;
    for (int i = jan; i <= dec; i++) {
        int monthSec = 0;
        if (i == apr || i == jun || i == sep || i == nov) {
            monthSec = secMon30Days;   //  24*3600*30
        } else if (i == 2) {
            monthSec = secMonFeb;   //  24*3600*28
        } else {
            monthSec = secMon31Days;   //  24*3600*31
        }
        if (monthTime - monthSec >= 0) {
            monthDif++;
            monthTime -= monthSec;
            monthTimeAux += monthSec;
        } else {
            break;
        }
    }
    y = year0 + yearsDif;                //  numarul de ani este dat de partea intreaga a acestei impartiri + 1970
    m = month0 + monthDif;
    d = day0 + (time - yearsDif * secYear1 - monthTimeAux) / secDay;
    if ((m == apr || m == jun || m == sep || m == nov) && d > cst30) {
        d = d - cst30;
        m++;
    } else if (m == feb) {
        if (d > cst28) {
            d = d - cst28;
            m++;
        }
    } else if (d > cst31) {
        d = d - cst31;
        m++;
    }
    result.year = y;
    result.month = m;
    result.day = d;
    return result;
}

// Task 3
TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result, dateWLY;
    unsigned int time = timestamp;
    unsigned int d = 0, m = 0, y = 0;
    dateWLY = convertUnixTimestampToDateWithoutLeapYears(time);
    d = dateWLY.day;
    m = dateWLY.month;
    y = dateWLY.year;
    // int yearsDif = dateWLY.year - year0;
    int nrLeapYears = 0;
    for (int i = year0; i <= dateWLY.year; ++i) {
        if ((i % cst4 == 0 && i % cst100 != 0) || (i % cst400 == 0)) {
            ++nrLeapYears;
        }
    }
    if (((y % cst4 == 0 && y % cst100 != 0) || (y % cst400 == 0)) && m <= 2) {
        --nrLeapYears;
    }
    int mon = dateWLY.month;
    int dayRef = 0;
    if (mon == jan) {
        dayRef = cst30;
    } else {
        mon--;
    }
    if (mon == apr || mon == jun || mon == sep || mon == nov) {
        dayRef = cst30;
    } else if (mon == feb) {
        if ((y % cst4 == 0 && y % cst100 != 0) || (y % cst400 == 0)) {
            dayRef = daysFebLeap;
        } else {
            dayRef = daysFebNLeap;
        }
    } else {
        dayRef = cst31;
    }
    if (dateWLY.day <= nrLeapYears) {
        d = dateWLY.day + dayRef - nrLeapYears;
        if (dateWLY.month == 1) {
            m = dec;
            y--;
        } else {
            m--;
        }
    } else {
        d = dateWLY.day - nrLeapYears;
    }
    result.year = y;
    result.month = m;
    result.day = d;
    return result;
}

// Task 4
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result;
    TTime time = convertUnixTimestampToTime(timestamp);
    TDate date = convertUnixTimestampToDate(timestamp);
    int hour = 0, min = 0, sec = 0, day = 0, month = 0;
    unsigned int year = 0;
    sec = time.sec;
    min = time.min;
    hour = time.hour;
    day = date.day;
    month = date.month;
    year = date.year;
    int hoursDif = timezones[timezone_index].utc_hour_difference;
    if (hour + hoursDif >= nrHoursInOneDay) {
        hour = hour + hoursDif - nrHoursInOneDay;
        day++;
    } else if (hour + hoursDif < 0) {
        hour = hour + hoursDif + nrHoursInOneDay;
        day--;
    } else {
        hour = hour + hoursDif;
    }
    if (day == 0 && month == mar) {
        if ((year % cst4 == 0 && year % cst100 != 0) || (year % cst400 == 0)) {
            day = daysFebLeap;
        } else {
            day = daysFebNLeap;
        }
        month--;
    } else if (day == 0 && (month != apr && month != jun && month != sep && month != nov)) {
        day = cst31;
        month--;
    } else if (day == 0) {
        day = cst30;
        month--;
    }
    if (((month == apr || month == jun || month == sep || month == nov)) && day > cst30) {
        day = day0;
        month++;
    } else if (month == feb && ((year % cst4 == 0 && year % cst100 != 0) || (year % cst400 == 0))) {
        if (day == daysFebNLeap) {
            day = daysFebLeap;
        } else if (day > daysFebNLeap) {
            day = day0;            // prima zi a lunii
            month++;
        }
    } else if (day > cst31) {
        day = day0;
        month++;
    }
    if (month > nrMonthsInOneYear) {
        month = jan;
        year++;
    } else if (month == 0) {
        month = nrMonthsInOneYear;
        year--;
    }
    result.time.sec = sec;
    result.time.min = min;
    result.time.hour = hour;
    result.date.day = day;
    result.date.month = month;
    result.date.year = year;
    result.tz = (timezones + timezone_index);
    return result;
}

// Task 5
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ DateAndTime) {
    unsigned int timestamp1 = 0, timestamp = 0;
    TTimezone *t = DateAndTime.tz;
    int hoursDif = (*t).utc_hour_difference;
    int hour = 0, min = 0, sec = 0, day = 0, month = 0;
    unsigned int year = 0;
    sec = DateAndTime.time.sec;
    min = DateAndTime.time.min;
    hour = DateAndTime.time.hour;
    day = DateAndTime.date.day;
    month = DateAndTime.date.month;
    year = DateAndTime.date.year;
    int cmonth = month - 1;
    while (cmonth > 0) {
        if (cmonth == feb) {
            if ((year % cst4 == 0 && year % cst100 != 0) || (year % cst400 == 0)) {
                timestamp1 += daysFebLeap * secDay;
            } else {
                timestamp1 += daysFebNLeap * secDay;
            }
        } else if (cmonth == apr || cmonth == jun || cmonth == sep || cmonth == nov) {
            timestamp1 += cst30 * secDay;
        } else {
            timestamp1 += cst31 * secDay;
        }
        cmonth--;
    }
    unsigned int cyear = year;
    while (year > year0) {
        if ((year % cst4 == 0 && year % cst100 != 0) || (year % cst400 == 0)) {
            timestamp1 += daysLeapYear * secDay;
        } else {
            timestamp1 += daysYear * secDay;
        }
        year--;
    }
    timestamp1 += day * secDay;
    timestamp1 += hour * secHour;
    timestamp1 += min * secMin;
    timestamp1 += sec;
    timestamp = timestamp1 - secHour * hoursDif - secDay;
    if ((cyear % cst4 == 0 && cyear % cst100 != 0) || (cyear % cst400 == 0)) {
        timestamp -= secDay;
    }
    return timestamp;
}

// Task 6
void printDateTimeTZ(TDateTimeTZ datetimetz) {
    TTimezone *t = datetimetz.tz;
    int hoursDif = (*t).utc_hour_difference;
    int hour = 0, min = 0, sec = 0, day = 0, month = 0;
    unsigned int year = 0;
    sec = datetimetz.time.sec;
    min = datetimetz.time.min;
    hour = datetimetz.time.hour;
    day = datetimetz.date.day;
    month = datetimetz.date.month;
    year = datetimetz.date.year;
    printf("%02d ", day);
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
    printf("%d, ", year);
    printf("%02d:%02d:%02d %s ", hour, min, sec, t->name);
    if (hoursDif >= 0) {
        printf("(UTC+%d)", hoursDif);
    } else {
        printf("(UTC%d)", hoursDif);
    }
}
