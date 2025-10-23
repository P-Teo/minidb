#include "pageio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

static int data_fd=-1; //descriptor pt. fisier DB

int open_data_file (const char *path)
{
    data_fd=open(path, O_RDWR | O_CREAT, 0644);
    if(data_fd<0)
    {
        perror("open");
        return -1;
    }
    return 0;
}

void close_data_file()
{
    if(data_fd >=0 )
        close(data_fd);
    data_fd =-1;
}

int read_page(page_id pid, void *buffer)
{
    off_t offset =(off_t)pid*PAGE_SIZE;
    ssize_t r =pread(data_fd, buffer,PAGE_SIZE,offset);
    if(r<0)
    {
        perror("page_read");
        return -1;
    }
    if(r<PAGE_SIZE)
        memset((char*)buffer+r,0,PAGE_SIZE-r);
    return 0;
}

int write_page(page_id pid, const void* buffer)
{
    off_t offset =(off_t)pid*PAGE_SIZE;
    ssize_t w =pread(data_fd, buffer,PAGE_SIZE,offset);
    if(w!=PAGE_SIZE)
    {
        perror("page_write");
        return -1;
    }
    fsync(data_fd);
    return 0;
}

page_id allocate_page()
{
    struct stat st;
    if( fstat(data_fd, &st)<0)
    {
        perror("fstat");
        return 0;
    }
    off_t size=st.st_size;
    page_id new_pid=(page_id)(size/PAGE_SIZE);
    void*zero=calloc(1,PAGE_SIZE);
    write_page(new_pid,zero);
    free(zero);
    return new_pid;
}