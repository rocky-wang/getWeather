#ifndef PARSER_H
#define PARSER_H
#include <sys/types.h>

#define MAX_URL_FILENAME  256
/***
 * @abstract : 根据请求以http://为前缀的URL地址反馈对应的数据。
 * @param url : 被解析的URL地址
 * @param hostname : 解析出来的主机名
 * @param filename : 请求服务的文件名路径及参数
 * @param port : 主机公开的端口号
 */
extern int parser_url(const char *url,char *hostname,char *filename,ushort *port);

/**
 *  获取http回应头中content-lenght中的大小
 *
 *  @param respond http回应头
 *
 *  @return 获取的文件大小
 */
extern unsigned long get_http_length(const char *respond);

#endif