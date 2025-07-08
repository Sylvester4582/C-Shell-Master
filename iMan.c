#include "header.h"

#define PORT 80
#define BUFFER_SIZE 4096

void print_without_html_tags(const char *str){
    int inside_tag = 0;
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == '<') inside_tag = 1;
        else if (str[i] == '>') inside_tag = 0;
        else if (!inside_tag) putchar(str[i]);  
    }
}

void get_man_page(const char *command_name) {

    int sock;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[1024], response[BUFFER_SIZE];
    int bytes_read;
    char *response_body;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Socket creation failed");
        return;
    }

    server = gethostbyname("man.he.net");
    if(server == NULL){
        fprintf(stderr, "No such host\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return;
    }

    snprintf(request, sizeof(request), "GET /?topic=%s&section=all\r\nHost: man.he.net\r\nConnection: close\r\n\r\n", command_name);

    if(send(sock, request, strlen(request), 0) < 0){
        perror("Send failed");
        return;
    }

    int no_match_found = 0;

    while((bytes_read = recv(sock, response, BUFFER_SIZE - 1, 0)) > 0){
        response[bytes_read] = '\0';
        if(strstr(response,"No matches for")) no_match_found = 1;
        print_without_html_tags(response);    
    }
    
    if(no_match_found) printf(RED "No Match Found !\n" RESET);

    close(sock);
}

void iMan(char* command){
    if(strlen(command) == 4) return;
    
    char* term = strtok(command+4," ");
    get_man_page(term);
}