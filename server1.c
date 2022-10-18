#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>   // This contains defn for data types used in system calls
#include<sys/socket.h>  // Contains Defn for structures used in socket
#include<netinet/in.h>  // Contains structures etc for internet domain addresses
#include<math.h>  

// A function to stdout error
void error(const char* str){
    perror(str);
    exit(1);
}

void checkInputs(int argc) {
   if(argc < 2) {
   	fprintf(stderr, "Error : invalid input\n");
        exit(1);
   }
   return;
}


int main(int argc, char** argv){

    

    int sock_fd, newsock_fd;
    int port_number, varible;
    
    checkInputs(argc);
    
    // Struct for server address, client address
    struct sockaddr_in server_addr , client_addr;
    
    // Client length
    socklen_t client_length;
    
    // String to take input from client
    char buff[255];
    
    
    // Calling socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    //Error handeling
    if( sock_fd < 0) {
        error("Error in creating socket.");
    }
    
    //Clearing buffer array
    bzero((char *) &server_addr, sizeof(server_addr));
    //Taking portnumber
    port_number = atoi(argv[1]);
    
    //Setting socket parameters
    server_addr.sin_family = AF_INET;
    
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    server_addr.sin_port = htons(port_number);

    // Binding as well as checking if bind has been successful
    if(bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        error("Binding Failed.");
    }
    printf("Binding Succesful...\n");

    // Listening 
    varible = listen(sock_fd, 10);
     
    client_length = sizeof(client_addr);
    //Error
    if(varible < 0){
            error("Error in Listening..");
    }
    printf("Listening...\n");

    // Accepting connection
    newsock_fd = accept(sock_fd, (struct sockaddr *) &client_addr, &client_length);
    
    // Defining variable for calculator
    int num1, num2;
    long long int result = 0;
    
    // Error in connecting 
    if (newsock_fd < 0){
        error("Error in connecting.");
    } else {
       close(sock_fd);
    }
    
    
    
    // Ch is for operations (+, -, *, /, %, ^)
    char ch;

    while(1) {
    
        // clearing buffer
        bzero(buff, 255);
        
        //Reading from client
        varible = read(newsock_fd, buff, 255);
        
        // Error handeling
        if(varible < 0){
            error("Error in reading.");
        }

        printf("Client : %s", buff);
        
        // Quit if client says END
        int quitStatus = strncmp("END", buff, 3);
        
        if (quitStatus == 0) {
            close(newsock_fd);
            close(sock_fd);
        }
        
        //Creating tokens from input string
        char * token = strtok(buff, " ");
        
        // First Number 
        num1 = atoi(token);
        token = strtok(NULL, " ");
        
        // Operation
        ch = token[0];
        token = strtok(NULL, " ");
         
        //S econd Number
        num2 = atoi(token);
        token = strtok(NULL, " ");
        
        // Calculation of answer for different cases
        switch(ch){
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                result = num1 / num2;
                break;
            case '%':
                result = num1 % num2;
                break;
            case '^':
                result = pow(num1, num2);
                break;
            default :
                result = 0;
                break;
        }

        // Returning Answer
        varible = write(newsock_fd, &result, sizeof(int));
        
        //Error handeling
        if(varible < 0) {
            error("Error in writing.");
        }
    }


    return 0;
}
