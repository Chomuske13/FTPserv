#include "ftp_server.h"
#include <signal.h>

static int server_running = 0;
static int server_fd = -1;

void start_server() {
    if (server_running) {
        printf("Server is already running\n");
        return;
    }

    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    server_running = 1;
    printf("FTP Server started on port %d\n", PORT);
}

void stop_server() {
    if (!server_running) {
        printf("Server is not running\n");
        return;
    }

    close(server_fd);
    server_running = 0;
    printf("Server stopped\n");
}

void show_server_status() {
    if (server_running) {
        printf("Server status: RUNNING on port %d\n", PORT);
    } else {
        printf("Server status: STOPPED\n");
    }
}

void handle_signal(int sig) {
    printf("\nReceived signal %d, shutting down server...\n", sig);
    stop_server();
    exit(0);
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    init_interface();
    run_interface();

    return 0;
}