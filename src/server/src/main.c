#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds.sock"
#define DATA "Fear is the mind-killer. Fear is the little-death that brings total obliteration.\n"

int main(int argc, char *argv[])
{
    // Clean up
    if (unlink(SOCKET_PATH) == -1)
    {
        if (errno != ENOENT)
        {
            perror("removing stale socket file");
            exit(EX_OSERR);
        }
    }

    // Create socket
    const int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("creating socket");
        exit(EX_OSERR);
    }
    fprintf(stdout, "socket created (fd: %d)\n", socket_fd);

    // Bind socket name
    const struct sockaddr_un socket_address = {.sun_family = AF_UNIX, .sun_path = SOCKET_PATH};

    if (bind(socket_fd, (const struct sockaddr *)&socket_address, sizeof(struct sockaddr_un)))
    {
        perror("binding socket");
        close(socket_fd);
        exit(EX_IOERR);
    }
    fprintf(stdout, "socket bound to '%s'\n", socket_address.sun_path);

    close(socket_fd);

    return EX_OK;
}
