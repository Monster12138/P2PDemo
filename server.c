#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(listen_sockfd >= 0);

    int opt;
    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(8000);

    int ret = bind(listen_sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    assert(ret >= 0);

    ret = listen(listen_sockfd, 5);
    assert(ret >= 0);

    struct sockaddr_in peer_addr1;
    socklen_t addr_len1 = sizeof(peer_addr1);
    int clientfd1 = accept(listen_sockfd, (struct sockaddr*)&peer_addr1, &addr_len1);
    printf("client ip: %s    port: %d\n", inet_ntoa(peer_addr1.sin_addr), ntohs(peer_addr1.sin_port));

    send(clientfd1, &peer_addr1.sin_addr, sizeof(peer_addr1.sin_addr), 0);
    send(clientfd1, &peer_addr1.sin_port, sizeof(peer_addr1.sin_port), 0);
    printf("Send finish!\n");
    
    struct sockaddr_in peer_addr2;
    socklen_t addr_len2 = sizeof(peer_addr2);
    int clientfd2 = accept(listen_sockfd, (struct sockaddr*)&peer_addr2, &addr_len2);
    printf("client ip: %s    port: %d\n", inet_ntoa(peer_addr2.sin_addr), ntohs(peer_addr2.sin_port));

    send(clientfd2, &peer_addr1.sin_addr, sizeof(peer_addr1.sin_addr), 0);
    send(clientfd2, &peer_addr1.sin_port, sizeof(peer_addr1.sin_port), 0);
    printf("Send finish!\n");

    close(listen_sockfd);
    close(clientfd1);
    close(clientfd2);

    return 0;
}
