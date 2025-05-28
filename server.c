#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
int server_fd, new_socket;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};

server_fd = socket(AF_INET, SOCK_STREAM, 0);
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);

bind(server_fd, (struct sockaddr *)&address, sizeof(address));
listen(server_fd, 3);

printf("Сервер запущен и ожидает подключения...\n");

new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
read(new_socket, buffer, BUFFER_SIZE);
printf("Получено сообщение от клиента: %s\n", buffer);

char *response = "Данные получены!";
send(new_socket, response, strlen(response), 0);
printf("Ответ отправлен\n");

close(new_socket);
close(server_fd);
return 0;
}
