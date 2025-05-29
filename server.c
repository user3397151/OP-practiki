#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080
#define MAX_MESSAGE_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    int reuse_addr = 1;
    socklen_t addr_len = sizeof(server_addr);
    char client_message[MAX_MESSAGE_SIZE] = {0};

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                 &reuse_addr, sizeof(reuse_addr))) {
        perror("Ошибка настройки сокета");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DEFAULT_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Ошибка listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен на порту %d. Ожидание подключений...\n", DEFAULT_PORT);

    client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &addr_len);
    if (client_socket < 0) {
        perror("Ошибка accept");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = read(client_socket, client_message, MAX_MESSAGE_SIZE - 1);
    if (bytes_read < 0) {
        perror("Ошибка чтения");
    } else {
        printf("Получено сообщение (%zd байт): %s\n", bytes_read, client_message);
    }

    const char *server_response = "Данные успешно получены сервером!";
    ssize_t bytes_sent = send(client_socket, server_response, strlen(server_response), 0);
    if (bytes_sent < 0) {
        perror("Ошибка отправки");
    } else {
        printf("Ответ (%zd байт) успешно отправлен клиенту\n", bytes_sent);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
