#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "packets.h"
#include "serverFunctions.h"


int main() {
    //declare variables
    int sockfd, subscriberCount, n;
    bool subscriberExists = false;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct Access_permission_request accessPkt;
    struct Verification_format subscribers[50];

    // create server socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        printf("Error creating server socket.\n");
        exit(1);
    }  

    // initialize socket structure
    bzero((char*) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7573);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // bind serverAddr to socket
    bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    // initialize structs for the sender
    struct sockaddr client;
    socklen_t clientSize = sizeof(client);
    memset(&client, 0, sizeof(client));

    // load verification database
    subscriberCount = saveVerificationDatabase(subscribers);

    while (1) {
        // reset the buffer
        memset(&buffer, 0, sizeof(buffer));

        // receive request packet from client
        recvfrom(sockfd, buffer, sizeof(buffer), 0, &client, &clientSize);

        // read the request packet and transfer data from buffer to Access_permission_request struct
        readAccessRequest(buffer, &accessPkt);
        
        // search for the subscriber and if found, see if they paid or not
        for (int i = 0; i < subscriberCount; i++) {
            if (accessPkt.payld.sourceSubscriberNum == subscribers[i].subscriberNum && accessPkt.payld.technology == subscribers[i].technology) {
                // set subscriberExists to true to indicate we found in database
                subscriberExists = true;
                
                // Case 1 - subscriber exists and has paid
                if (subscribers[i].paid == 1) {
                    accessPkt.access = ACCESS_GRANTED;
                    printf("Subscriber found - ACCESS GRANTED\n");
                    break;
                }
                // Case 2 - subscriber exists but has not paid
                else {
                    accessPkt.access = NOT_PAID;
                    printf("Subscriber found - NOT PAID\n");
                    break;
                }
            }
        }
        // Case 3 - subscriber does not exist on database
        if (subscriberExists == false) {
            accessPkt.access = NOT_EXIST;
            printf("Subscriber NOT FOUND\n");
        }

        // build and send the appropriate response packet back to client
        n = buildAccessPacket(buffer, accessPkt);
        if (sendto(sockfd, buffer, n, 0, (struct sockaddr*) &client, clientSize) < 0) {
            printf("Error sending response packet.\n\n");
        }
        else {
            printf("Response packet successfully sent!\n\n");
        }
    }
    return 0;
}