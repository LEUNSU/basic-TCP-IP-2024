#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRUE 1
#define BUF_SIZE 2048
#define MAX_IMG_SIZE 700000

void send_webpage(int clnt_sock);
void send_image(int clnt_sock);

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sin_len = sizeof(clnt_addr);
    int serv_sock, clnt_sock;
    char buf[BUF_SIZE];
    int option = TRUE;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    if (listen(serv_sock, 5) == -1) {
        perror("listen() error");
        exit(1);
    }

    printf("Server started. Listening on port %s\n", argv[1]);

    while (1) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &sin_len);
        if (clnt_sock == -1) {
            perror("accept() error");
            continue;
        }
        printf("New client connection from %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        // Read request
        ssize_t read_size = read(clnt_sock, buf, BUF_SIZE);
        if (read_size <= 0) {
            perror("read() error or connection closed");
            close(clnt_sock);
            continue;
        }
        buf[read_size] = '\0'; // Null terminate the string
        printf("Request:\n%s\n", buf);

        // Check if the request is for the image
        if (strstr(buf, "GET /puppy.png") != NULL) {
            send_image(clnt_sock);
        } else {
            send_webpage(clnt_sock);
        }

        close(clnt_sock);
        printf("Connection closed for %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    }

    close(serv_sock);
    return 0;
}

void send_webpage(int clnt_sock) {
    char webpage[] = "HTTP/1.1 200 OK\r\n"
                     "Server: Linux Web Server\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html><head><title>My Web Page</title>\r\n"
                     "<link rel=\"icon\" href=\"data:,\">\r\n"
                     "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                     "<body><center><h1>Hello world!!</h1><br>\r\n"
                     "<img src=\"puppy.png\"></center></body></html>\r\n";

    ssize_t sent_size = write(clnt_sock, webpage, strlen(webpage));
    if (sent_size == -1) {
        perror("write() error");
    }
}

void send_image(int clnt_sock) {
    char img_buf[MAX_IMG_SIZE];
    ssize_t img_size;
    int fdimg;

    fdimg = open("puppy.png", O_RDONLY);
    if (fdimg == -1) {
        perror("open() error");
        return;
    }

    img_size = read(fdimg, img_buf, sizeof(img_buf));
    if (img_size == -1) {
        perror("read() error");
        close(fdimg);
        return;
    }

    char header[BUF_SIZE];
    snprintf(header, sizeof(header), "HTTP/1.1 200 OK\r\n"
                                      "Server: Linux Web Server\r\n"
                                      "Content-Type: image/png\r\n"
                                      "Content-Length: %zd\r\n\r\n", img_size);

    ssize_t sent_header_size = write(clnt_sock, header, strlen(header));
    if (sent_header_size == -1) {
        perror("write() error");
        close(fdimg);
        return;
    }

    ssize_t sent_img_size = write(clnt_sock, img_buf, img_size);
    if (sent_img_size == -1) {
        perror("write() error");
        close(fdimg);
        return;
    }

    close(fdimg);
}
