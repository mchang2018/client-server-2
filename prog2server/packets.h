#ifndef PACKETS_H
    #define PACKETS_H

    // start and end of packet IDs
    #define START_OF_PACKET_ID 0xFFFF
    #define END_OF_PACKET_ID 0XFFFF

    // access permissions
    #define ACCESS_PERMISSION 0xFFF8
    #define NOT_PAID 0xFFF9
    #define NOT_EXIST 0xFFFA
    #define ACCESS_GRANTED 0xFFFB


    // make struct for the payload (technology + source subscriber number)
    struct Payload {
        unsigned int technology;
        unsigned int sourceSubscriberNum;
    };

    // make struct for access permission request packet
    struct Access_permission_request {
        unsigned int startID;
        char clientID;
        unsigned int access;
        unsigned int segmentNum;
        unsigned int payloadLength;
        struct Payload payld;
        unsigned int endID;
    };

    // make struct for verification database format
    struct Verification_format {
        unsigned int subscriberNum;
        unsigned int technology;
        unsigned int paid;
    };

#endif