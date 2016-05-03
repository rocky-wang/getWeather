#ifndef SOCKETCOM_H
#define SOCKETCOM_H

/**
 *  产生一个连接成功的以ip和port为目标的TCP连接
 *
 *  @param ip   ip地址或主机域名
 *  @param port 端口号
 *
 *  @return 连接成功的socket描述符，失败为-1
 */
extern int create_tcp_client(const char *ip,unsigned short port);

extern int socket_sendnbuf(int sockfd,char *buf,int send_count);

#endif

