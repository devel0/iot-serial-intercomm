---
title: include/intercomm.h


---

# include/intercomm.h








## Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[IC_Header](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/struct_i_c___header.md)**  |
| class | **[Intercomm](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/class_intercomm.md)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef void(*)(const uint8_t *buf, int buf_size) | **[InterCommRxDataCallback](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/intercomm_8h.md#typedef-intercommrxdatacallback)**  |





## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[IC_HEADER_SIZE](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/intercomm_8h.md#define-ic_header_size)**  |
|  | **[IC_RESEND_TRIES_MAX](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/intercomm_8h.md#define-ic_resend_tries_max)**  |
|  | **[IC_RESEND_INTERVAL_MS](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Files/intercomm_8h.md#define-ic_resend_interval_ms)**  |





## Types Documentation

### typedef InterCommRxDataCallback

```cpp
typedef void(* InterCommRxDataCallback) (const uint8_t *buf, int buf_size);
```
































## Macro Documentation

### define IC_HEADER_SIZE

```cpp
#define IC_HEADER_SIZE (sizeof([IC_Header](https://github.com/devel0/iot-serial-intercomm/tree/main/data/api/Classes/struct_i_c___header.md)))
```





























### define IC_RESEND_TRIES_MAX

```cpp
#define IC_RESEND_TRIES_MAX 3
```





























### define IC_RESEND_INTERVAL_MS

```cpp
#define IC_RESEND_INTERVAL_MS 1000
```































## Source code

```cpp
#ifndef _INTERCOMM_H
#define _INTERCOMM_H

#include <Arduino.h>

//
// Intercom protocol
// -----------------
//
//                     HEADER                                                          DATA
// ********************************************************************************    *********************************
// (8bit)       (8bit)       (8bit)    (16bit)            (16bit)      (16bit)         (data-len * 8bit)   (16bit)
// 'I' (0x49)   'C' (0x43)   <flags>   <data-ack-chksum>   <data-len>   <hdr-chksum>   <data>              <data-chksum>
//

#pragma pack(push, 1)
struct IC_Header
{
    uint8_t magic0;
    uint8_t magic1;

    uint8_t flag_ack : 1;

    uint16_t data_ack_chksum;

    uint16_t data_len;

    uint16_t hdr_chksum;
};
#pragma pack(pop)

//-----------------------------------------------------------------

#define IC_HEADER_SIZE (sizeof(IC_Header))

#define IC_RESEND_TRIES_MAX 3

#define IC_RESEND_INTERVAL_MS 1000

typedef void (*InterCommRxDataCallback)(const uint8_t *buf, int buf_size);

//-----------------------------------------------------------------

class Intercomm
{
    HardwareSerial &serial;
    unsigned long speed;

    uint8_t *rx_hdr_buf = NULL;
    int rx_hdr_buf_off = 0;
    IC_Header rx_hdr;

    bool waiting_ack = false;    
    IC_Header tx_hdr;
    const uint8_t *last_tx_data = NULL;
    uint16_t tx_data_chksum;
    uint32_t tx_timestamp = 0UL;
    int tx_retry = 0;

    uint8_t *rx_data_buf = NULL;
    int rx_data_buf_off = 0; 
    uint16_t rx_data_chksum = 0;   

    int max_data_len = 256;

    uint32_t invalid_crc_total = 0;
    uint32_t retransmission_total = 0;

    InterCommRxDataCallback rxCallback = NULL;

    void isend(const uint8_t *tx_data_buf, int len, int _tx_retry);        
    void sendAck();

public:
    Intercomm(HardwareSerial &_serial, unsigned long _speed, InterCommRxDataCallback _rxCallback, int _max_data_len = 256);
    ~Intercomm();

    void setup();
    void send(const uint8_t *tx_data_buf, int len); 

    bool ackReceived();
    void loop();

    inline uint32_t getInvalidCrcTotal() const { return invalid_crc_total; }
    inline uint32_t getRetransmissionTotal() const { return retransmission_total; }
};

#endif
```


-------------------------------


