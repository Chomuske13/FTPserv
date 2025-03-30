#include "ftp_server.h"
#include <stdlib.h>

void compress_to_tar_gz(const char *dirpath, const char *output_file) {
    char command[BUFFER_SIZE * 2];
    snprintf(command, BUFFER_SIZE * 2, "tar -czf %s %s", output_file, dirpath);
    system(command);
}

void extract_tar_gz(const char *filename, const char *output_dir) {
    char command[BUFFER_SIZE * 2];
    snprintf(command, BUFFER_SIZE * 2, "mkdir -p %s && tar -xzf %s -C %s", output_dir, filename, output_dir);
    system(command);
}