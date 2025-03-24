#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h> 
int main() {
    struct timespec start, end;
    double elapsed_time;
    int iterations = 100;
    char buf[100];
    int nb0, nb1;

    buf[0] = 9;
    buf[1] = 81;
    buf[2] = 'A';
    buf[3] = 'q';
    buf[4] = '0';

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; ++i) {
        int fd0 = open("/tmp/FUSE_MOUNT/foo", O_RDWR);
        if (fd0 == -1) {
            perror("Error opening file for writing");
            return 1;
        }
        int fd1 = open("/tmp/FUSE_MOUNT/foo", O_RDONLY);
        if (fd1 == -1) {
            perror("Error opening file for reading");
            return 1;
        }
        nb0 = write(fd0, buf, sizeof(buf));
        if (nb0 == -1) {
            perror("Error writing to file");
            return 1;
        }
        close(fd0);
        nb1 = read(fd1, buf, sizeof(buf));
        if (nb1 == -1) {
            perror("Error reading from file");
            return 1;
        }

        assert(buf[0] == 9);
        assert(buf[1] == 81);
        assert(buf[2] == 'A');
        assert(buf[3] == 'q');
        assert(buf[4] == '0');

        close(fd1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed_time = (end.tv_sec - start.tv_sec);
    elapsed_time += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Repeated write-read %d times\n", iterations);
    printf("Total time: %.3f seconds\n", elapsed_time);

    return 0;
}
