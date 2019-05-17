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

    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    peer_addr.sin_port = htons(8000);
    int ret = connect(sockfd, (struct sockaddr*)&peer_addr, sizeof(peer_addr));
    assert(ret >= 0);

    recv(sockfd, &peer_addr.sin_addr, sizeof(peer_addr.sin_addr), 0);
    recv(sockfd, &peer_addr.sin_port, sizeof(peer_addr.sin_port), 0);
    printf("client ip: %s    port: %d\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
    close(sockfd);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    ret = connect(sockfd, (struct sockaddr*)&peer_addr, sizeof(peer_addr));
    assert(ret >= 0);
    printf("Connect success!\n");

    char recv_buf[BUFFSIZE] = {0};
    char send_buf[BUFFSIZE] = {0};
    while(1)
    {
        bzero(send_buf, sizeof(send_buf));
        bzero(recv_buf, sizeof(recv_buf));

        printf("Please enter:> ");
        scanf("%s", send_buf);
        send(sockfd, send_buf, strlen(send_buf), 0);
        recv(sockfd, recv_buf, sizeof(recv_buf), 0);
        printf("peer say:# %s\n", recv_buf);
    }
    
    close(sockfd);
    return 0;
}
