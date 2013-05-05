#include <stdio.h>
#include <Winsock2.h>
#include <string.h>
#define    WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "spider.h"

#include "easy.h"

#pragma comment(lib, "ws2_32.lib")

#define BUF_LEN 1024	// 1k

/*
 * ͨ��url��ȡ��ҳ����
 * @url : uniform resource locator
 * @page_data : ��url�ڵ�����
 */
int get_url_page(const char* url, char* page_data, int page_size)
{
	WSADATA wsa_data={0}; 
	SOCKET sockfd; 
	struct sockaddr_in addr; 
	struct hostent *pURL; 
	char url_buf[BUF_LEN] = {0}; 
	char *pHost = 0;
	char* pGET = 0; 
	char host[BUF_LEN] = {0};
	char GET[BUF_LEN] = {0}; 
	char header[BUF_LEN] = {0}; 

	int total_receive_len = 0;
	int cur_recv_str_len = 0;
	char buf_for_every_recv[BUF_LEN] = {0}; 

	/* 
	* windows��ʹ��socket������WSAStartup��ʼ���������ܵ��� 
	*/ 
	if(WSAStartup(MAKEWORD(2,2), &wsa_data)) 
	{ 
		printf("WSA failed\n"); 
		return -1; 
	}

	/* 
	* ����url�е�������ַ�����·�� 
	*/ 
	strcpy_s(url_buf, 1024, url); 
	for (pHost = url_buf; *pHost != '/' && *pHost != '\0'; ++pHost); 

	if ( (int)(pHost - url_buf) == strlen(url_buf) ) 
	{
		strcpy(GET, "/"); 
	}
	else
	{
		strcpy(GET, pHost); 
	}

	*pHost = '\0'; 
	strcpy(host, url_buf); 
	//printf("%s\n%s\n", host, GET);

	/* 
	* �趨socket����,��δ������ʼ�� 
	*/ 
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
	pURL = gethostbyname(host); 
	if (NULL == pURL)
	{
		printf("��ȡpURL����!\n");
		return -1;
	}
	addr.sin_family = AF_INET; 
	addr.sin_addr.s_addr = *((unsigned long*)pURL->h_addr); 
	addr.sin_port = htons(80);

	/* 
	* ��֯���͵�web����������Ϣ 
	* Ϊ��Ҫ�����������Ϣ��ο�HTTPЭ���Լ�� 
	*/ 
	strcat(header, "GET "); 
	strcat(header, GET); 
	strcat(header, " HTTP/1.1\r\n"); 
	strcat(header, "HOST: "); 
	strcat(header, host); 
	strcat(header, "\r\nConnection: Close\r\n\r\n"); 
	//printf(header); 
	/* 
	* ���ӵ�����������������header�������ܷ���������ҳԴ���룩 
	*/ 
	connect(sockfd,(SOCKADDR *)&addr,sizeof(addr));

	send(sockfd, header, strlen(header), 0);

	cur_recv_str_len = recv(sockfd, buf_for_every_recv, BUF_LEN, 0);

	while (cur_recv_str_len > 0 && total_receive_len < page_size) 
	{   
		memcpy(page_data + total_receive_len, buf_for_every_recv, cur_recv_str_len); 
		total_receive_len += cur_recv_str_len;

		memset(buf_for_every_recv,0,BUF_LEN);

		cur_recv_str_len = recv(sockfd, buf_for_every_recv, BUF_LEN, 0);
	}

	closesocket(sockfd);

	WSACleanup();

	return 0; 
}

//extern CURL_EXTERN CURLcode curl_easy_recv(CURL *curl, void *buffer, size_t buflen, size_t *n);

// int get_url_page_libcurl(const char* url, char* page_data, int page_size)
// {
// 	size_t count = 0;
// 	char buf[BUF_LEN] = {0};
// 
// 	char* init = curl_easy_init();
// 
// 	strcpy_s(buf, BUF_LEN, url);
// 	curl_easy_recv(buf, page_data, page_size, &count);
// 
// 	return 0;
// }

/* Auxiliary function that waits on the socket. */
static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
  struct timeval tv;
  fd_set infd, outfd, errfd;
  int res;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec= (timeout_ms % 1000) * 1000;

  FD_ZERO(&infd);
  FD_ZERO(&outfd);
  FD_ZERO(&errfd);

  FD_SET(sockfd, &errfd); /* always check for error */

  if(for_recv)
  {
    FD_SET(sockfd, &infd);
  }
  else
  {
    FD_SET(sockfd, &outfd);
  }

  /* select() returns the number of signalled sockets or -1 */
  res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
  return res;
}

// int get_url_page_libcurl(const char* url, char* page_data, int page_size)
// {
//   CURL *curl;
//   CURLcode res;
//   /* Minimalistic http request */
//   char request[BUF_LEN] = {0};
//   //const char *request = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n";
//   curl_socket_t sockfd; /* socket */
//   long sockextr;
//   size_t iolen;
//   int rec_len = 0;
//   curl_off_t nread;
// 
//   sprintf(request, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", url);
// 
//   curl = curl_easy_init();
//   if(curl) {
// 	//curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
// 	curl_easy_setopt(curl, CURLOPT_URL, url);
//     /* Do not do the transfer - only connect to host */
//     curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
//     res = curl_easy_perform(curl);
// 
//     if(CURLE_OK != res)
//     {
//       printf("Error: %s\n", strerror(res));
//       return -1;
//     }
// 
//     /* Extract the socket from the curl handle - we'll need it for waiting.
//      * Note that this API takes a pointer to a 'long' while we use
//      * curl_socket_t for sockets otherwise.
//      */
//     res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);
// 
//     if(CURLE_OK != res)
//     {
//       printf("Error: %s\n", curl_easy_strerror(res));
//       return -1;
//     }
// 
//     sockfd = sockextr;
// 
//     /* wait for the socket to become ready for sending */
//     if(!wait_on_socket(sockfd, 0, 60000L))
//     {
//       printf("Error: timeout.\n");
//       return -1;
//     }
// 
//     puts("Sending request.");
//     /* Send the request. Real applications should check the iolen
//      * to see if all the request has been sent */
//     res = curl_easy_send(curl, request, strlen(request), &iolen);
// 
//     if(CURLE_OK != res)
//     {
//       printf("Error: %s\n", curl_easy_strerror(res));
//       return -1;
//     }
//     puts("Reading response.");
// 
//     /* read the response */
// 	iolen = 0;
// 	rec_len = 0;
//     for(;;)
//     {
//       //char buf[1024];
// 
//       //wait_on_socket(sockfd, 1, 60000L);
//       res = curl_easy_recv(curl, page_data + rec_len, page_size - rec_len, &iolen);
// 
//       if(CURLE_OK != res)
//         break;
// 
// 	  rec_len += iolen;
//       nread = (curl_off_t)iolen;
// 
//       printf("Received %" CURL_FORMAT_CURL_OFF_T " bytes.\n", nread);
//     }
// 
//     /* always cleanup */
//     curl_easy_cleanup(curl);
//   }
//   return 0;
// }

int writer(unsigned char *data, size_t size, size_t count, page_buf_t page_buf)
{
	int wr_size = write_page_buf(page_buf, data, size*count);
	return wr_size;
}

static int get_data_libcurl(const char* url, page_buf_t page_buf)
{
	CURLcode code;

	CURL *curl = curl_easy_init();
	if(curl == NULL)
	{
		return -1;
	}  
	//curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);//30�볬ʱ
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,writer);//�󶨻ص�����
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, page_buf);//���ò��� 
	code = curl_easy_perform(curl); //ִ�����������Ҳ����˵��ʼͨ��
	if(code != CURLE_OK)
	{
		curl_easy_cleanup(curl);
		return -1;
	}
	curl_easy_cleanup(curl);
	
	return 0;
}

/* �����5�� ��5�ξ�ʧ���򷵻�ʧ�� */
int get_url_page_libcurl(const char* url, page_buf_t page_buf)
{
    int ret = 0;
    int i = 0;
    for (; i < 5; i++)
    {
        ret = get_data_libcurl(url, page_buf);
        if (-1 == ret)
        {
            clear_page_buf(page_buf);
            continue;
        }
        return 0;
    }

    //printf("LIBCURL FAIL : %s\n", url);
    return -1;
}