#include "../../common/protocol/protocol_messages.h"
#include "../../common/networking/fetch.h"
#include "../state.h"
#include "../const.h"

int ping() {
    // Use connection from global client state
    int connectionfd = server.fd;

    // Generate request
    ProtocolPingRequest request;
    strncpy(request.type, PROTOCOL_PING_TYPE, sizeof(PROTOCOL_PING_TYPE));

    // Fetch games
    unsigned char * recvBuff = fetch(connectionfd, &request, sizeof(ProtocolPingRequest));

    // Process response
    if (isMessageType(recvBuff, PROTOCOL_PONG_TYPE)) {
        return 1;
    } else {
        return 0;
    }
}