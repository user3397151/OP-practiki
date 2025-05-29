#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    char *student_info = "Шевченко Святослав Ильич ККСО-26-24 1 курс";
    send(sock, student_info, strlen(student_info), 0);
    printf("Данные студента отправлены серверу\n");

    read(sock, buffer, BUFFER_SIZE);
    printf("Ответ сервера: %s\n", buffer);

    close(sock);
    return 0;
}
