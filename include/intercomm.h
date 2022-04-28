#ifndef _INTERCOMM_H
#define _INTERCOMM_H

#include <Arduino.h>

#include <type_traits>

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
    /**
     * @brief I
     */
    uint8_t magic0;
    /**
     * @brief C
     */
    uint8_t magic1;

    uint8_t flag_ack : 1;

    uint8_t flag_datagram : 1;

    uint16_t data_ack_chksum;

    uint16_t data_len;

    uint16_t hdr_chksum;
};
#pragma pack(pop)

//
// Intercom protocol (Datagram) : for urgent non acked notification ( no data, only 4 bytes in header : 32bit flags )
// -----------------
//
//                     HEADER
// **************************************************************
// (8bit)       (8bit)       (8bit)    (32bit)      (16bit)
// 'I' (0x49)   'C' (0x43)   <flags>   <datagram>   <hdr-chksum>
//

#pragma pack(push, 1)
struct IC_Header_Datagram
{
    /**
     * @brief I
     */
    uint8_t magic0;
    /**
     * @brief C
     */
    uint8_t magic1;
    
    uint8_t flag_ack : 1;

    uint8_t flag_datagram : 1;

    uint8_t custom[4];

    uint16_t hdr_chksum;
};
#pragma pack(pop)

static_assert(sizeof(IC_Header) == sizeof(IC_Header_Datagram));

//-----------------------------------------------------------------

#define IC_HEADER_SIZE (sizeof(IC_Header))

#define IC_ERRCODE_NOERROR 0
#define IC_ERRCODE_RESEND_EXCEEDED 1
#define IC_ERRCODE_WAITING_ACK 2

typedef void (*InterCommRxDataCallback)(const uint8_t *buf, int buf_size);

/**
 * @brief rx 4 bytes datagram
 */
typedef void (*InterCommRxDatagramCallback)(const uint8_t *custom);

//-----------------------------------------------------------------

class Intercomm
{
    int is_in_error = 0;
    int resend_tries = 3;
    uint32_t resend_interval_ms = 1000;

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

    IC_Header_Datagram tx_datagram;

    uint8_t *rx_data_buf = NULL;
    int rx_data_buf_off = 0;
    uint16_t rx_data_chksum = 0;

    int max_data_len = 256;

    uint32_t invalid_crc_total = 0;
    uint32_t retransmission_total = 0;

    InterCommRxDataCallback rxCallback = NULL;
    InterCommRxDatagramCallback rxDatagramCallback = NULL;

    void isend(const uint8_t *tx_data_buf, int len, int _tx_retry);
    void isend_datagram(const uint8_t *tx_datagram_buf);
    void sendAck();

public:
    Intercomm(HardwareSerial &_serial,
              unsigned long _speed,
              InterCommRxDataCallback _rxCallback,
              int _max_data_len = 256,
              int _resend_tries = 3,
              uint32_t _resend_interval_ms = 1000,
              InterCommRxDatagramCallback _rxDatagramCallback = NULL);
    ~Intercomm();

    void setup();
    void send(const uint8_t *tx_data_buf, int len);
    /**
     * @brief send custom datagram ( custom must a 4 byte array or equivalent struct of 32bits )
     * 
     * no ack expected for a datagram, other end just recognize and process packet if crc matches
     */
    void sendDatagram(const uint8_t *custom);

    /**
     * @brief after a send, place a "while (!ic.ackReceived()) { ... }"
     */
    bool ackReceived();
    void loop();

    inline uint32_t getInvalidCrcTotal() const { return invalid_crc_total; }
    inline uint32_t getRetransmissionTotal() const { return retransmission_total; }
    inline bool isInError() const { return is_in_error; }
};

#endif
