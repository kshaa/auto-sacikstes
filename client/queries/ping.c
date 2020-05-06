#include "../../common/protocol/protocol_messages.h"
#include "../../common/networking/fetch.h"
#include "../state.h"
#include "../const.h"
#include <time.h>
#include <netinet/in.h>

int ping() {
    int success = 1;
    unsigned char * recvBuff;

    // Use connection from global client state
    int connectionfd = server.fd;
    int connectionfdUDP = server.fdUDP;
    printf("%d", connectionfdUDP);

    // Generate request
    ProtocolPingRequest request;
    strncpy(request.type, PROTOCOL_PING_TYPE, sizeof(PROTOCOL_PING_TYPE));

    // Fetch ping response using TCP
    recvBuff = fetch(connectionfd, &request, sizeof(ProtocolPingRequest));
    if (!isMessageType(recvBuff, PROTOCOL_PONG_TYPE)) {
        success = 0;
        return success;
    }

    // Fetch ping response using UDP
    // struct sockaddr_in * serverAddress = getVolatileAddress(server.address, server.port); 
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(server.address);
    serverAddress.sin_port = htons(server.port);
    
    int sendSuccess = sendMessageTo(connectionfdUDP, &request, sizeof(ProtocolPingRequest), MSG_DONTWAIT, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in));
    if (!sendSuccess) {
        printf("AAA");
        success = 0;
        return success;
    }
        printf("BBB");

    int startSuccess = 0;
    int timeoutClicker = 0;
    int timeoutClickerMax = 100;
    while (!startSuccess) {
        recvBuff = receiveMessageFrom(connectionfdUDP, 0, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
        if (!recvBuff) {
            startSuccess = 0;
        }

        // Sleep a little, to avoid making CPU sad
        struct timespec timeout;
        timeout.tv_sec = 0;
        timeout.tv_nsec = 1; // 0.01 ms
        nanosleep(&timeout, NULL);
        timeoutClicker++;
    printf("%d", timeoutClicker);


        // Timeout after a while
        if (timeoutClicker > timeoutClickerMax) {
            success = 0;
            return success;
        }
    }

    if (!isMessageType(recvBuff, PROTOCOL_PONG_TYPE)) {
        success = 0;
        return success;
    }

    return success;
}