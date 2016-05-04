#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socketCom.h"
#include "parser.h"
#include "downloader.h"
#include "http.h"
#include "debug.h"
#include "jsonbuf.h"
#include "weatherData.h"
#include "getAK.h"

int get_weather(const char *url)
{
	int ret;
    int len;
	char hostname[64];
	char filename[MAX_URL_FILENAME];
	ushort port;
	int sockfd;

    // 解析请求URL地址，获取主机名、请求文件名、端口号
	ret = parser_url(url,hostname,filename,&port);
	if(ret < 0){
		ERROR("parser url error!\n");
		return -1;
	}
    // 连接服务器
	sockfd = create_tcp_client(hostname,port);
	if(sockfd < 0){
		ERROR("tcp client sock failed!\n");
		return -1;
	}

    // 发送http协议的请求
	ret = send_http_request(sockfd,hostname,port,filename);
	if(ret < 0){
		ERROR("send http request error!\n");
		return -1;
	}

    PJson = NULL;
	ret = recv_http_respond_header(sockfd);
	if(ret < 0){
		ERROR("recv http request error!\n");
		return -1;
	}
    
    if(PJson != NULL){
        len = recv_json_data(sockfd);
        INFO("the json data len is %d\n",len);
        get_weather_infos(PJson->databuf);
        free_jsonbuf(PJson);
    }
    
    close(sockfd);
	return ret;
}

void show_weather()
{
    int i,j;
    
    for (i = 0; i < QueryNums; i++) {
        printf("the city is %s\n",DayWeathers[i].city);
        printf("the pm2.5 is %s\n",DayWeathers[i].pm_value);
        printf("Date\t\tTEMP.\t\tweather\t\tWind\n");
        for (j = 0; j < DayWeathers[i].weather_nums; j++) {
            printf("%s\t\t%s\t\t%s\t\t%s\n",DayWeathers[i].wdatas[j].date,
                   DayWeathers[i].wdatas[j].tempature,
                   DayWeathers[i].wdatas[j].weather,
                   DayWeathers[i].wdatas[j].wind);
        }
    }
}

char *init_weather_query(const char *city_name,const char *ak_key)
{
    static char buf[256];
    
    snprintf(buf, 256, "http://api.map.baidu.com/telematics/v3/weather?location=%s&output=json&ak=%s",city_name,ak_key);
    
    return buf;
}

int main()
{
    char ak_key[128];
    size_t ak_len = 128;
	int ret;
    char *query_string;
    
    // 只是为了做教学使用而设计的函数，实际开发不需要这样设计
    ret = getAKInfo(NULL, ak_key, &ak_len);
    if (ret < 0) {
        fprintf(stderr, "Can't get AK Key Value!\n");
        return -1;
    }

    query_string = init_weather_query("成都", ak_key);
    init_Day_Weathers();
    
	ret = get_weather(query_string);
	if( ret == -1 ){
		ERROR("http test downloader failed!\n");
		exit(1);
	}

    show_weather();
	INFO("success test downloader!\n");
	return 0;
}

