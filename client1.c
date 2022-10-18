
#include<sys/types.h>   
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


#define buffsize 255


void checkInputs(int argc) {
   if(argc < 3) {
   	fprintf(stderr, "Error : invalid input\n");
        exit(1);
   }
   return;
}

// Function to stdout error
void error(const char* str){
    perror(str);
    exit(1);
}

void createSocket(int *sock_fd) {
    *sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Error Handeling
    if(sock_fd < 0){
        error("Error in creating socket.");
    }
}

int main(int argc, char** argv) {
    
    // Defining sockets file descriptors
    
    int compareStatus;
    int sock_fd, port_number;
    int newsock_fd, varible;
    
    checkInputs(argc);
    
    // Calling socket
    createSocket(&sock_fd);
    
    // String to take input from client
    char buff[buffsize];
    
    // Struct for server address, client address
    struct sockaddr_in server_addr , client_addr;
    // Clearing the buffer
    bzero((char *) &server_addr, sizeof(server_addr));
    
    struct hostent *server;
    
    // Getting port number
    port_number = atoi(argv[2]);
    
    
    // Getting Server Info
    server = gethostbyname(argv[1]);
    
    // Error Handeling
    if(server == NULL) {
        error("No such Host.");
    }

    // Clearing the buffer
    bzero((char *)&server_addr, sizeof(server_addr));

    // Setting socket parameters
    server_addr.sin_family = AF_INET;
    
    bcopy((char *) server->h_addr, (char*)&server_addr.sin_addr.s_addr, server->h_length);
    
    server_addr.sin_port = htons(port_number);
    
    // Connecting to the server
    if(connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        error("Line busy.");
    }

    // result
    long long int result = 0;


    while(1) {
    
        // Clearing the buffer
        bzero(buff, 255);
        
        // Getting input for client
        fgets(buff, 255, stdin);
        
        // To quit client should enter END
        compareStatus = strncmp("END", buff, 3);
        
        //Error Handeling
        if (compareStatus == 0) {
            break;
        }
             
        // Writing
        varible = write(sock_fd, buff, strlen(buff));
        
        // Error Handeling
        if(varible < 0) {
            error("Error while writing.");
        }
        // Clearing the buffer
        bzero(buff, 255);
        
        // Reading answer from server
        read(sock_fd, &result, sizeof(int));

        // Printing the final answer
        printf("Server : The Answer is = %lld\n", result);

    }
    // Close the socket connection
    close(sock_fd);
    return 0;

}
