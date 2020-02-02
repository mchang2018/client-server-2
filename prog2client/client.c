#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>

#include "packets.h"
#include "clientFunctions.h"


int main() {
    // declare variables
    int sockfd, pktLen;
    struct sockaddr_in serverAddr;
    struct Access_permission_request reqPkt;
    char buffer[1024];

    // create client socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        printf("Error creating client socket.\n");
        exit(1);
    }

    // initialize serverAddr information
    bzero((char*) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7573);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // use S0_RCVTIME0 to set a timeout value for input operations to complete
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) {
        perror("setsockopt failed\n");
    }

    // send 4 identification requests to the server (1 for each condition)
    for (int i = 1; i < 5; i++) {
        printf("Sending access permission request packet.\n");
        switch(i) {
            // Test case 1 - Subscriber doesn't exist (subscriber number not found)
            case 1:
                initializeAccessRequest(&reqPkt, i, 4, 4085546806);
                pktLen = buildAccessPacket(buffer, reqPkt);
                sendto(sockfd, buffer, pktLen, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
                break;
            // Test case 2 - Subscriber doesn't exist (subscriber number found, but technology doesn't match)
            case 2:
                initializeAccessRequest(&reqPkt, i, 3, 4085546805);
                pktLen = buildAccessPacket(buffer, reqPkt);
                sendto(sockfd, buffer, pktLen, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
                break;
            // Test case 3 - Subscriber found, but has not paid
            case 3:
                initializeAccessRequest(&reqPkt, i, 3, 4086668821);
                pktLen = buildAccessPacket(buffer, reqPkt);
                sendto(sockfd, buffer, pktLen, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
                break;
            // Test case 4 - Subscriber found and paid - permitted to access the network
            case 4:
                initializeAccessRequest(&reqPkt, i, 2, 4086808821);
                pktLen = buildAccessPacket(buffer, reqPkt);
                sendto(sockfd, buffer, pktLen, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
                break;
        }

        // handle the response from server
        struct sockaddr from;
        socklen_t fromSize = sizeof(from);
        memset(&from, 0, sizeof(from));
        bind(sockfd, &from, fromSize);
        char buffer2[1024];

        int counter = 0;
        while (counter < 3) {
            // get response packet from server
            if (recvfrom(sockfd, buffer2, sizeof(buffer2), 0, (struct sockaddr* )&from, &fromSize) >= 0) {
                // determine what the response is and print out message
                handleResponse(buffer2);
                break;
            }
            else {
                // no response from server - increment counter and then re-transmit
                counter++;
                sendto(sockfd, buffer, pktLen, 0, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
            }
        }
        // if client has tried sending same packet 3 times already and server still doesn't respond, print error
        if (counter == 3) {
            printf("Server does not respond\n");
        }
    }

    return 0;
}