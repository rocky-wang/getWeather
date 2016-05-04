#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "weatherData.h"
#include "cJSON.h"
#include "debug.h"

struct weather_show DayWeathers[NUM_OF_CITY];
int QueryNums = 0;

void init_Day_Weathers()
{
    int i;
    for (i = 0; i < NUM_OF_CITY; i++) {
        memset(&DayWeathers[i], 0, sizeof(struct weather_show));
    }
}

int get_weather_infos(const unsigned char *str)
{
    cJSON *root = NULL;
    cJSON *result = NULL;
    cJSON *st = NULL;
    cJSON *weathers = NULL;
    cJSON *tmp;
    cJSON *wtmp;
    char *status;
    int nums;
    int result_num;
    int i , j;
   
    root = cJSON_Parse((char *)str);
    
    // 获取状态
    st = cJSON_GetObjectItem(root, "status");
    if (st->type != cJSON_String) {
        ERROR("status error info!\n");
        return -1;
    }
    status = st->valuestring; 
    if ( strncmp(status,"success",7) != 0 ) {
        ERROR("No correct Data!\n");
        return -1;
    }
    
    // 获取结果数组
    result = cJSON_GetObjectItem(root, "results");
    if ( result == NULL ) {
        ERROR("Not Found Result item.\n");
        return -1;
    }
    result_num = cJSON_GetArraySize(result);
    if(result_num >= NUM_OF_CITY){
        result_num = NUM_OF_CITY;
    }
    QueryNums = result_num;
    for (j = 0; j < result_num; j++) {
        tmp = cJSON_GetArrayItem(result, j);
        // 填充结果中的城市信息
        snprintf(DayWeathers[j].city, sizeof(DayWeathers[j].city), "%s",cJSON_GetObjectItem(tmp, "currentCity")->valuestring);
        // 填充结果中的PM2.5信息
        snprintf(DayWeathers[j].pm_value, sizeof(DayWeathers[j].pm_value), "%s",cJSON_GetObjectItem(tmp, "pm25")->valuestring);
        
        // 获取未来的天气预报数组
        weathers = cJSON_GetObjectItem(tmp, "weather_data");
        // 获取天气预报的数组，逐条处理填充
        nums = cJSON_GetArraySize(weathers);
        if (nums >= 5) {
            nums = 4;
        }
        DayWeathers[j].weather_nums = nums;
        for (i = 0; i < nums; i++) {
            wtmp = cJSON_GetArrayItem(weathers, i);
            snprintf(DayWeathers[j].wdatas[i].date, sizeof(DayWeathers[j].wdatas[i].date), "%s",cJSON_GetObjectItem(wtmp, "date")->valuestring);
            snprintf(DayWeathers[j].wdatas[i].tempature, sizeof(DayWeathers[j].wdatas[i].tempature), "%s",cJSON_GetObjectItem(wtmp, "temperature")->valuestring);
            snprintf(DayWeathers[j].wdatas[i].weather, sizeof(DayWeathers[j].wdatas[i].weather), "%s",cJSON_GetObjectItem(wtmp, "weather")->valuestring);
            snprintf(DayWeathers[j].wdatas[i].wind, sizeof(DayWeathers[j].wdatas[i].wind), "%s",cJSON_GetObjectItem(wtmp, "wind")->valuestring);
        }
    }
    
    return 0;
}
