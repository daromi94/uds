#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_FILE_PATH "/tmp/uds.sock"
#define DATA "Fear is the mind-killer. Fear is the little-death that brings total obliteration.\n"

void remove_stale_socket_file(const char *socket_file_path)
{
    if (unlink(socket_file_path) == -1)
    {
        if (errno != ENOENT)
        {
            perror("removing stale socket file");
            exit(EX_OSERR);
        }
    }
}

void remove_socket_file(const char *socket_file_path)
{
    if (unlink(socket_file_path) == -1)
    {
        perror("removing socket file");
        exit(EX_OSERR);
    }
}

int create_socket()
{
    const int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("creating socket");
        exit(EX_OSERR);
    }
    return socket_fd;
}

void close_socket(const int socket_fd)
{
    if (close(socket_fd) == -1)
    {
        perror("closing socket");
        exit(EX_OSERR);
    }
}

void bind_socket_name(const int socket_fd, const char *socket_file_path)
{
    struct sockaddr_un socket_address;
    socket_address.sun_family = AF_UNIX;
    strcpy(socket_address.sun_path, socket_file_path);

    if (bind(socket_fd, (const struct sockaddr *)&socket_address, sizeof(struct sockaddr_un)) == -1)
    {
        perror("binding socket");
        close_socket(socket_fd);
        exit(EX_OSERR);
    }
}

int main(int argc, char *argv[])
{
    const char *socket_file_path = SOCKET_FILE_PATH;

    remove_stale_socket_file(socket_file_path);

    int socket_fd = create_socket();
    fprintf(stdout, "socket created (fd: %d)\n", socket_fd);

    bind_socket_name(socket_fd, socket_file_path);
    fprintf(stdout, "socket bound to file (path: %s)\n", socket_file_path);

    close_socket(socket_fd);
    fprintf(stdout, "socket closed (fd: %d)\n", socket_fd);

    remove_socket_file(socket_file_path);
    fprintf(stdout, "socket file path removed (path: %s)\n", socket_file_path);

    return EX_OK;
}
