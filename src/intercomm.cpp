#include "intercomm.h"

#include <sys-debug.h>
#include "internet-checksum.h"

Intercomm::Intercomm(HardwareSerial &_serial,
                     unsigned long _speed,
                     InterCommRxDataCallback _rxCallback,
                     int _max_data_len,
                     int _resend_tries,
                     uint32_t _resend_interval_ms) : serial(_serial), speed(_speed), rxCallback(_rxCallback), max_data_len(_max_data_len), resend_tries(_resend_tries), resend_interval_ms(_resend_interval_ms)
{
    rx_hdr_buf = new uint8_t[IC_HEADER_SIZE];
    rx_data_buf = new uint8_t[max_data_len];
}

Intercomm::~Intercomm()
{
    delete rx_data_buf;
    delete rx_hdr_buf;
}

void Intercomm::setup()
{
    serial.begin(speed);
    while (serial.available())
        serial.read();

    // printf("setup (IC_HEADER_SIZE:%d)\n", IC_HEADER_SIZE);
}

void Intercomm::isend(const uint8_t *tx_data_buf, int len, int _tx_retry)
{
    if (_tx_retry == 0 && waiting_ack)
    {
        is_in_error = IC_ERRCODE_WAITING_ACK;
        error("waiting acknowledge");
    }
    else
    {
        tx_hdr.magic0 = 'I';
        tx_hdr.magic1 = 'C';
        tx_hdr.flag_ack = false;
        tx_hdr.data_ack_chksum = 0;
        tx_hdr.data_len = len;
        auto tx_hdr_ptr = (const uint8_t *)&tx_hdr;
        tx_hdr.hdr_chksum = InternetChecksum(tx_hdr_ptr, IC_HEADER_SIZE - 2);

        tx_data_chksum = InternetChecksum(tx_data_buf, len);

        serial.write(tx_hdr_ptr, IC_HEADER_SIZE);
        serial.write(tx_data_buf, len);
        serial.write((const uint8_t *)&tx_data_chksum, sizeof(uint16_t));

        waiting_ack = true;
        tx_retry = _tx_retry;
        tx_timestamp = millis();
        last_tx_data = tx_data_buf;
    }
}

void Intercomm::sendAck()
{
    tx_hdr.magic0 = 'I';
    tx_hdr.magic1 = 'C';
    tx_hdr.flag_ack = true;
    tx_hdr.data_ack_chksum = rx_data_chksum;
    tx_hdr.data_len = 0;
    auto tx_hdr_ptr = (const uint8_t *)&tx_hdr;
    tx_hdr.hdr_chksum = InternetChecksum(tx_hdr_ptr, IC_HEADER_SIZE - 2);

    serial.write(tx_hdr_ptr, IC_HEADER_SIZE);

    // printf("sent ack for rx chksum %d\n", rx_data_chksum);
}

void Intercomm::send(const uint8_t *tx_data_buf, int len)
{
    isend(tx_data_buf, len, 0);
}

bool Intercomm::ackReceived()
{
    loop();
    return !waiting_ack;
}

void Intercomm::loop()
{
    if (is_in_error != IC_ERRCODE_NOERROR)
        return;

    if (waiting_ack)
    {
        if (tx_retry >= resend_tries)
        {
            is_in_error = IC_ERRCODE_RESEND_EXCEEDED;
            error("comm error ; not rx ack in time");
        }
        if (millis() - tx_timestamp > resend_interval_ms)
        {
            ++retransmission_total;
            isend(last_tx_data, tx_hdr.data_len, ++tx_retry);
        }
    }

    while (serial.available())
    {
        int c = serial.read();

        // printf("rx c:%c [%d]\n", c, c);

        if (rx_hdr_buf_off == 0 && c == 'I')
        {
            rx_hdr_buf[rx_hdr_buf_off++] = c;
        }
        else if (rx_hdr_buf_off == 1 && c == 'C')
        {
            rx_hdr_buf[rx_hdr_buf_off++] = c;
        }
        else if (rx_hdr_buf_off < IC_HEADER_SIZE - 1)
        {
            rx_hdr_buf[rx_hdr_buf_off++] = c;
        }
        else if (rx_hdr_buf_off == IC_HEADER_SIZE - 1)
        {
            rx_hdr_buf[rx_hdr_buf_off++] = c;

            memcpy((uint8_t *)&rx_hdr, rx_hdr_buf, IC_HEADER_SIZE);

            auto chksum = InternetChecksum(rx_hdr_buf, IC_HEADER_SIZE - 2);

            if (chksum == rx_hdr.hdr_chksum)
            {
                if (rx_hdr.flag_ack && waiting_ack && rx_hdr.data_ack_chksum == tx_data_chksum)
                {
                    waiting_ack = false;
                    rx_hdr_buf_off = 0;
                    tx_retry = 0;
                }
                else
                {
                    ++rx_hdr_buf_off;

                    rx_data_buf_off = 0;
                }
            }
            else
            {
                debug("  ** inv hdr crc %d instead of expected %d\n",
                      rx_hdr.hdr_chksum, chksum);

                rx_hdr_buf_off = 0;
                // ++invalid_hdr_crc;
                // header_rx_cnt = 0; // invalidate packet
            }
        }
        else // rx data
        {
            if (rx_data_buf_off < rx_hdr.data_len)
            {
                rx_data_buf[rx_data_buf_off++] = c;
            }
            else if (rx_data_buf_off == rx_hdr.data_len)
            {
                ((uint8_t *)&rx_data_chksum)[0] = c;
                ++rx_data_buf_off;
            }
            else if (rx_data_buf_off == rx_hdr.data_len + 1)
            {
                ((uint8_t *)&rx_data_chksum)[1] = c;

                auto data_chksum = InternetChecksum(rx_data_buf, rx_hdr.data_len);

                if (data_chksum != rx_data_chksum)
                {
                    ++invalid_crc_total;
                }
                else
                {
                    sendAck();

                    rxCallback(rx_data_buf, rx_hdr.data_len);
                }

                rx_hdr_buf_off = 0;
            }
        }
    }
}
