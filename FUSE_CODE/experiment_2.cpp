#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h> 

int main() {
    struct timespec start, end;
    double elapsed_time;
    int iterations = 10000;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; ++i) {
        int fd = open("/tmp/FUSE_MOUNT/foo", O_RDONLY); 
        if (fd == -1) {
            perror("Error opening file");
            return 1;
        }
        close(fd); 
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Opened the file %d times\n", iterations);
    printf("Total time: %.3f seconds\n", elapsed_time);
    return 0;
}
