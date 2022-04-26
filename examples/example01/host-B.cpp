#include <sys-debug.h>
#include <string-utils.h>
#include <number-utils.h>

#include "intercomm.h"
#include "host-protocol.h"

void rxDataCallback(const uint8_t *buf, int buf_size)
{
    MyData data;

    memcpy((void *)&data, buf, buf_size);

    printf("<= my_millis(A) = %lu\n", data.my_millis);
}

Intercomm ic(Serial3, 115200, rxDataCallback);

void setup()
{
    Serial.begin(9600);
    setSystemPrinter(Serial);

    printf("START HOST B\n");

    ic.setup();
}

uint32_t m = millis();

uint8_t tx_data[DATA_SIZE_MAX];

void loop()
{
    //if (millis() - m > 500)
    {
        MyData data;
        data.my_millis = millis();
        printf("=> my_millis(B) = %lu\n", data.my_millis);
        ic.send((const uint8_t *)&data, sizeof(MyData));
        while (!ic.ackReceived())
        {
            // ...
        }
        printf("<= ack\n");

        m = millis();

        printf("  total (inv crc:%lu ; rtx:%lu)\n", ic.getInvalidCrcTotal(), ic.getRetransmissionTotal());
    }

    ic.loop();
}
