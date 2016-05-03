#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <sys/types.h>

/**
 *  发送以HTTP协议
 *
 *  @param sockfd   tcp connect成功的描述符
 *  @param hostname http协议需要的主机名
 *  @param port     连接主机的端口
 *  @param filename 请求主机的服务文件和参数
 *
 *  @return <#return value description#>
 */
int send_http_request(int sockfd,const char *hostname,ushort port,const char *filename);

int recv_http_respond_header(int sockfd);

int recv_json_data(int sockfd);

#endif

