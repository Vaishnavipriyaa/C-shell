#include "headers.h"

void die(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int connectToServer(const char* ip_address, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("socket");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &(server_addr.sin_addr));

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        die("connect");
    }

    return sockfd;
}

void fetchManPage(const char* command_name) {
    char request[512];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHOST: man.he.net\r\n\r\n", command_name);

    struct hostent* host = gethostbyname("man.he.net");
    if (host == NULL) {
        die("gethostbyname");
    }

    struct in_addr** addr_list = (struct in_addr**)host->h_addr_list;
    char* ip_address = inet_ntoa(*addr_list[0]);

    int sockfd = connectToServer(ip_address, 80);

    if (send(sockfd, request, strlen(request), 0) == -1) {
        die("send");
    }

    char buffer[1024];
    int bytes_received;
    int startPrinting = 0;  
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';

        int i = 0, j = 0;
        int insideTag = 0;

        while (buffer[i]) {
            if (buffer[i] == '<') {
                insideTag = 1;
            } else if (buffer[i] == '>') {
                insideTag = 0;
            } else if (!insideTag) {
                buffer[j++] = buffer[i];
            }
            i++;
        }
        buffer[j] = '\0';

        if (startPrinting) {
            printf("%s", buffer);
        } else if (strstr(buffer, "NAME") != NULL) {
            startPrinting = 1;
            char* nameStart = strstr(buffer, "NAME");
            if (nameStart != NULL) {
                printf("%s", nameStart);
            }
        }
    }

    close(sockfd);
}


