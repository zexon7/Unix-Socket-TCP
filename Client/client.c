#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/stat.h>

int main(int argc , char *argv[])
{
    //create socket
    char buffer[1024];
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (argc < 3) {//check user input
         perror("No IP or port provided.\n");
         exit(1);
     }

    if (sockfd == -1){
        perror("Fail to create a socket.");
        exit(1);
    }
    
    //define which server client wants to connect
    struct sockaddr_in serverInfo;
    bzero(&serverInfo,sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
    serverInfo.sin_port = htons(atoi(argv[2]));

    if(connect(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo)) < 0){
        perror("Connection error");
        exit(1);
    }
    printf("The server with IP \"%s\" has accepted your connection.\n",argv[1]);

    //receive Welcome
    read(sockfd,buffer,sizeof(buffer));
    printf("%s",buffer);

    //Send a info to server
    char type[] = {"SOCK_STREAM(TCP)"};
    write(sockfd,type,sizeof(type));

    //input command
    while(1){
        char input[16];
        scanf("%s",input);
        //goodbye command
        if(strcmp(input,"goodbye")==0){
            write(sockfd,input,sizeof(input));// send goodbye to server to let server know client is going to close socket
            read(sockfd,buffer,sizeof(buffer));
            printf("%s\n",buffer);
            close(sockfd);
            break;   
        }
        //upload command
        else if(strcmp(input,"upload")==0){
            char fname[100];
            write(sockfd,input,sizeof(input)); 
            printf("Please input your filename: ");
            scanf("%s",fname);
            FILE *fp = fopen(fname,"rb");
            if(fp==NULL)
            {
                printf("Invalid filename or file doesn't exist.Please input again.\n");
                scanf("%s",fname);
                fp = fopen(fname,"rb");  
            }
            write(sockfd, fname,256);//send filename
            int numbytes;
            char buff[5000000]; 
            while(1){
		        numbytes = fread(buff, sizeof(char), sizeof(buff), fp);//size,times
                if (numbytes == 0)
                    break;
                numbytes = write(sockfd, buff, numbytes);      
		        printf("Original file length: %d bytes\n",numbytes);
            }
            close(sockfd);
            break;
        }
        //help command
        else if(strcmp(input,"help")==0){
            printf("\nupload\t\t upload your file\n");
            printf("goodbye\t\t close socket\n\n");
        }
        else {printf("Input \"goodbye\" to close socket or \"help\" for more information.\n");}
        
    }
    return 0;
}
