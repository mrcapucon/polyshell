#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   int fin = 0;
   char buffer[256];
   char * tmp="#";

   
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[2]);
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(argv[1]);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }
   
   /* Now ask for a message from the user, this message
      * will be read by server
   */
   bzero(buffer,256);
   recv(sockfd,buffer,255,0);
   printf("%s",buffer);
   //printf("Please enter the message: ");
   while(fin!=1){
       bzero(buffer,256);
         recv(sockfd,buffer,255,0);
         printf("%s",buffer);
        bzero(buffer,256);
        strcat(buffer,tmp);
        fgets(buffer,255,stdin);
        if(strcmp(buffer,"quit")==0)
            fin = 1;
        strcat(buffer,tmp);
  
        n=send(sockfd, buffer, strlen(buffer),0);
        if(fin==1)
            return 0;
         bzero(buffer,256);
        recv(sockfd,buffer,255,0);
        printf("%s\n",buffer);

   }
  
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   /* Now read server response */
   bzero(buffer,256);
   n = read(sockfd, buffer, 255);
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
	
   printf("%s\n",buffer);
   return 0;
}