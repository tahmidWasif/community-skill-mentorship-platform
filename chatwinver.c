#ifdef _WIN32
#include <ctype.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#endif
#include <stdlib.h>          
#include <stdbool.h>         
#include <stdio.h>           
#include <string.h>          



#include "udp3winver.h"            // udp3 library

#define LOCAL_PORT  5000     // port from which this program receives data
#define REMOTE_PORT 5000     // port to which this program sends data

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
    char str[101];
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
    bool openListener = openUdpListenerPort(REMOTE_PORT);
    {
        if (!openListener)
        {
            printf("Opening local udp listener port failed\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nNetwork Chat Established (Max character length: 100).\n(Type 'quit' to end chat).\n\n");
    // Print heading so that sent and received messages are in different columns
    printf("Sent\t\t\t\t\t\t\t\t\t\t\tReceived\n\n");
    
    // Start chat loop until someone types a string starting with QUIT
    while(!quit)
    {
        // if keyboard has been hit, send string, quit to exit
        if (_kbhit())
        {
            while (1){      //validating input length
                fgets(str, 101, stdin);
                
                if (strlen(str) <= 99) {
                    break;
                }
                else {
                    printf("\nInput too long. Max limit: 100 characters\n\n ");
                    int c;
                    // clears input buffer
                    while ((c = getchar()) != '\n' && c != EOF);
                    
                }
            }
            bool send = sendUdpData(remoteIp, REMOTE_PORT, str);
            
            // turns each character of str to uppercase to compare with QUIT
            for (int i = 0; str[i] != '\0'; i++){
                str[i] = toupper(str[i]);
            }
            if (send){
                if ((!(strncmp(str, "QUIT", 4))))
                {
                    quit=true;
                }
            }
        }
        // receive text
        // if data is received:
        // print to the right column
        // leave while loop if a string starting with QUIT is received
            bool receive = receiveUdpData(str, 100, 10);
            if (receive)
            {
                printf("\t\t\t\t\t\t\t\t\t\t\t %s", str);
                for (int i = 0; str[i] != '\0'; i++){
                    str[i] = toupper(str[i]);
                }
                if ((!(strncmp(str, "QUIT", 4))))
                {
                    quit=true;
                }
            }
    }
    system("cls");
    printf("Session Terminated\n");
    // Closes udp listener port
    closeUdpListenerPort();
    #ifdef _WIN32
    WSACleanup();
    #endif
    return EXIT_SUCCESS;
}
