#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h> 
#include <unistd.h>
#include<string.h> 
#include <arpa/inet.h>
#include<time.h>
#include <stdlib.h>

struct chunk_s {
int type;
int length;
char buff[80];
};

void main()
{
int b,sockfd,connfd,sin_size,l,n,len,length,size;
char operator,choice,word[50];
time_t tick;
int op1,op2,result;

if((sockfd=socket(AF_INET,SOCK_STREAM,0))>0)
printf("socket created sucessfully\n");  

struct sockaddr_in servaddr;              
struct sockaddr_in clientaddr;

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
servaddr.sin_port=6006;

if((bind(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)))==0)
printf("bind sucessful\n");   
if((listen(sockfd,5))==0) 
printf("listen sucessful\n");


sin_size = sizeof(struct sockaddr_in);
if((connfd=accept(sockfd,(struct sockaddr *)&clientaddr,&sin_size))>0);
printf("accept sucessful\n");

struct chunk_s chunks;
bzero(&chunks,sizeof(chunks));
read(connfd,&chunks,sizeof(chunks));
if(chunks.type == 1)
{
	printf("INIT RECEIVED\n");
	chunks.type=2;
	strcpy(chunks.buff,"INIT ACK");
	chunks.length = sizeof(chunks.buff);
	printf("ACK SENT\n");
}
else
{
	printf("INITIATION IS NOT SUCCESSFULLY\n");
	exit(1);
}
write(connfd,&chunks,sizeof(chunks));
bzero(&chunks,sizeof(chunks));
read(connfd,&chunks,sizeof(chunks));
if(chunks.type == 10)
{
	printf("COOKIE ECHO RECEIVED\n");
	chunks.type=11;
	strcpy(chunks.buff,"COOKIE ACK");
	chunks.length = sizeof(chunks.buff);
	printf("COOKIE ACK SENT\n");
}
else
{
	printf("INITIATION IS NOT SUCCESSFULLY\n");
	exit(1);
}
write(connfd,&chunks,sizeof(chunks));
for(;;)
{
	bzero(&chunks,sizeof(chunks));
	read(connfd,&chunks,sizeof(chunks));
	switch(chunks.type)
	{
		case 0:
		        /*length = strlen(chunks.buff);
		        if(length == 0)
		            printf("From client : Empty String\n");
		            strcpy(chunks.buff,"SACK");
			    chunks.type = 0;
			    chunks.length = sizeof(chunks.buff);
			    write(connfd,&chunks,sizeof(chunks));
			    break;
		        }
		        else
		        {*/
			printf("From client : %s\n",chunks.buff);
			bzero(&chunks,sizeof(chunks));
			strcpy(chunks.buff,"ACK");
			chunks.type = 0;
			chunks.length = sizeof(chunks.buff);
			write(connfd,&chunks,sizeof(chunks));
			break;
			//}
		case 3:
		        printf("%s\n",chunks.buff);
			//bzero(&chunks,sizeof(chunks));
			size=strlen(chunks.buff);
			for(int i=0;i<size;i++)
			{
			  word[i]=chunks.buff[i];
			}
			size=strlen(chunks.buff);
			int mid=size/2;
			chunks.buff[0]=mid;
			chunks.type = 11;
			chunks.length = sizeof(chunks.buff);
			write(connfd,&chunks,sizeof(chunks));
			break;
		case 4:
			printf("%s\n",chunks.buff);
			bzero(&chunks,sizeof(chunks));
			strcpy(chunks.buff,"HEARTBEAT ACK");
			chunks.type = 5;
			chunks.length = sizeof(chunks.buff);
			write(connfd,&chunks,sizeof(chunks));
			break;
		case 6:
			printf("SOCKET CLOSED. PROCESS TERMINATED\n");
			close(sockfd);
			exit(1);
			break;
		case 7:
	                printf("%s\n",chunks.buff);
			bzero(&chunks,sizeof(chunks));
			printf("SHUTDOWN ACK\n");
			close(sockfd);
			exit(1);
			break;	  
		case 12:
		        printf("%s\n",chunks.buff);
			bzero(&chunks,sizeof(chunks)); 
			break;  
		case 14:
		        printf("OPERATIONAL ERROR\n");
		        printf("SOCKET CLOSED. PROCESS SHUTDOWN COMPLETED\n");
			close(sockfd);
			exit(1);
			break;
	}
}

close(sockfd);


}
