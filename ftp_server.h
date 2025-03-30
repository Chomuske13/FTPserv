#ifndef FTP_SERVER_H
#define FTP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>
#include <time.h>
#include <pthread.h>

#define PORT 2121
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define CMD_MAX_LEN 50
#define PATH_MAX_LEN 256

typedef struct {
    int sock;
    struct sockaddr_in address;
    int addr_len;
} client_t;

// Основные функции сервера
void start_server();
void stop_server();
void show_server_status();

// Функции для работы с клиентами
void *handle_client(void *arg);
void send_response(int sock, const char *response);

// Функции для работы с файлами
int send_file(int sock, const char *filename);
int recv_file(int sock, const char *filename);
void list_files(int sock);

// Функции для работы с архивами
void compress_to_tar_gz(const char *dirpath, const char *output_file);
void extract_tar_gz(const char *filename, const char *output_dir);

// Обработка команд
void process_command(int sock, const char *command, const char *arg1, const char *arg2);

// Интерфейс
void init_interface();
void run_interface();
void show_help();

#endif