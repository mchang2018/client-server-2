#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "packets.h"
#include "clientFunctions.h"


// This function copies data from struct to buffer using memcpy
void structToBuffer(char* buffer, void* x, int* index, int numBytes) {
    memcpy(&(buffer[*index]), x, numBytes);
    *index += numBytes;
}


// This function populates an Access_permission_request structure with correct information
void initializeAccessRequest(struct Access_permission_request* requestPacket, int segNum, int tech, unsigned int subscriberNum) {
    requestPacket->startID = START_OF_PACKET_ID;
    requestPacket->clientID = 5;
    requestPacket->access = ACCESS_PERMISSION;
    requestPacket->segmentNum = segNum;
    requestPacket->payloadLength = 5;
    requestPacket->payld.technology = tech;
    requestPacket->payld.sourceSubscriberNum = subscriberNum;
    requestPacket->endID = END_OF_PACKET_ID;
}


// This function builds an Access_permission_request packet (transfers data from struct to buffer)
int buildAccessPacket(char* buffer, struct Access_permission_request accessPacket) {
    int length = 0;

    structToBuffer(buffer, &(accessPacket.startID), &length, 2);

    structToBuffer(buffer, &(accessPacket.clientID), &length, 1);

    structToBuffer(buffer, &(accessPacket.access), &length, 2);

    structToBuffer(buffer, &(accessPacket.segmentNum), &length, 1);

    structToBuffer(buffer, &(accessPacket.payloadLength), &length, 1);

    structToBuffer(buffer, &(accessPacket.payld.technology), &length, 1);

    structToBuffer(buffer, &(accessPacket.payld.sourceSubscriberNum), &length, 4);

    structToBuffer(buffer, &(accessPacket.endID), &length, 2);

    return length;
}


// This function determines what the access permission/status is in the response from server
void handleResponse(char* buff) {
    int permission;

    // get access permission status from the server response
    memcpy(&permission, &(buff[3]), 2);

    // check access permission status and print appropriate message
    if (permission == NOT_PAID) {
        printf("Subscriber has not paid.\n\n");
    }
    else if (permission == NOT_EXIST) {
        printf("Subscriber does not exist on database.\n\n");
    }
    else if(permission == ACCESS_GRANTED) {
        printf("Subscriber permitted to access the network.\n\n");
    }
}