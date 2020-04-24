#include <stdio.h>
#include "translate.h"

// Refactored from source: https://www.lemoda.net/c/ip-to-integer/
unsigned int translateIPAddress(const char * stringAddress)
{
    int success = 1;
    unsigned int intAddress = 0;
    const char * octetStart = stringAddress;

    // Process the IP address - 4 octets
    for (int i = 0; i < 4 && success; i++) {
        char tmpChar;
        int octet = 0;

        // Process one octet of the IP address
        int processingOctet = 1; 
        while (processingOctet) {
            // Read one of the octet chars'
            tmpChar = *octetStart;
            // Bump octet pointer
            octetStart++;
            if (tmpChar >= '0' && tmpChar <= '9') {
                // Read digit
                octet *= 10;
                octet += tmpChar - '0';
            } else if ((i < 3 && tmpChar == '.') || i == 3) {
                // Reached end of octet
                processingOctet = 0;
            } else {
                // Incorrect IP address
                intAddress = INVALID_IP_ADDRESS;
                // Stop processing octet
                processingOctet = 0;
                // Stop processing address
                success = 0;
            }
        }

        // Calculate new address if possible
        if (success) {
            if (octet <= 256) {
                // Valid address
                intAddress *= 256;
                intAddress += octet;
            } else {
                // Incorrect IP address
                intAddress = 0;
                // Stop processing address
                success = 0;
            }
        }
    }

    return intAddress;
}
