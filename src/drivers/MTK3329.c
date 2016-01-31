
#include <MTK3329.h>

void MTK3329ParseGPRMC(struct MTK3329Module *const myMTK3329, uint8_t *const message) {
    float latitude;
    float longitude;
    char *RMCMessage = message;
    char degreebuff[10];

    // parse time
    RMCMessage = strchr(RMCMessage,',')+1;
    myMTK3329->timef = atof(RMCMessage);
    uint32_t time = myMTK3329->timef;
    myMTK3329->hour = time/10000;
    myMTK3329->minute = (time % 10000)/ 100;
    myMTK3329->second = (time % 100);

    //parse valid data
    RMCMessage = strchr(RMCMessage,',')+1;
    if (RMCMessage[0] == 'A') {
        myMTK3329->validSalad = true;
    } else if (RMCMessage[0] == 'V') {
        myMTK3329->validSalad = false;
    }

    //parse latitude
    RMCMessage = strchr(RMCMessage,',')+1;
    strncpy(degreebuff, RMCMessage,2);
    RMCMessage += 2;
    degreebuff[2] = '\0';
    long degree = atol(degreebuff) * 10000000;
    strncpy(degreebuff, RMCMessage, 2); // minutes
    RMCMessage += 3; // skip decimal point
    strncpy(degreebuff + 2, RMCMessage, 4);
    degreebuff[6] = '\0';
    long minutes = 50 * atol(degreebuff) / 3;
    latitude = degree / 100000 + minutes * 0.000006F;
    myMTK3329->latitude = (latitude-100*(int)(latitude/100))/60.0;
    myMTK3329->latitude += (int)(latitude/100);

    RMCMessage = strchr(RMCMessage, ',')+1;
    if (',' != *RMCMessage) {
        if (RMCMessage[0] == 'S') {
            myMTK3329->latitude *= -1.0;
        };
    };


//parse longitude
    RMCMessage = strchr(RMCMessage,',')+1;
    if (',' != *RMCMessage) {
        strncpy(degreebuff, RMCMessage, 3);
        RMCMessage += 3;
        degreebuff[3] = '\0';
        degree = atol(degreebuff) * 10000000;
        strncpy(degreebuff, RMCMessage, 2); // minutes
        RMCMessage += 3; // skip decimal point
        strncpy(degreebuff + 2, RMCMessage, 4);
        degreebuff[6] = '\0';
        minutes = 50 * atol(degreebuff) / 3;
        longitude = degree / 100000 + minutes * 0.000006F;
        myMTK3329->longitude = (longitude-100*(int)(longitude/100))/60.0;
        myMTK3329->longitude += (int)(longitude/100);
    }

    RMCMessage = strchr(RMCMessage, ',')+1;
    if (',' != *RMCMessage) {
        if (RMCMessage[0] == 'W') {
            myMTK3329->longitude *= -1.0;
        };

    }

}




void MTK3329ParseMessage(struct MTK3329Module *const myMTK3329,uint8_t *const messageToParse) {
    int32_t i = 0;
    do {
        if (messageToParse[i] == 0x24) {
            volatile uint8_t header[3];
            header[0] = messageToParse[i+3];
            header[1] = messageToParse[i+4];
            header[2] = messageToParse[i+5];


            if ((header[0] == 0x52) & (header[1] == 0x4d) & (header[2] == 0x43)) {
                MTK3329ParseGPRMC(myMTK3329, &messageToParse[i]);
            } else {
            }

        }

    }  while (++i < 255);
}
