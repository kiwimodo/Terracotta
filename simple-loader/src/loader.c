#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include "blob.h"

#define SHM_NAME "shm"

#ifndef USE_SHM
#define __NR_memfd_create 319

static inline int memfd_create(const char *name, unsigned int flags) {
	return syscall(__NR_memfd_create, name, flags);
}

// Create and opens anonymous file.
// Returns a file descriptor that refers to it.
int open_fd(void) {
    int fd;
    fd = memfd_create(SHM_NAME, 1);
    
    if(fd < 0) exit(-1);

    return fd;
}

#else

// Create and opens POSIX shared memory object.
// Returns a file descriptor that refers to it.
int open_fd(void) {
    int fd;
    fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, S_IRWXU);
    
    if(fd < 0) exit(-1);

    return fd;
}

#endif

void write_to_ram(int fd, const unsigned char *bytes, const unsigned int length) {
    if(write(fd, bytes, length) != length) {
        close(fd);
        exit(-1);
    }
}

#ifndef USE_SHM

// Loads the shared object found in blob.h into the anonymous file.
// Returns a handle to the library.
void *load_so(int fd) {
    char path[1024];
    void *handle;

    snprintf(path, 1024, "/proc/%d/fd/%d", getpid(), fd);
    handle = dlopen(path, RTLD_NOW);
	
    if (!handle) exit(-1);

    return handle;
}

#else

// Loads the shared object found in blob.h into the shared object.
// Returns a handle to the library.
void *load_so(int fd) {
    char path[1024];
    void *handle;
    
    close(fd);
    snprintf(path, 1024, "/dev/shm/%s", SHM_NAME);
    handle = dlopen(path, RTLD_NOW);
    remove("/dev/shm/shm"); // TODO: Is it still possible to call shm_unlink?
	
    if (!handle) exit(-1);

    return handle;
}

#endif

int main(void) {
    void *handle = NULL;
    int fd = -1;
    
    // Load the shared object into RAM
    fd = open_fd();
    write_to_ram(fd, blob, blob_len);
    handle = load_so(fd);
    
    // Load the run function from the in process library and call it
    void (*fptr)(void) = (void(*)(void))dlsym(handle, "run");
    (*fptr)();

    exit(0);
}