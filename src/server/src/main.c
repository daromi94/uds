#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds.sock"
#define DATA "Fear is the mind-killer. Fear is the little-death that brings total obliteration.\n"

int main(int argc, char *argv[])
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("creating socket");
        exit(EX_OSERR);
    }

    close(sock);

    return EX_OK;
}
