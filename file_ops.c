#include "ftp_server.h"

int send_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        if (send(sock, buffer, bytes_read, 0) < 0) {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

int recv_file(int sock, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return -1;
    }

    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) {
        if (bytes_received < 0) {
            fclose(file);
            return -1;
        }
        fwrite(buffer, 1, bytes_received, file);
    }

    fclose(file);
    return 0;
}

void list_files(int sock) {
    DIR *dir;
    struct dirent *ent;
    char response[BUFFER_SIZE] = {0};
    char file_list[BUFFER_SIZE * 10] = {0};

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                strncat(file_list, ent->d_name, BUFFER_SIZE * 10 - strlen(file_list) - 1);
                strncat(file_list, "\r\n", BUFFER_SIZE * 10 - strlen(file_list) - 1);
            }
        }
        closedir(dir);

        send_response(sock, "150 Listing files\r\n");
        send(sock, file_list, strlen(file_list), 0);
        send_response(sock, "226 Transfer complete\r\n");
    } else {
        send_response(sock, "550 Failed to list directory\r\n");
    }
}