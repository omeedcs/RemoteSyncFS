#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> 
#include <cstring>

int main() {
    struct timespec start, end;
    double elapsed_time;
    int iterations = 200; 
    const char *data = "Hello, World!"; 
    ssize_t data_len = strlen(data);
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; ++i) {
        int fd = open("/tmp/FUSE_MOUNT/foo", O_WRONLY); 
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }
        if (write(fd, data, data_len) != data_len) {
            perror("Error writing to file");
            close(fd); 
            return 1;
        }
        close(fd); 
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Wrote to the file %d times\n", iterations);
    printf("Total time: %.3f seconds\n", elapsed_time);
    return 0;
}
