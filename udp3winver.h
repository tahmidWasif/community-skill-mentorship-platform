#ifndef UDP3_H
#define UDP3_H

#include <stdbool.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

bool sendUdpData(const char ipv4Address[], int port, const char str[]);
bool openUdpListenerPort(int port);
bool receiveUdpData(char str[], int str_length, int timeout_ms);
void closeUdpListenerPort(void);

#ifdef __cplusplus
}
#endif

#endif  // UDP3_H