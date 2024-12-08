#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(9999),
        .sin_addr.s_addr = INADDR_ANY
    };

    connect(sockfd, (struct sockaddr*)&address, sizeof(address));

    struct pollfd fds[2] = {
        {
            .fd = 0,
            .events = POLLIN,
            .revents = 0
        },
        {
            .fd = sockfd,
            .events = POLLIN,
            .revents = 0
        }
    };

    for (;;) {
        char buffer[256] = {0};

        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(sockfd, buffer, 255, 0); // no need of flags
        } else if (fds[1].revents & POLLIN) {
            recv(sockfd, buffer, 255, 0);
            printf("%s\n", buffer);
        }
    }

    return 0;
}