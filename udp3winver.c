#define _WIN32_WINNT 0x0600  // Enable newer Windows API (needed for inet_pton)

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "udp3winver.h"

static SOCKET listen_sockfd;

bool sendUdpData(const char ipv4Address[], int port, const char str[])
{
    SOCKET sockfd;
    struct sockaddr_in remote_addr;
    int count;
    bool ok = true;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ok = sockfd != INVALID_SOCKET;

    if (ok)
    {
        memset(&remote_addr, 0, sizeof(remote_addr));
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ipv4Address, &remote_addr.sin_addr) <= 0)
        {
            printf("Invalid IP address format: %s\n", ipv4Address);
            closesocket(sockfd);
            return false;
        }

        count = sendto(sockfd, str, (int)strlen(str) + 1, 0,
                       (struct sockaddr*)&remote_addr, sizeof(remote_addr));
        ok = count != SOCKET_ERROR;
        closesocket(sockfd);
    }

    return ok;
}

bool openUdpListenerPort(int listenPort)
{
    struct sockaddr_in local_addr;
    int opt = 1;

    listen_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (listen_sockfd == INVALID_SOCKET)
        return false;

    setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(listenPort);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR)
        return false;

    return true;
}

bool receiveUdpData(char str[], int str_length, int timeout_ms)
{
    struct sockaddr_in sender_addr;
    int sender_addr_len = sizeof(sender_addr);
    bool success = false;

    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(listen_sockfd, &readfds);

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int result = select(0, &readfds, NULL, NULL, &tv);
    if (result > 0 && FD_ISSET(listen_sockfd, &readfds))
    {
        int bytes = recvfrom(listen_sockfd, str, str_length, 0,
                             (struct sockaddr*)&sender_addr, &sender_addr_len);
        if (bytes != SOCKET_ERROR)
            success = true;
    }

    return success;
}

void closeUdpListenerPort()
{
    closesocket(listen_sockfd);
}
