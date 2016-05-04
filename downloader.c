#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "http.h"
#include "parser.h"
#include "socketCom.h"
#include "debug.h"
#include "jsonbuf.h"

int send_http_request(int sockfd,const char *hostname,ushort port,const char *filename)
{
	struct http_request_header *request;
	int ret;

	request = create_http_header();

	init_get_request(request,filename);

	add_host_header(request,hostname,port);
	add_request(request,"Accept: ","*/*");
	add_request(request,"Connection: ","Close");

	end_request(request);

	ret = socket_sendnbuf(sockfd,request->request_buf,request->offset);
	if(ret < 0){
		close(sockfd);
		release_http_header(request);
		return -1;
	}

	release_http_header(request);

	return 0;
}

struct jsonbuf* init_to_jsonbuf(struct http_respond_header *head,unsigned long len)
{
    struct jsonbuf *p;
    
    p = create_jsonbuf(len);
    if ( p == NULL) {
        return NULL;
    }

    memcpy(p->databuf,head->respond_buf + head->head_part_index,head->offset - head->head_part_index);

    p->offset = (int)(head->offset - head->head_part_index);
    
	return p;
}

int recv_json_data(int sockfd)
{
    ssize_t ret;
    char buf[1024];
    
    ret = recv(sockfd, buf, 1024, 0);
    while(ret){
        if (ret < 0) {
            perror("recv data");
            return -1;
        }
        memcpy(PJson->databuf + PJson->offset, buf, ret);
        PJson->offset += ret;
        ret = recv(sockfd, buf, 1024, 0);
    }
    PJson->databuf[PJson->offset] = 0;
    return PJson->offset;
}

int recv_http_respond_header(int sockfd)
{
	struct http_respond_header *head;
    unsigned long data_len;
	int ret;
	int code;

	head = create_respond_header();
	if(head == NULL){
		ERROR("create respond header error!\n");
		return -1;
	}

	ret = get_respond_header(sockfd,head);
	if(ret < 0){
		ERROR("Not Found respond header!\n");
		return -1;
	}

	code = get_http_status(head->respond_buf);
	if(code != 200){
		ERROR("Not support the code!\n");
		return -1;
	}
    
    data_len = get_http_length(head->respond_buf);
    if ( data_len > 0 ) {
        PJson = init_to_jsonbuf(head, data_len);
    }
    
	release_respond_header(head);

	return 0;
}

