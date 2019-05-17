#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFSIZE 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >=0 );

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    peer_addr.sin_port = htons(8000);
    int ret = connect(sockfd, (struct sockaddr*)&peer_addr, sizeof(peer_addr));
    assert(ret >= 0);

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    recv(sockfd, &local_addr.sin_addr, sizeof(local_addr.sin_addr), 0);
    recv(sockfd, &local_addr.sin_port, sizeof(local_addr.sin_port), 0);
    printf("My ip: %s    port: %d\n", inet_ntoa(local_addr.sin_addr), ntohs(local_addr.sin_port));

    close(sockfd);

    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    opt = 1;
    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ret = bind(listen_sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    assert(ret >= 0);
    printf("Bind finish!\n");

    ret = listen(listen_sockfd, 5);
    assert(ret >= 0);
    printf("Listen finish!\n");

    socklen_t addr_len = sizeof(peer_addr);
    sockfd = accept(listen_sockfd, (struct sockaddr*)&peer_addr, &addr_len);
    printf("new connection ip: %s    port: %d\n", inet_ntoa(peer_addr.sin_addr), peer_addr.sin_port);

    char recv_buf[BUFFSIZE] = {0};
    char send_buf[BUFFSIZE] = {0};
    while(1)
    {
        bzero(send_buf, sizeof(send_buf));
        bzero(recv_buf, sizeof(recv_buf));

        recv(sockfd, recv_buf, sizeof(recv_buf), 0);
        printf("peer say:# %s\n", recv_buf);
        printf("Please enter:> ");
        scanf("%s", send_buf);
        send(sockfd, send_buf, strlen(send_buf), 0);
    }
    
    close(sockfd);
    return 0;
}
