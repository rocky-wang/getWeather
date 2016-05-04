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
#include "cgic.h"
#include "erro_code.h"

int get_weather(const char *url)
{
	int ret;
	char hostname[64];
	char filename[MAX_URL_FILENAME];
	ushort port;
	int sockfd;

    // 解析请求URL地址，获取主机名、请求文件名、端口号
	ret = parser_url(url,hostname,filename,&port);
	if(ret < 0){
		return ERROR_PROTCAL;
	}
    // 连接服务器
	sockfd = create_tcp_client(hostname,port);
	if(sockfd < 0){
		return ERROR_PROTCAL;
	}

    // 发送http协议的请求
	ret = send_http_request(sockfd,hostname,port,filename);
	if(ret < 0){
		return ERROR_REMOTE;
	}

    PJson = NULL;
	ret = recv_http_respond_header(sockfd);
	if(ret < 0){
		return ERROR_REMOTE;
	}
    
    if(PJson != NULL){
        recv_json_data(sockfd);
        ret = get_weather_infos(PJson->databuf);
        free_jsonbuf(PJson);
    }
    else{
        ret = ERROR_PROTCAL;
    }
    
    close(sockfd);
	return ret;
}

void show_weather()
{
    int i,j;
    
    for (i = 0; i < QueryNums; i++) {
        fprintf(cgiOut,"<h3>the city is %s</h3>\n",DayWeathers[i].city);
        fprintf(cgiOut,"<h3>the pm2.5 is %s</h3>\n",DayWeathers[i].pm_value);
        fprintf(cgiOut,"<h3>Date&nbsp&nbsp&nbsp&nbsp&nbsp&nbspTEMP.&nbsp&nbspweather&nbsp&nbsp&nbspWind</h3>\n");
        for (j = 0; j < DayWeathers[i].weather_nums; j++) {
            fprintf(cgiOut,"<h5>%s&nbsp&nbsp%s&nbsp&nbsp%s&nbsp&nbsp%s</h5>\n",DayWeathers[i].wdatas[j].date,
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

int query_weather(const char *city)
{
    char ak_key[128];
    size_t ak_len = 128;
	int ret;
    char *query_string;
    
    // 只是为了做教学使用而设计的函数，实际开发不需要这样设计
    ret = getAKInfo(NULL, ak_key, &ak_len);
    if (ret < 0) {
        return ERROR_NOCONF;
    }

    query_string = init_weather_query(city, ak_key);
    init_Day_Weathers();
    
	ret = get_weather(query_string);

	return ret;
}

extern char *cgiQueryString;

int cgiMain()
{
    char *buf;
    char city[32];
    int ret;
    
    cgiHeaderContentType("text/html");
    
    fprintf(cgiOut, "<html><head>\n");
    fprintf(cgiOut, "<meta charset=\"UTF-8\">");
    fprintf(cgiOut, "<title>天气预报</title></head>\n");
    
    cgiUnescapeChars(&buf,cgiQueryString,(int)strlen(cgiQueryString));
    cgiFormString("input_city" ,city, sizeof(city));
    ret = query_weather(city);
    switch (ret) {
        case ERROR_UNVALID_AK:
            fprintf(cgiOut, "<h2>无效的AK请求值！</h2>\n");
            break;
        case ERROR_UNCITY_NAME:
            fprintf(cgiOut, "<h2>查询不到%s城市信息！</h2>\n",city);
            break;
        case ERROR_REMOTE:
            fprintf(cgiOut, "<h2>服务器错误，请稍后再试！</h2>\n");
            break;
        case ERROR_NOCONF:
            fprintf(cgiOut, "<h2>没有找到AK值的配置文件！</h2>\n");
            break;
        case ERROR_PROTCAL:
            fprintf(cgiOut, "<h2>内部协议错误！</h2>\n");
            break;
        case 0:
            show_weather();
            break;
        default:
            fprintf(cgiOut, "<h2>未定义错误！</h2>\n");
            break;
    }
    
    fprintf(cgiOut, "</body>");
    return 0;
}

