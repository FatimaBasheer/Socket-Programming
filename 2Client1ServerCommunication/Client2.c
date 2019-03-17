#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>
void fileInteraction(int sockid){
	char buffer[100];
	FILE* rollList = fopen("RollNumberC2.txt","w");
	fclose(rollList);
	while(1){
		fgets(buffer,sizeof(buffer),stdin);
		send(sockid,buffer,sizeof(buffer),0);
		bzero(buffer,sizeof(buffer));
		
		rollList = fopen("RollNumberC2.txt","a");
		if(rollList==NULL){
			printf("Error!Could not open the file\n");
			exit(-1);
		}
		int count;
		/*while(fgets(buffer,sizeof(buffer),rollList)!=NULL){
				
			send(sockid,buffer,sizeof(buffer),0);
			bzero(buffer,sizeof(buffer));	
		}
		strcpy(buffer,"-1");
		send(sockid,buffer,sizeof(buffer),0);
		fclose(rollList);
		rollList = fopen("NumbersC.txt","w");
		bzero(buffer,sizeof(buffer));
		count = read(sockid,buffer,sizeof(buffer));*/
		recv(sockid,buffer,sizeof(buffer),0);
		if(!strncmp(buffer,"0",1)){
			fclose(rollList);
			return ;
		}
		fprintf(rollList,"%s",buffer);
		fclose(rollList);
	}
}
void main(){
	int sockid,socknew;
	struct sockaddr_in serveraddr,clientaddr;

	if(!(sockid=socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);
	}
	printf("Socket Creation Successful...\n");
	memset(&serveraddr,0,sizeof(serveraddr));

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port=htons(7002);

	if(connect(sockid,(struct sockaddr *)&serveraddr , sizeof(serveraddr))){
		printf("Connection to Server Failed!\n");
		exit(0);
	}
	printf("Connection to Server Successful...\n");
	fileInteraction(sockid);
	close(sockid);

}
