#ifndef TIMELIB_H_INCLUDED
#define TIMELIB_H_INCLUDED
#define jan 1
#define feb 2
#define mar 3
#define apr 4
#define may 5
#define jun 6
#define jul 7
#define aug 8
#define sep 9
#define oct 10
#define nov 11
#define dec 12
#define secYear0 31557600    //  24*3600*365 + 6*3600 - numarul de secunde al unui an in medie
#define secYear1 31536000    //  24*3600*365 - numarul de secunde al unui an "simplu"
#define secMonFeb 2419200    //  24*3600*28
#define secMon30Days 2592000   //  24*3600*30
#define secMon31Days 2678400   //  24*3600*31
#define secDay 86400        //  24*3600 - numarul de secunde al unei zile
#define secHour 3600        //  60*60 - numarul de secunde al unei ore
#define secMin 60           //  60 - numarul de secunde al unui minut
#define year0 1970
#define month0 1
#define day0 1
#define nrHoursInOneDay 24
#define nrMonthsInOneYear 12
#define daysFebLeap 29
#define daysFebNLeap 28
#define daysYear 365
#define daysLeapYear 366
#define cst1 1
#define cst2 2
#define cst4 4
#define cst5 5
#define cst7 7
#define cst8 8
#define cst9 9
#define cst11 11
#define cst28 28
#define cst30 30
#define cst31 31
#define cst32 32
#define cst100 100
#define cst400 400
#define dayMask 0x1F
#define monthMask 0xF
#define yearMask 0x3F

typedef struct {
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
} TTime;

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} TDate;

typedef struct {
    char name[cst5];
    signed char utc_hour_difference;
} TTimezone;

typedef struct {
    TDate date;
    TTime time;
    TTimezone *tz;
} TDateTimeTZ;

typedef struct {
    char namePers[cst11];
    char nameTz[cst5];
    int nrInt;
    int tz_index;
    TDateTimeTZ *Intervale;
    int *Durate;
} Person;

TTime convertUnixTimestampToTime(unsigned int timestamp);
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp);
TDate convertUnixTimestampToDate(unsigned int timestamp);
TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index);
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ);
void printDateTimeTZ(TDateTimeTZ datetimetz);

#endif  // TIMELIB_H_INCLUDED
