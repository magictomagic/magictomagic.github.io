/*************************************
compile required: -pthread -lcurl
e.g. 
	gcc client4.c -pthread -lcurl -o client4.o && ./client4.o
	gcc server4.c -pthread -lcurl -o server4.o && ./server4.o
watch your offline upload & download result(s), visit: http://47.94.157.136/2345/
upload file from: /home/hhy/simpleChat/uploadPlace/
download file to: /home/hhy/simpleChat/downloadPlace/
you can change the directory as you like by modifying code.


some server releated commands:
restart apache: 
	systemctl restart httpd.service


better to do:
NAT
list all files.
send a message(file name) to partner while upload/download a offline file.


Bug:
after offine up/down load, chat cannot be used, 
server say: 
	two users's option is different or input wrong,connect can't be built!
**************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <malloc.h>
#include <pthread.h>
#include <fcntl.h>
#include <curl/curl.h>
#define SERVER_PORT 1234
/* 从http头部获取文件size*/
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
       int r;
       long len = 0;
 
       /* _snscanf() is Win32 specific */
       // r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
 r = sscanf(ptr, "Content-Length: %ld\n", &len);
       if (r) /* Microsoft: we don't read the specs */
              *((long *) stream) = len;
 
       return size * nmemb;
}
 
/* 保存下载文件 */
size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
        return fwrite(ptr, size, nmemb, stream);
}
 
/*读取上传文件 */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
       FILE *f = stream;
       size_t n;
 
       if (ferror(f))
              return CURL_READFUNC_ABORT;
 
       n = fread(ptr, size, nmemb, f) * size;
 
       return n;
}
 
// 下载 或者上传文件函数
int download(CURL *curlhandle, const char * remotepath, const char * localpath,
           long timeout, long tries)
{
       FILE *f;
       curl_off_t local_file_len = -1 ;
       long filesize =0 ;
       
       CURLcode r = CURLE_GOT_NOTHING;
       int c;
  struct stat file_info;
  int use_resume = 0;
  /* 得到本地文件大小 */
  //if(access(localpath,F_OK) ==0)
  
    if(stat(localpath, &file_info) == 0) 
     {
        local_file_len =  file_info.st_size;
        use_resume  = 1;
      }
  //采用追加方式打开文件，便于实现文件断点续传工作
       f = fopen(localpath, "ab+"); 
       if (f == NULL) {
              perror(NULL);
              return 0;
       }
 
       //curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
 
       curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
 
       curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
       //设置http 头部处理函数
       curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
       curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
 // 设置文件续传的位置给libcurl
       curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
 
       curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
       curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);
 
       //curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
       //curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
       curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
       curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
  
  
  r = curl_easy_perform(curlhandle);
       
 
       fclose(f);
 
       if (r == CURLE_OK)
              return 1;
       else {
              fprintf(stderr, "%s\n", curl_easy_strerror(r));
              return 0;
       }
}
 
struct recvpromsg{
	char recv_message[100];
	int serversock;
};
void *recvprocess(void *arg){
	struct recvpromsg *recvmsg;
	recvmsg=(struct recvpromsg*)arg;
	while(1){
		recv(recvmsg->serversock,recvmsg->recv_message,sizeof(recvmsg->recv_message),0);	
		if(strcmp(recvmsg->recv_message,"quit")==0 || strlen(recvmsg->recv_message)==0){
			printf("talker quit!\n");
			break;
		}
		printf("%s\n",recvmsg->recv_message);
		memset(recvmsg->recv_message,0,sizeof(recvmsg->recv_message));
	}
}

// 下载 或者上传文件函数
int upload(CURL *curlhandle, const char * remotepath, const char * localpath,
           long timeout, long tries)
{
       FILE *f;
       curl_off_t local_file_len = -1 ;
       long filesize =0 ;
       
       CURLcode r = CURLE_GOT_NOTHING;
       int c;
  struct stat file_info;
  int use_resume = 0;
  /* 得到本地文件大小 */
  //if(access(localpath,F_OK) ==0)
  
    if(stat(localpath, &file_info) == 0) 
     {
        local_file_len =  file_info.st_size;
        use_resume  = 1;
      }
  //采用追加方式打开文件，便于实现文件断点续传工作
       f = fopen(localpath, "ab+"); 
	//f = fopen(remotepath, "ab+"); 
       if (f == NULL) {
              perror(NULL);
              return 0;
       }
 
       curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
 
       curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
 
       curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
       //设置http 头部处理函数
       //curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
       //curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
 // 设置文件续传的位置给libcurl
       curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
 
       //curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
       //curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);
 
       curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
       curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
       curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
       curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
  
  
  r = curl_easy_perform(curlhandle);
       
 
       fclose(f);
 
       if (r == CURLE_OK)
              return 1;
       else {
              fprintf(stderr, "%s\n", curl_easy_strerror(r));
              return 0;
       }
}
int main(int argc,char*argv[]) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);  //创建套接字

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址


    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //IP地址
    serv_addr.sin_port = htons(SERVER_PORT);  //端口


    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));  //向服务器（特定的IP和端口）发起请求

	char username[20];
	char password[20];
	int valid=-1;

	while(valid<0){
		memset(username,0,sizeof(username));
		memset(password,0,sizeof(password));
	
		printf("username: ");
		scanf("%s",username);
		send(sock,username,sizeof(username),MSG_NOSIGNAL);
		printf("password: ");
		scanf("%s",password);
		send(sock,password,sizeof(password),0);

 		recv(sock,&valid,sizeof(int),0);
		switch(valid){
			case -2: printf("\nUser not exist!\n\n");	break;
			case -1: printf("\nUser not exist but wrong password!\n\n");	break;
			default: printf("\nLogin success!\n\n");	break;
		}
	}
	sleep(1);
while(1){
	int mod;
	//printf("input '0' to end process while '1' to chat and '2' to transfer files: ");
	printf("input: 0	exit\ninput: 1	chat\ninput: 2	transfer online files\ninput: 3	break point & offline download\ninput: 4	break point & offline upload\nYour Choice: ");
	scanf("%d",&mod);
	send(sock,&mod,sizeof(int),0);
	if(mod==0){
		printf("process exit!\n");
		break;
	}

	if(mod==1){
		printf("\n***********************chat start**************************************\n");
		printf("\n|**user input window***************|chat message window****************|\n");
		struct recvpromsg recvmsg;
		memset(recvmsg.recv_message,0,sizeof(recvmsg.recv_message));
		recvmsg.serversock=sock;

		pthread_t recv_pth;
		//recv_pth=(pthread_t *)malloc(sizeof(pthread_t));
		pthread_create(&recv_pth,NULL,recvprocess,&recvmsg);

		char send_message[20];
		char print_send_message[100];
		char star[40]="|**                              **|";
		while(1){
			memset(send_message,0,sizeof(send_message));
			memset(print_send_message,0,sizeof(print_send_message));
	   		scanf("%s",send_message);
			if(strcmp(send_message,"quit")==0){
				send(sock,send_message,sizeof(send_message),0);
				break;
			}
			sprintf(print_send_message,"%s%s:%s",star,username,send_message);
			printf("%s\n",print_send_message);
			if(send(sock,print_send_message,sizeof(print_send_message),0)==SO_ERROR){
				printf("send error\n\n");
			}			
		}
		pthread_join(recv_pth,NULL);
		//free(&recv_pth);
	}else if (mod == 2){//mod==2,transfer files;
		int c;
		printf("\ninput '3' to send files while '4' to receive : ");
		scanf("%d",&c);
		send(sock,&c,sizeof(int),0);

		if(c==3){
			char filename[20];
			char sendbuffer[100];
			int read_count,send_count;

			printf("please input the filename which will be sent: ");
			scanf("%s",filename);
			send(sock,filename,sizeof(filename),0);

			int sendfile;
			sendfile=open(filename,O_RDONLY);
			if(sendfile==-1) printf("sendflie open fail!\n");
			else{
				while(1){
					read_count=read(sendfile,sendbuffer,sizeof(sendbuffer));
					send_count=send(sock,sendbuffer,read_count,0);
					memset(sendbuffer,0,sizeof(sendbuffer));
					if(read_count<100)	break;
				}
				printf("send success!\n");
				close(sendfile);
			}	
		}else if(c==4){
			char recvfilename[20];
			recv(sock,recvfilename,sizeof(recvfilename),0);
			if(creat(recvfilename,0777)<0)	printf("recvfile create fail！\n");
			
			char recvbuffer[100];
			int recv_count;
			int recvfile;
			if((recvfile=open(recvfilename,O_RDWR))==-1)	printf("recvflie open fail!\n");
			else{
				while(1){
					recv_count=recv(sock,recvbuffer,sizeof(recvbuffer),0);
					write(recvfile,recvbuffer,recv_count);
					memset(recvbuffer,0,recv_count);
					if(recv_count<100)	break;
				}
				printf("recevie success!\n");
				close(recvfile);
			}	
		}
		else{
			printf("wrong input\n");
		}
	
	}else if (mod == 3){	//break point & offline download
		
	    	CURL *curlhandle = NULL;
       		curl_global_init(CURL_GLOBAL_ALL);
       		curlhandle = curl_easy_init();
		char url_path[500], file_name[100], local_path[300];
		strcpy(url_path, "http://47.94.157.136/2345/");			// my apache server
		strcpy(local_path, "/home/hhy/simpleChat/downloadPlace/");	// set your own local path
		printf("type a file name you want to download\nFile Name: ");
		scanf("%s", file_name);

		//send(sock,file_name,sizeof(file_name),0);

		strcat(url_path, file_name);
		strcat(local_path, file_name);
		printf("%s", url_path);
       		download(curlhandle , url_path,local_path,1,3);
       		curl_easy_cleanup(curlhandle);
       		curl_global_cleanup();

	}else if (mod == 4){	//break point & offline upload
		CURL *curlhandle = NULL;
       		curl_global_init(CURL_GLOBAL_ALL);
       		curlhandle = curl_easy_init();
 		char ftp_url_path[500], ftp_file_name[100], local_file_path[100];
		strcpy(ftp_url_path, "ftp://root:hhyzuishuai@47.94.157.136//var/www/html/2345/"); // double '/' means start from /
		strcpy(local_file_path, "/home/hhy/simpleChat/uploadPlace/");	// set your own local path
		printf("type a file name you want to upload\nFile Name: ");
		scanf("%s", ftp_file_name);
		strcat(ftp_url_path, ftp_file_name);
		strcat(local_file_path, ftp_file_name);
		upload(curlhandle, ftp_url_path, local_file_path, 10, 3);
       		curl_easy_cleanup(curlhandle);
       		curl_global_cleanup();

	} else {
		printf("input wrong\n");
		exit(0);
	}
}
	close(sock);  //关闭套接字
    return 0;
}
