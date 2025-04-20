#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#endif
#include <stdlib.h>          // EXIT_ codes
#include <stdbool.h>         // bool
#include <stdio.h>           // printf, scanf
#include <string.h>          // strlen, strcmp



#include "udp3winver.h"            // udp3 library

#define LOCAL_PORT  5000     // port from which this program receives data
#define REMOTE_PORT 5000     // port to which this program sends data

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    #ifdef _WIN32
    WSADATA wsaData;
    int wsaInit = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaInit != 0)
    {
        printf("WSAStartup failed: %d\n", wsaInit);
        return EXIT_FAILURE;
    }
    #endif
    char* remoteIp;
    char str[100];
    bool quit = false;

    // Verify arguments are good
    bool goodArguments = (argc == 2);
    if (goodArguments)
    {
        remoteIp = argv[1];
    }
    else
    {
        printf("usage: chat IPV4_ADDRESS\n");
        printf("  where:\n");
        printf("  IPV4_ADDRESS is address of the remote machine\n");
        exit(EXIT_FAILURE);
    }

    // Open listener port
    // TODO: Open local udp listener port
    // TODO: Show an error and exit if the port cannot be opened
    bool openListener = openUdpListenerPort(REMOTE_PORT);
    {
        if (!openListener)
        {
            printf("Opening local udp listener port failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Print heading so that sent and received messages are in different columns
    printf("Sent\t\t\t\t\t\tReceived\n");
    
    // Start chat loop until someone types a string starting with QUIT
    while(!quit)
    {
        // send string
        // TODO: Add code:
        // if keyboard has been hit:
        // - read a string from the keyboard and send
        // - send the string to the remote port
        // - leave while loop if a string starting with QUIT was sent
        if (_kbhit())
        {
            fgets(str, 100, stdin);
            bool send = sendUdpData(remoteIp, REMOTE_PORT, str);
            if (send){
                if ((!(strncmp(str, "QUIT", 4)))||(!(strncmp(str, "quit", 4))))
                {
                    quit=true;
                }
            }
        }
        // receive text
        // TODO: Add code:
        // if data is received:
        // - print to the right column
        // - leave while loop if a string starting with QUIT is received
            bool receive = receiveUdpData(str, 100, 10);
            if (receive)
            {
                printf("\t\t\t\t\t\t%s", str);
                if ((!(strncmp(str, "QUIT", 4)))||(!(strncmp(str, "quit", 4))))
                {
                    quit=true;
                }
            }
    }
    printf("\nSession Terminated\n");
    // TODO: Close udp listener port
    closeUdpListenerPort();
    #ifdef _WIN32
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
