#include <stdio.h>
#include <sysexits.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds.sock"

int start()
{
    unlink(SOCKET_PATH);

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        // TODO: write error to stderr
        return EX_IOERR;
    }

    return EX_OK;
}

int main(int argc, char *argv[])
{
    printf("starting server...\n");

    int exit_status = start();

    return exit_status;
}
