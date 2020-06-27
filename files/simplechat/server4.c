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
**************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<pthread.h>
#define SERVER_PORT 1234
struct userinfo{                     //保存用户名和密码的结构体{
    char username[20];
    char password[20];
};
struct userinfo Users[] = {
    {"shaocong", "123"},
	{"gexiao", "456"},
    {" ", " "}                      //以只含一个空格的字符串作为数组的结束标志
};
void UserinfoInput(int clnt_sock,struct userinfo *user){
	char name[20];
	char pwd[20];
	memset(name,0,20);
	memset(pwd,0,20);

	recv(clnt_sock,name,sizeof(name),0);
	strcpy(user->username,name);
	printf("username: %s\n",user->username);

	recv(clnt_sock,pwd,sizeof(pwd),0);
	strcpy(user->password,pwd);
	printf("password: %s\n",user->password);
}
int ConfirmUser(struct userinfo *user){	
	int i;
	for(i=0;Users[i].username[0]!=' ';i++){
        if(strcmp(Users[i].username,user->username) == 0){
			if(strcmp(Users[i].password,user->password) == 0)	return i;
			else	return -1;
        }
    }
	return -2;
}

struct tranmsg{
	int src_client;
	char message[100];
	int dst_client;
};
void *transportmessage(void *para){
	struct tranmsg *msg; 
	msg=(struct tranmsg *)para;
	memset(msg->message,0,sizeof(msg->message));
	int flag=0;
	while(1){		
		if((recv(msg->src_client,msg->message,sizeof(msg->message),0))<0)
			printf("server recv error!\n");
		/*if(flag%2==1){
			flag++;
			continue;
		}
		flag++;*/
		if((send(msg->dst_client,msg->message,sizeof(msg->message),0))==SO_ERROR)
			printf("server send error!\n");
		else
			printf("%s\n",msg->message);

		if(strcmp(msg->message,"quit")==0){	 
			printf("a talker quit communication\n");
			break;
		}
		else if(strlen(msg->message)==0){
			printf("a user process shutdown\n");
			break;
		}
		memset(msg->message,0,sizeof(msg->message));
	}
}
void transportfile(int srcclient,int dstclient){
	char tembuffer[100];
	int recvcount;
	char filename[20];
	recv(srcclient,filename,sizeof(filename),0);
	send(dstclient,filename,sizeof(filename),0);
	while(1){
		recvcount=recv(srcclient,tembuffer,sizeof(tembuffer),0);
		send(dstclient,tembuffer,recvcount,0);
		if(recvcount<100)	break;
	}
	printf("srcclient%d's files to dstclient%d,transfer finish!\n",srcclient,dstclient);
}


int main(int argc,char *argv[]){
    int serv_sock;
	if((serv_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){  //创建套接字
		perror("socket");
		return 1;
	}
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	memset(&serv_addr,0,sizeof(serv_addr));//每个字节都用0填充
    serv_addr.sin_family=AF_INET;  //使用IPv4地址


    serv_addr.sin_addr.s_addr=htons(INADDR_ANY);  //服务端IP地址
    serv_addr.sin_port=htons(SERVER_PORT);  //端口
   

	int on=1;  
    if((setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0){  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }
    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){  //将套接字和IP、端口绑定
		perror("connect");
		return  1;  
	}
    if(listen(serv_sock,20)<0){  //进入监听状态，等待用户发起请求
		perror("listen");
		return 1;
	}
    //接收客户端请求
    struct sockaddr_in clnt_addrA,clnt_addrB;
    socklen_t clntA_addr_size=sizeof(clnt_addrA);
	socklen_t clntB_addr_size=sizeof(clnt_addrB);
    int clnt_sockA,clnt_sockB;
//*********************************user login********************************************
	int user_num;
	struct userinfo userA,userB;

	clnt_sockA=accept(serv_sock,(struct sockaddr*)&clnt_addrA,&clntA_addr_size);
	if(clnt_sockA<0)	perror("acceptA");
	do{
		UserinfoInput(clnt_sockA,&userA);
		user_num=ConfirmUser(&userA);
		printf("user_num:%d\n\n",user_num);
		send(clnt_sockA,&user_num,sizeof(int),0);  
	}while(user_num<0);
	
	clnt_sockB=accept(serv_sock,(struct sockaddr*)&clnt_addrB,&clntB_addr_size);
	if(clnt_sockB<0)	perror("acceptB");
	do{
		UserinfoInput(clnt_sockB,&userB);
		user_num=ConfirmUser(&userB);		
		printf("user_num:%d\n\n",user_num);
		send(clnt_sockB,&user_num,sizeof(int),0);
	}while(user_num<0);
//*************************************************************************************
while(1){
	int optA,optB;	
    recv(clnt_sockA,&optA,sizeof(int),0);
    recv(clnt_sockB,&optB,sizeof(int),0);

	pthread_t *pth;
	pth=malloc(2*sizeof(pthread_t));

	if(optA==optB && optA != 3 && optA != 4){
		if(optA==0){//end process
			printf("two users want to exit process..\nserver process exit!\n");
			break;
		}else if(optA==1){//chat
			printf("users want to chat\n");	
			struct tranmsg AmsgToB,BmsgToA;
			AmsgToB.src_client=clnt_sockA;
			AmsgToB.dst_client=clnt_sockB;

			BmsgToA.src_client=clnt_sockB;
			BmsgToA.dst_client=clnt_sockA;

			pthread_create(&pth[0],NULL,transportmessage,&AmsgToB);
			pthread_create(&pth[1],NULL,transportmessage,&BmsgToA);
			pthread_join(pth[0],NULL);
			pthread_join(pth[1],NULL);
			free(pth);
		}else{//transfer files
			printf("users want to transfiles\n");
			int a,b;
			recv(clnt_sockA,&a,sizeof(int),0);
			recv(clnt_sockB,&b,sizeof(int),0);
			
			if(a==b){
				transportfile(clnt_sockA,clnt_sockB);
				transportfile(clnt_sockB,clnt_sockA);
			}
			else if(a==3&&b==4)	transportfile(clnt_sockA,clnt_sockB);
			else if(a==4&&b==3)	transportfile(clnt_sockB,clnt_sockA);
			else	printf("users choose wrong!\n");
		}
	}else if (optA ==3 || optA ==4 || optB == 3 || optB == 4){
		printf("some user upload/download files");
		/*struct msgobj AtoB, BtoA;
		AtoB.src_client = clnt_sockA;
		AtoB.dst_client = clnt_sockB;
		BtoA.src_client = clnt_sockB;
		BtoA.dst_client = clnt_sockA;
		//int a,b;
		if (optA ==3){
			pthread_create(&pth[0],NULL,optA3,&AtoB);
			//pthread_create(&pth[1],NULL,optA3,&BtoA);
			pthread_join(pth[0],NULL);
			//pthread_join(pth[1],NULL);
			free(pth);
			
		}else if(optA == 4){

		}else if(optB == 3){
		
		}else if(optB == 4){
		
		}else {
			printf("What Fuck!\n");
		};*/
		

	}else{
		if(optA==0||optB==0){
			printf("one or more users choose to end process..\nserver process also exit!\n");
			break;
		}
		else printf("two users's option is different or input wrong,connect can't be built!\n");
	}
}

    close(clnt_sockA);
   	close(clnt_sockB);
    close(serv_sock);
    return 0;
}







