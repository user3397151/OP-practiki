#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка подключения");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    const char *student_data = "Шевченко Святослав Ильич ККСО-26-24 1 курс";
    ssize_t bytes_sent = send(client_socket, student_data, strlen(student_data), 0);
    if (bytes_sent < 0) {
        perror("Ошибка отправки данных");
    } else {
        printf("Данные студента успешно отправлены серверу\n");
    }

    char response_buffer[MAX_BUFFER_SIZE] = {0};
    ssize_t bytes_received = read(client_socket, response_buffer, MAX_BUFFER_SIZE - 1);
    if (bytes_received < 0) {
        perror("Ошибка чтения данных");
    } else {
        printf("Ответ сервера: %s\n", response_buffer);
    }

    close(client_socket);
    return 0;
}
