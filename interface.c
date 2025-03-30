#include "ftp_server.h"
#include <readline/readline.h>
#include <readline/history.h>

void init_interface() {
    printf("Archive FTP Server Management Console\n");
    printf("Type 'help' for available commands\n");
}

void show_help() {
    printf("\nAvailable commands:\n");
    printf("  start       - Start the FTP server\n");
    printf("  stop        - Stop the FTP server\n");
    printf("  status      - Show server status\n");
    printf("  exit        - Exit the program\n");
    printf("  help        - Show this help message\n");
}

void run_interface() {
    char *input;

    while (1) {
        input = readline("ftp> ");

        if (!input) break;

        if (strlen(input) > 0) {
            add_history(input);
        }

        if (strcmp(input, "start") == 0) {
            start_server();
        } else if (strcmp(input, "stop") == 0) {
            stop_server();
        } else if (strcmp(input, "status") == 0) {
            show_server_status();
        } else if (strcmp(input, "help") == 0) {
            show_help();
        } else if (strcmp(input, "exit") == 0) {
            free(input);
            stop_server();
            break;
        } else if (strlen(input) > 0) {
            printf("Unknown command. Type 'help' for available commands.\n");
        }

        free(input);
    }
}