#include "ftp_server.h"
#include <pthread.h>

void *handle_client(void *arg) {
    client_t *client = (client_t *)arg;
    char buffer[BUFFER_SIZE] = {0};
    char command[CMD_MAX_LEN] = {0};
    char arg1[PATH_MAX_LEN] = {0};
    char arg2[PATH_MAX_LEN] = {0};
    int bytes_read;

    send_response(client->sock, "220 Welcome to Archive FTP Server\r\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = recv(client->sock, buffer, BUFFER_SIZE, 0);

        if (bytes_read <= 0) {
            printf("Client disconnected\n");
            break;
        }

        printf("Received: %s", buffer);
        sscanf(buffer, "%s %s %s", command, arg1, arg2);
        process_command(client->sock, command, arg1, arg2);

        if (strcmp(command, "QUIT") == 0) {
            break;
        }
    }

    close(client->sock);
    free(client);
    pthread_exit(NULL);
}

void send_response(int sock, const char *response) {
    send(sock, response, strlen(response), 0);
}