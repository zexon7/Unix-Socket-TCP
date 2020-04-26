#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/stat.h>

char buffer[1024];
int bytesReceived = 0;
int sockfd = 0,forClientSockfd = 0;
struct sockaddr_in serverInfo,clientInfo;

//client disconnect function
void gb(){
    printf("The client \"%s\" with port %d has terminated the connection.\n",inet_ntoa(clientInfo.sin_addr),ntohs(clientInfo.sin_port));
    write(forClientSockfd,"See you next time.\n",sizeof(buffer));
}

int main(int argc , char *argv[])
{
    //create socket
    char fname[100];
    int size = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        perror("Fail to create a socket.");
        exit(1);
    }
    
    //define server address
    //struct sockaddr_in serverInfo,clientInfo;
    int cli_len = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));//erases the data by writing zeros
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY; //htonl(INADDR_ANY);
    serverInfo.sin_port = htons(8080);

    if(bind(sockfd, (struct sockaddr *) &serverInfo, sizeof(serverInfo)) < 0){// bind socket address
        perror("Failed to bind");
        exit(1);
    }

    if(listen(sockfd, 5) < 0){
        perror("Fail to listen");
        exit(1);
    }

    printf("Server started.\n");
    
    while(1){

        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &cli_len);        
        if(forClientSockfd < 0){
            perror("Fail to accept");
            exit(1);
        }
        
        //send Welcome   
        char s_msg[] = {"Welcome!!! Input \"goodbye\" to close socket or \"help\" for more information.\n"};                
        write(forClientSockfd,s_msg,sizeof(s_msg));
        
        //received client info
        read(forClientSockfd,buffer,sizeof(buffer));
        printf("A client \"%s\" has connected via port num %d using %s.\n",inet_ntoa(clientInfo.sin_addr),ntohs(clientInfo.sin_port),buffer);
        
        //if received upload
        read(forClientSockfd,buffer,sizeof(buffer));
        int u = strncmp("upload", buffer, 6);
        if(u == 0){
            FILE *fp;
            printf("Receiving file... \n");      
            read(forClientSockfd, fname,256);//received filename
            fp = fopen(fname, "wb");//create a file
    	    if(fp == NULL){
       	        printf("Error opening file\n");
                exit(1);
    	    }
            bzero(buffer,sizeof(buffer));
             
            while(1){
                bytesReceived = read(forClientSockfd,buffer,sizeof(buffer));//read file into buffer
                if(bytesReceived == 0){
                    break;
                }
                fwrite(buffer, sizeof(char), bytesReceived,fp);//write buffer into file
                size = size + bytesReceived;
            }
            fclose(fp);
            printf("The client send a file \"%s\" with size of %d bytes.\n",fname,size);        
        }

        //if received goodbye             
        int dc = strncmp("goodbye", buffer, 7);
        if(dc == 0){
            gb();
        }
        
    }

    return 0;
}