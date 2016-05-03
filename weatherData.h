#ifndef weatherData_h
#define weatherData_h

#include <stdio.h>
struct weather_data{
    char date[64];
    char weather[16];
    char tempature[16];
    char wind[16];
};

struct weather_show{
    char city[16];
    char pm_value[16];
    struct weather_data wdatas[4];
    int weather_nums;
};

#define NUM_OF_CITY     3
extern int get_weather_infos(const unsigned char *str);

extern void init_Day_Weathers();

extern struct weather_show DayWeathers[NUM_OF_CITY];
extern int QueryNums;

#endif /* weatherData_h */
