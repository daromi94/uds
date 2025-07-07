#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_FILE_PATH "/tmp/uds.sock"
#define SOCKET_BACKLOG 100
#define DATA "Fear is the mind-killer. Fear is the little-death that brings total obliteration.\n"

typedef struct BoundSocket
{
    const int fd;
    const char *file_path;
} BoundSocket;

void remove_stale_socket_file(const char *file_path)
{
    if (unlink(file_path) == -1)
    {
        if (errno != ENOENT)
        {
            perror("removing stale socket file");
            exit(EX_OSERR);
        }
    }
}

void remove_socket_file(const char *file_path)
{
    if (unlink(file_path) == -1)
    {
        perror("removing socket file");
        exit(EX_OSERR);
    }
}

int create_socket()
{
    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("creating socket");
        exit(EX_OSERR);
    }
    return fd;
}

void close_socket(const int fd)
{
    if (close(fd) == -1)
    {
        perror("closing socket");
        exit(EX_OSERR);
    }
}

BoundSocket bind_socket(const int fd, const char *file_path)
{
    struct sockaddr_un socket_address;
    socket_address.sun_family = AF_UNIX;
    strcpy(socket_address.sun_path, file_path); // FIXME: buffer overflow

    if (bind(fd, (const struct sockaddr *)&socket_address, sizeof(struct sockaddr_un)) == -1)
    {
        perror("binding socket");
        close_socket(fd);
        exit(EX_OSERR);
    }

    return (BoundSocket){.fd = fd, .file_path = file_path};
}

void clean_bound_socket(const BoundSocket bound_socket)
{
    close_socket(bound_socket.fd);
    remove_socket_file(bound_socket.file_path);
}

int main(int argc, char *argv[])
{
    const char *socket_file_path = SOCKET_FILE_PATH;
    remove_stale_socket_file(socket_file_path);

    const int socket_fd = create_socket();
    fprintf(stdout, "socket created (fd: %d)\n", socket_fd);

    BoundSocket bound_socket = bind_socket(socket_fd, socket_file_path);
    fprintf(stdout, "socket bound to file (path: %s)\n", bound_socket.file_path);

    clean_bound_socket(bound_socket);

    return EX_OK;
}
