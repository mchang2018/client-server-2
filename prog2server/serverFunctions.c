#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "packets.h"
#include "serverFunctions.h"


// This function copies data from buffer to struct using memcpy
void bufferToStruct(char* buffer, void* x, int* index, int numBytes) {
    memcpy(x, &(buffer[*index]), numBytes);
    *index += numBytes;
}


// This function copies data from struct to buffer using memcpy
void structToBuffer(char* buffer, void* x, int* index, int numBytes) {
    memcpy(&(buffer[*index]), x, numBytes);
    *index += numBytes;
}


// Function to read an access permission request packet. Transfers data from buffer to struct
void readAccessRequest(char* buffer, struct Access_permission_request* accessPacket) {
    int length = 0;

    // copy start of packet ID from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->startID), &length, 2);

    // copy client ID from accessPacket to bufffer
    bufferToStruct(buffer, &(accessPacket->clientID), &length, 1);

    // copy access permission from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->access), &length, 2);

    // copy segment number from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->segmentNum), &length, 1);

    // copy payload length from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->payloadLength), &length, 1);

    // copy technology from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->payld.technology), &length, 1);

    // copy source subscriber number from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->payld.sourceSubscriberNum), &length, 4);

    // copy end of packet ID from accessPacket to buffer
    bufferToStruct(buffer, &(accessPacket->endID), &length, 2);
}


/* 
   This function opens and reads file "Verification_Database.txt" and saves the contents on the server
   and then returns the subscriber count
*/
int saveVerificationDatabase(struct Verification_format* subscriberInfo) {
    FILE* fp;
    char subscriberNumber[20];
    char tech[5];
    int paid;
    int subscriber = 0;

    // open file to be read
    fp = fopen("Verification_Database.txt", "r");
    if (fp == NULL) {
        printf("No such file.\n");
        exit(1);
    }

    // read contents of files and store into subscriberInfo
    while(fscanf(fp, "%s %s %d\n", subscriberNumber, tech, &paid) == 3) {
        // remove dashes from the subscriber number
        char subscriberNumber2[20];
        for (int i = 0, j = 0; i < 12; i++, j++) {
            if (i == 3 || i == 7) {
                j--;
                continue;
            }
            else {
                subscriberNumber2[j] = subscriberNumber[i];
            }
        }

        // store each line of subscriber details in file into subscriberInfo; convert from string to int when needed
        subscriberInfo[subscriber].subscriberNum = atoi(subscriberNumber2);
        subscriberInfo[subscriber].technology = atoi(tech);
        subscriberInfo[subscriber].paid = paid;

        // increment subscriber count
        subscriber++;
    }
    // close file
    fclose(fp);

    // return the subscriber count
    return subscriber;
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