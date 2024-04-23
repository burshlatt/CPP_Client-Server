#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define MY_PORT_NUM 62324

static void die(const char *s) {
    perror(s);
    exit(1);
}

static void server() {
    int in = 0;
    int ret = 0;
    int flags = 0;
    int conn_fd = 0;
    int listen_fd = 0;
    struct sctp_sndrcvinfo sndrcvinfo;

    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(MY_PORT_NUM),
    };

    struct sctp_initmsg initmsg = {
        .sinit_num_ostreams = 5,
        .sinit_max_instreams = 5,
        .sinit_max_attempts = 4,
    };

    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (listen_fd < 0)
        die("socket");

    ret = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    if (ret < 0)
        die("bind");

    ret = setsockopt(listen_fd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));

    if (ret < 0)
        die("setsockopt");

    ret = listen(listen_fd, initmsg.sinit_max_instreams);

    if (ret < 0)
        die("listen");

    while (true) {
        char buffer[1024];

        printf("Waiting for connection\n");
        fflush(stdout);

        conn_fd = accept(listen_fd, (struct sockaddr *) NULL, NULL);
        if(conn_fd < 0)
            die("accept()");

        printf("New client connected\n");
        fflush(stdout);

        in = sctp_recvmsg(conn_fd, buffer, sizeof(buffer), NULL, 0, &sndrcvinfo, &flags);
        if (in > 0) {
            printf("Received data: %s\n", buffer);
            fflush(stdout);
        }

        close(conn_fd);
    }
}

int main() {
    server();
    return 0;
}
