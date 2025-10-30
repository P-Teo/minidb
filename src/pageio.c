#include "pageio.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <stdint.h>
#define open_file _open
#define close_file _close
#define fsync_file _commit
#define lseek_file _lseek
#define read_file _read
#define write_file _write
#else
#include <unistd.h>
#include <fcntl.h>
#define open_file open
#define close_file close
#define fsync_file fsync
#define lseek_file lseek
#define read_file read
#define write_file write
#endif

static int data_fd = -1;

int open_data_file(const char *path)
{
#ifdef _WIN32
    data_fd = open_file(path, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE);
#else
    data_fd = open_file(path, O_RDWR | O_CREAT, 0644);
#endif
    if (data_fd < 0) {
        perror("open");
        return -1;
    }
    return 0;
}

void close_data_file()
{
    if (data_fd >= 0)
        close_file(data_fd);
    data_fd = -1;
}

int read_page(page_id pid, void *buffer)
{
    off_t offset = (off_t)pid * PAGE_SIZE;
    if (lseek_file(data_fd, offset, SEEK_SET) < 0) {
        perror("lseek read");
        return -1;
    }

    ssize_t r = read_file(data_fd, buffer, PAGE_SIZE);
    if (r < 0) {
        perror("read_page");
        return -1;
    }
    if (r < PAGE_SIZE)
        memset((char *)buffer + r, 0, PAGE_SIZE - r);
    return 0;
}

int write_page(page_id pid, const void *buffer)
{
    off_t offset = (off_t)pid * PAGE_SIZE;
    if (lseek_file(data_fd, offset, SEEK_SET) < 0) {
        perror("lseek write");
        return -1;
    }

    ssize_t w = write_file(data_fd, buffer, PAGE_SIZE);
    if (w != PAGE_SIZE) {
        perror("write_page");
        return -1;
    }
    fsync_file(data_fd);
    return 0;
}

page_id allocate_page()
{
    struct stat st;
    if (fstat(data_fd, &st) < 0) {
        perror("fstat");
        return 0;
    }
    off_t size = st.st_size;
    page_id new_pid = (page_id)(size / PAGE_SIZE);

    void *zero = calloc(1, PAGE_SIZE);
    if (!zero) return 0;

    if (write_page(new_pid, zero) < 0) {
        free(zero);
        return 0;
    }
    free(zero);
    return new_pid;
}
