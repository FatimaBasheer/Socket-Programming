//Server
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#define QUEUE_LIMIT 1


FILE* delFirstLine(FILE * rollLis){

	char in,ch;
	FILE* newfile = fopen("Replica.txt","w");
	//while((in = fgetc(rollLis)) !='\n'){
 	//}
	while( ( ch = fgetc(rollLis) ) != EOF )
	      fputc(ch, newfile);
	fclose(rollLis);
	remove("RollS.txt");
	rename("Replica.txt","RollS.txt");
	return newfile;

}

void fileInteraction(int sockid){
	char buffer[100];
	FILE* rollLis = fopen("RollS.txt","r");
	if(rollLis==NULL){
		printf("Error!Could not create the file\n");
		exit(-1);
	}
	int count;
	bzero(buffer,sizeof(buffer));
	//count =  read(sockid,buffer,sizeof(buffer));
	//while(strcmp(buffer,"-1")){
	//	fprintf(rollLis,"%s",buffer);
		//printf("%s",buffer);
	//	count = read(sockid,buffer,sizeof(buffer));
		
	//}
	//check if the file is empty
	fseek (rollLis, 0, SEEK_END);
        int size = ftell(rollLis);
	if(size==0){
		strcpy(buffer,"0");
		send(sockid,buffer,sizeof(buffer),0);
		//*s = 1;
		return ;
	}
	fclose(rollLis);
	rollLis = fopen("RollS.txt","r+");
	fgets(buffer,sizeof(buffer),rollLis);
	send(sockid, buffer,sizeof(buffer),0);
	//fprintf(stdin,"%s",buffer);
	bzero(buffer,sizeof(buffer));
	fclose(delFirstLine(rollLis));
	
	/*rollLis = fopen("RollS.txt","r");
	bzero(buffer,sizeof(buffer));
	fgets(buffer,sizeof(buffer),rollLis);
	int i =0;
	int j=0;
	char num[10];
	while(buffer[i]!='\0'){
		if(buffer[i]>='0'&&buffer[i]<='9'){
			num[j]=buffer[i];
			++j;
		
}
		++i;
	}
	num[j]='\0';
	strcpy(buffer,num);
	send(sockid,buffer,sizeof(buffer),0);
	fclose(rollLis);	
	*/
}

int main(){
	int sockid,len,socknew;
	struct sockaddr_in serveraddr,clientaddr;

	int c = fork();
	//socket creation
	if(!(sockid = socket(AF_INET,SOCK_STREAM,0))){
		printf("Socket Creation Failed!\n");
		exit(0);		
	}
	printf("Socket Creation Successful...\n");
	memset(	&serveraddr,0,sizeof(serveraddr));
	
	//setting the server address and port
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(c ==0){
		serveraddr.sin_port = htons(7002);
	}
	else if(c>0){
		serveraddr.sin_port = htons(4002);
	}

	if(bind(sockid,(struct sockaddr *) &serveraddr,sizeof(serveraddr))!=0){
		printf("Socket Binding Failed!\n");
		exit(0);
	}
	printf("Binded to Socket Successfully...\n");
	if(listen(sockid,QUEUE_LIMIT)){
		printf("Listening Failed!\n");
		exit(0);
	}
	len= sizeof(clientaddr);
	printf("Server Listening...\n");
	if(!(socknew=accept(sockid,(struct sockaddr *) &clientaddr,&len))!=0){
		printf("Sever Accepting Failed!\n");
		exit(0);
	}
	printf("Server Accepted the client...\n");
	char serverbuffer[256];
	while(1){
		recv(socknew,serverbuffer,sizeof(serverbuffer),0);
		if(!strcmp(serverbuffer,"\n")){
			fileInteraction(socknew);
		}
		else{
			continue;
		}
	}
	close(sockid);
}

