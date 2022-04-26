#include "internet-checksum.h"

uint32_t CheckSumPartial(uint32_t prevSum, const uint8_t *data, uint16_t count, bool last)
{
    auto sum = prevSum;

    while (count > 1)
    {
        sum += (((uint32_t)(*data)) << 8) | (*(data + 1));
        count -= 2;
        data += 2;
    }

    if (count)
    {
        sum += (uint32_t)(((uint16_t)*data) << 8);
    }

    return sum;
}

uint16_t CheckSumFinalize(uint32_t sum)
{
    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    uint16_t chksum = (~sum) & 0xffff;

    return chksum;
}

uint16_t InternetChecksum(const uint8_t *data, uint16_t len)
{
    return CheckSumFinalize(CheckSumPartial(0, data, len, true));
}