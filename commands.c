#include "ftp_server.h"

void process_command(int sock, const char *command, const char *arg1, const char *arg2) {
    char response[BUFFER_SIZE];

    if (strcmp(command, "GET") == 0) {
        if (access(arg1, F_OK) == -1) {
            snprintf(response, BUFFER_SIZE, "550 File %s not found\r\n", arg1);
            send_response(sock, response);
        } else {
            snprintf(response, BUFFER_SIZE, "150 Opening data connection for %s\r\n", arg1);
            send_response(sock, response);
            send_file(sock, arg1);
            snprintf(response, BUFFER_SIZE, "226 Transfer complete for %s\r\n", arg1);
            send_response(sock, response);
        }
    } else if (strcmp(command, "PUT") == 0) {
        snprintf(response, BUFFER_SIZE, "150 Ready to receive %s\r\n", arg1);
        send_response(sock, response);
        if (recv_file(sock, arg1) == 0) {
            snprintf(response, BUFFER_SIZE, "226 Transfer complete for %s\r\n", arg1);
            send_response(sock, response);
        } else {
            snprintf(response, BUFFER_SIZE, "550 Failed to receive %s\r\n", arg1);
            send_response(sock, response);
        }
    } else if (strcmp(command, "LIST") == 0) {
        list_files(sock);
    } else if (strcmp(command, "COMPRESS") == 0) {
        if (access(arg1, F_OK) == -1) {
            snprintf(response, BUFFER_SIZE, "550 Directory %s not found\r\n", arg1);
            send_response(sock, response);
        } else {
            char output_file[PATH_MAX_LEN];
            if (strlen(arg2) == 0) {
                snprintf(output_file, PATH_MAX_LEN, "%s.tar.gz", arg1);
            } else {
                strncpy(output_file, arg2, PATH_MAX_LEN);
            }
            compress_to_tar_gz(arg1, output_file);
            snprintf(response, BUFFER_SIZE, "226 Archive created: %s\r\n", output_file);
            send_response(sock, response);
        }
    } else if (strcmp(command, "EXTRACT") == 0) {
        if (access(arg1, F_OK) == -1) {
            snprintf(response, BUFFER_SIZE, "550 File %s not found\r\n", arg1);
            send_response(sock, response);
        } else {
            char output_dir[PATH_MAX_LEN];
            if (strlen(arg2) == 0) {
                snprintf(output_dir, PATH_MAX_LEN, "extracted_%s", arg1);
            } else {
                strncpy(output_dir, arg2, PATH_MAX_LEN);
            }
            extract_tar_gz(arg1, output_dir);
            snprintf(response, BUFFER_SIZE, "226 Archive extracted to: %s\r\n", output_dir);
            send_response(sock, response);
        }
    } else if (strcmp(command, "QUIT") == 0) {
        send_response(sock, "221 Goodbye!\r\n");
    } else {
        send_response(sock, "500 Unknown command\r\n");
    }
}