#ifndef SERVERFUNCTIONS_H
    #define SERVERFUNCTIONS_H

    #include "packets.h"

    // declare functions
    void readAccessRequest(char* buffer, struct Access_permission_request* accessPacket);
    int saveVerificationDatabase(struct Verification_format* subscriberInfo);
    int buildAccessPacket(char* buffer, struct Access_permission_request accessPacket);
    void bufferToStruct(char* buffer, void* x, int* index, int numBytes);
    void structToBuffer(char* buffer, void* x, int* index, int numBytes); 

#endif