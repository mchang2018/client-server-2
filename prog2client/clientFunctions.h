#ifndef CLIENTFUNCTIONS_H
    #define CLIENTFUNCTIONS_H

    #include "packets.h"

    // declare functions
    void initializeAccessRequest(struct Access_permission_request* requestPacket, int segNum, int tech, unsigned int subscriberNum);
    int buildAccessPacket(char* buffer, struct Access_permission_request accessPacket);
    void handleResponse(char* buff);
    void structToBuffer(char* buffer, void* x, int* index, int numBytes); 

#endif